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

	ifstream fs;
	fs.open((ATMdata_ptr->getInputFileName()).c_str());
	string cmdLine;

	//initialize usleep parameters
	unsigned int ATMSleep=100000;	//100mSec


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

			//see that account doesn't exist
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			int initialAmount=atoi(args[3].c_str());
			readLock();
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			map<int, Account>::iterator endAcc = accounts.end();
			readUnLock();
			if(desiredAccount != endAcc){

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_O);
				writeToLog((void*)data);
				sleep(1);
			} else {
				//add account
				Account newAccount(accountNumber, accountPassword, initialAmount);
				writeLock();
				accounts.insert(pair<int, Account>(accountNumber,newAccount));
				writeUnLock();
				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, newAccount.get_balance(), \
						-1, -1, -1, -1,  OK_O);
				writeToLog((void*)data);
			}

		}
		else if (args[0]=="D"){	//deposit into account
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			int amount=atoi(args[3].c_str());
			readLock();
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			map<int, Account>::iterator endAcc = accounts.end();
			readUnLock();
			if (desiredAccount == endAcc){
				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_ACCOUNT);
				writeToLog((void*)data);
				sleep(1);
			} else {
				int res = (*desiredAccount).second.deposit(accountPassword,amount);
				if (res == -1){
					LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, -1, \
							-1, -1, -1, -1, BAD_D);
					writeToLog((void*)data);
				}
				else {
					LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, \
							res, amount, -1, -1, -1, OK_D);
					writeToLog((void*)data);
				}
			}
		}

		else if (args[0]=="W"){	//withdraw from account
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			int amount=atoi(args[3].c_str());
			readLock();
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			map<int, Account>::iterator endAcc = accounts.end();
			readUnLock();
			if (desiredAccount == endAcc){
				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_ACCOUNT);
				writeToLog((void*)data);
				sleep(1);
			} else {
				int res = (*desiredAccount).second.withdrawal(accountPassword,amount);
				if (res == -1){
					LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, -1, \
							-1, -1, -1, -1, BAD_W_PASSWORD);
					writeToLog((void*)data);
				}
				else if (res == -2){
					LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, -1, \
							amount, -1, -1, -1, BAD_W_BALANCE);
					writeToLog((void*)data);
				}
				else {
					LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, \
							res, amount, -1, -1, -1, OK_W);
					writeToLog((void*)data);
				}
			}
		}


		else if (args[0]=="B"){	//get balance
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			readLock();
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			map<int, Account>::iterator endAcc = accounts.end();
			readUnLock();
			if (desiredAccount == endAcc){
				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_ACCOUNT);
				writeToLog((void*)data);
				sleep(1);
			} else {
				int res = (*desiredAccount).second.get_balance_atm(accountPassword);
				if (res == -1){
					LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, \
							-1, -1, -1, -1, -1, BAD_B);
					writeToLog((void*)data);
				}
				else {
					LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, \
							res, -1, -1, -1, -1, OK_B);
					writeToLog((void*)data);
				}
			}
		}


		else if (args[0]=="T"){	//do transaction
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			int amount=atoi(args[4].c_str());
			int targetAccountNumber=atoi(args[3].c_str());
			readLock();
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			map<int, Account>::iterator endAcc = accounts.end();
			readUnLock();
			if (desiredAccount == endAcc){
				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1, -1,  BAD_ACCOUNT);
				writeToLog((void*)data);
				sleep(1);
			} else {
				readLock();
				map<int, Account>::iterator targetAccount = accounts.find(targetAccountNumber);
				endAcc = accounts.end();
				readUnLock();
				if (targetAccount == endAcc){
					LogData* data=new LogData(ATMdata_ptr->getID(), targetAccountNumber, -1, -1, \
							-1, -1, -1, -1,  BAD_ACCOUNT);
					writeToLog((void*)data);
					sleep(1);
				} else {
					int traget_balance = 0;
					int res = (*desiredAccount).second.transfer(accountPassword,(*targetAccount).second,amount,&traget_balance);
					if (res == -1){
						LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, \
								-1, -1, -1,	-1, -1, BAD_T_PASSWORD);
						writeToLog((void*)data);
					}
					else if (res == -2){
						LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, \
								-1, amount, -1,	-1, -1, BAD_T_BALANCE);
						writeToLog((void*)data);
					}
					else {
						LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, accountPassword, \
								res, amount, targetAccountNumber,\
								traget_balance, -1, OK_T);
						writeToLog((void*)data);
					}
				}
			}
		}


		else {
		}
	}

	pthread_exit(NULL);
}
