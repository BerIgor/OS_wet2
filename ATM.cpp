#include "ATM.h"
#include "logger.h"
#include "account.h"
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>	//required for exit()
#include <iostream> //required for std::cout
#include <unistd.h>	//required for usleep

extern pthread_mutex_t mapWriteLock;
extern pthread_mutex_t mapReadLock;
extern int map_read_cnt;


using namespace std;


void readLock(){
	//READERS WRITERS 2016S
	pthread_mutex_lock(&mapReadLock);
	map_read_cnt++;
	if (map_read_cnt == 1){
		pthread_mutex_lock(&mapWriteLock);
	}
	pthread_mutex_unlock(&mapReadLock);
}

void writeLock(){
	//READERS WRITERS 2016S
	pthread_mutex_lock(&mapWriteLock);
}

void readUnLock(){
	//READERS WRITERS 2016S
	pthread_mutex_lock(&mapReadLock);
	map_read_cnt--;
	if (map_read_cnt == 0){
		pthread_mutex_unlock(&mapWriteLock);
	}
	pthread_mutex_unlock(&mapReadLock);
}

void writeUnLock(){
	//READERS WRITERS 2016S
	pthread_mutex_unlock(&mapWriteLock);
}


void* ATMOperator(void* inputData){

	ATMData* ATMdata_ptr=static_cast<ATMData*>(inputData);

//	cout << (ATMdata_ptr->getInputFileName()).c_str() << "  " << ATMdata_ptr->getID() << endl;
	ifstream fs;
	fs.open((ATMdata_ptr->getInputFileName()).c_str());
	string cmdLine;

	//initialize usleep parameters
	unsigned int ATMSleep=100000;


	while (getline(fs, cmdLine)){
		usleep(ATMSleep);
		string args[5];
		int ind=0;
		size_t pos;
		string delimiter=" ";
		while ((pos = cmdLine.find(delimiter)) != string::npos) {
		    args[ind] = cmdLine.substr(0, pos);
		    cmdLine.erase(0, pos + delimiter.length());
		    ind++;
		}
		args[ind]=cmdLine.substr(0, pos);	//this works, not sure why


		if(args[0]=="O"){	//open a new account: account password initial_amount
//			printf("got o\n");
			//see that account doesn't exist
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			int initialAmount=atoi(args[3].c_str());
			readLock();
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			map<int, Account>::iterator endAcc = accounts.end();
			readUnLock();
			if(desiredAccount != endAcc){
//				printf("ERROR: account already exists\n");

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_O);
				writeToLog((void*)data);
				sleep(1);
				break;
				//TODO: handle account already exists
			}
			//add account
			Account newAccount(accountNumber, accountPassword, initialAmount);
			writeLock();
			accounts.insert(pair<int, Account>(accountNumber,newAccount));
			writeUnLock();

//			printf("opened new account, number %d\n",accountNumber);
			LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, newAccount.get_balance(), \
					-1, -1, -1, -1,  OK_O);
			writeToLog((void*)data);

		}
		else if (args[0]=="D"){	//deposit into account
//			printf("got d\n");
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			int amount=atoi(args[3].c_str());
			readLock();
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			map<int, Account>::iterator endAcc = accounts.end();
			readUnLock();
			if (desiredAccount == endAcc){
//				printf("Error <ATM ID>: Your transaction failed – account id %04d does not exist\n",accountNumber);
				//TODO: handle account doesn't exists

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_ACCOUNT);
				writeToLog((void*)data);
			}
			int res = (*desiredAccount).second.deposit(accountPassword,amount);
			//TODO: actually write it to the log
			if (res == -1){
//				printf("Error <ATM ID>: Your transaction failed – password for account id %0d is incorrect\n"\
						,accountNumber);

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, -1, \
						-1, -1, -1, -1, BAD_D);
				writeToLog((void*)data);
			}
			else {
//				printf("<ATM ID>: Account %04d new balance is %d after %d $ was deposited\n",accountNumber,res,amount);

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, \
						(*desiredAccount).second.get_balance(), -1, -1, -1, -1, OK_D);
				writeToLog((void*)data);
			}
		}

		else if (args[0]=="W"){	//withdraw from account
//			printf("got w\n");
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			int amount=atoi(args[3].c_str());
			readLock();
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			map<int, Account>::iterator endAcc = accounts.end();
			readUnLock();
			if (desiredAccount == endAcc){
//				printf("Error <ATM ID>: Your transaction failed – account id %04d does not exist\n",accountNumber);
				//TODO: handle account doesn't exists

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_ACCOUNT);
				writeToLog((void*)data);
			}
			int res = (*desiredAccount).second.withdrawal(accountPassword,amount);
			if (res == -1){
//				printf("Error <ATM ID>: Your transaction failed – password for account id %0d is incorrect\n"\
//						,accountNumber);

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, -1, \
						-1, -1, -1, -1, BAD_W_PASSWORD);
				writeToLog((void*)data);
			}
			else if (res == -2){
//				printf("Error <ATM ID>: Your transaction failed – account id %04d balance is lower than %d\n"\
//						,accountNumber,amount);

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, -1, \
						-1, -1, -1, -1, BAD_W_BALANCE);
				writeToLog((void*)data);
			}
			else {
//				printf("<ATM ID>: Account %04d new balance is %d after %d $ was withdrew\n",\
//						accountNumber,res,amount);

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, \
						(*desiredAccount).second.get_balance(), amount, -1, -1, -1, OK_W);
				writeToLog((void*)data);
			}
		}


		else if (args[0]=="B"){	//get balance
//			printf("got b\n");
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			readLock();
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			map<int, Account>::iterator endAcc = accounts.end();
			readUnLock();
			if (desiredAccount == endAcc){
//				printf("Error <ATM ID>: Your transaction failed – account id %04d does not exist\n"\
//						,accountNumber);
				//TODO: handle account doesn't exists

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_ACCOUNT);
				writeToLog((void*)data);
			}
			int res = (*desiredAccount).second.get_balance_atm(accountPassword);
			if (res == -1){
//				printf("Error <ATM ID>: Your transaction failed – password for account id %0d is incorrect\n"\
//						,accountNumber);

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, \
						-1, -1, -1, -1, -1, BAD_B);
				writeToLog((void*)data);
			}
			else {
//				printf("<ATM ID>: Account %04d balance is %d\n",accountNumber,res);

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, \
						(*desiredAccount).second.get_balance(), -1, -1, -1, -1, OK_B);
				writeToLog((void*)data);
			}
		}


		else if (args[0]=="T"){	//do transaction
//			printf("got t\n");
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			int amount=atoi(args[4].c_str());
			int targetAccountNumber=atoi(args[3].c_str());
			readLock();
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			map<int, Account>::iterator endAcc = accounts.end();
			readUnLock();
			if (desiredAccount == endAcc){
//				printf("Error <ATM ID>: Your transaction failed – account id %04d does not exist\n",accountNumber);
				//TODO: handle account doesn't exists

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_ACCOUNT);
				writeToLog((void*)data);
			}
			readLock();
			map<int, Account>::iterator targetAccount = accounts.find(targetAccountNumber);
			endAcc = accounts.end();
			readUnLock();
			if (desiredAccount == endAcc){
//				printf("Error <ATM ID>: Your transaction failed – account id %04d does not exist\n"\
//						,targetAccountNumber);
				//TODO: handle account doesn't exists

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_ACCOUNT);
				writeToLog((void*)data);
			}
			int res = (*desiredAccount).second.transfer(accountPassword,(*targetAccount).second,amount);
			if (res == -1){
//				printf("Error <ATM ID>: Your transaction failed – password for account id %0d is incorrect\n"\
//						,accountNumber);

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, \
						-1, -1, -1,	-1, -1, BAD_T_PASSWORD);
				writeToLog((void*)data);
			}
			else if (res == -2){
//				printf("Error <ATM ID>: Your transaction failed – account id %04d balance is lower than %d\n"\
//						,accountNumber,amount);

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, \
						-1, amount, -1,	-1, -1, BAD_T_BALANCE);
				writeToLog((void*)data);
			}
			else {
/*				printf("<ATM ID>: Transfer %d from account %04d to account %04d new account balance is %d "
						"new target account balance is %d\n"\
						,amount,accountNumber,targetAccountNumber,\
						(*desiredAccount).second.get_balance(),(*targetAccount).second.get_balance());
*/
				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, \
						(*desiredAccount).second.get_balance(), amount, targetAccountNumber,\
						(*targetAccount).second.get_balance(), -1, OK_T);
				writeToLog((void*)data);
			}
		}


		else {
			printf("ERROR\n");	//TODO: handle bad command
		}
	}

	pthread_exit(NULL);
}
