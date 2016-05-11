#include "ATM.h"
#include "logger.h"
#include "account.h"
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>	//required for exit()
#include <iostream> //required for std::cout



using namespace std;

bool checkPassword(int providedPassword){

	return true;
}


void* ATMOperator(void* inputData){

	ATMData* ATMdata_ptr=static_cast<ATMData*>(inputData);

	cout << (ATMdata_ptr->getInputFileName()).c_str() << "  " << ATMdata_ptr->getID() << endl;
	ifstream fs;
	fs.open((ATMdata_ptr->getInputFileName()).c_str());
	string cmdLine;

	while (getline(fs, cmdLine)){
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
			printf("got o\n");
			//see that account doesn't exist
			int accountNumber=atoi(args[1].c_str());
			int accountPassword=atoi(args[2].c_str());
			int initialAmount=atoi(args[3].c_str());
			map<int, Account>::iterator desiredAccount = accounts.find(accountNumber);
			if(desiredAccount!=accounts.end()){
				printf("ERROR: account already exists\n");

				LogData* data=new LogData(ATMdata_ptr->getID(), accountNumber, -1, -1, \
						-1, -1, -1,  BAD_O);
				writeToLog((void*)data);
				//TODO: handle account already exists
			}
			//add account
			Account newAccount(accountNumber, accountPassword, initialAmount);
			accounts.insert(pair<int, Account>(accountNumber,newAccount));

		}
		else if (args[0]=="D"){	//deposit into account
			printf("got d\n");
		}
		else if (args[0]=="W"){	//withdraw from account
			printf("got w\n");
		}
		else if (args[0]=="B"){	//get balance
			printf("got b\n");
		}
		else if (args[0]=="T"){	//do transaction
			printf("got t\n");
		}
		else {
			printf("ERROR\n");	//TODO: handle bad command
		}
	}

	pthread_exit(NULL);
}
