#include "logger.h"
#include <fstream>
#include <stdio.h>	//required for bullshit printing

using namespace std;
void* writeToLog(void* _data){
	LogData* dataP=static_cast<LogData*>(_data);

	pthread_mutex_lock(&logMutex);
	ofstream logFile;
	logFile.open("log.txt");

	switch(dataP->type){
	case BAD_O:			//account open failed
		logFile<<"Error "<<dataP->atmID<<": Your transaction failed - ";
		logFile<<"account with the same id exists"<<endl;
		break;
	case BAD_D:			//deposit failed
		logFile<<"Error "<<dataP->atmID<<": Your transaction failed - ";
		logFile<<" password for account id "<<dataP->id;
		logFile<<" is incorrect"<<endl;
		break;
	case BAD_W_PASSWORD:	//withdraw failed - wrong password
		logFile<<"Error "<<dataP->atmID<<": Your transaction failed - ";
		logFile<<"password for account id "<<dataP->id;
		logFile<<" is incorrect"<<endl;
		break;
	case BAD_W_BALANCE:	//withdraw failed - insufficient funds
		logFile<<"Error "<<dataP->atmID<<": Your transaction failed - ";
		logFile<<"account id "<<dataP->id;
		logFile<<" balance is lower than "<<dataP->amount<<endl;
		break;
	case BAD_B:			//balance query failed
		logFile<<"Error "<<dataP->atmID<<": Your transaction failed - ";
		logFile<<"password for account id "<<dataP->id<<" is incorrect"<<endl;
		break;
	case BAD_T_PASSWORD: //transfer failed - wrong password
		logFile<<"Error "<<dataP->atmID<<": Your transaction failed - ";
		logFile<<"password for account id "<<dataP->id<<" is incorrect"<<endl;
		break;
	case BAD_T_BALANCE:	//transfer failed - insufficient funds
		logFile<<"Error "<<dataP->atmID<<": Your transaction failed - ";
		logFile<<"account id "<<dataP->id;
		logFile<<" balance is lower than "<<dataP->amount<<endl;
		break;
	case BAD_ACCOUNT:	//no account found
		logFile<<"Error "<<dataP->atmID<<": Your transaction failed - ";
		logFile<<"account id "<<dataP->id<<" does not exist"<<endl;
		break;

	case OK_O:			//open okay
		logFile<<dataP->atmID<<": ";
		logFile<<"New account id is "<<dataP->id;
		logFile<<" with password "<<dataP->password;
		logFile<<" and initial balance "<<dataP->balance<<endl;
		break;
	case OK_D:			//deposit okay
		logFile<<dataP->atmID<<": ";
		logFile<<"Account "<<dataP->id<<" new balance is "<<dataP->balance;
		logFile<<" after "<<dataP->amount<<" $ was deposited"<<endl;
		break;
	case OK_W:			//withdraw okay
		logFile<<dataP->atmID<<": ";
		logFile<<"Account "<<dataP->id;
		logFile<<" new balance is "<<dataP->balance;
		logFile<<" after "<<dataP->amount<<" $ was withdrew"<<endl;	//this should be withdrawn, not withdrew
		break;
	case OK_B:			//balance okay
		logFile<<dataP->atmID<<": ";
		logFile<<"Account "<<dataP->id<<" balance is "<<dataP->balance<<endl;
		break;
	case OK_T:			//transfer okay
		logFile<<dataP->atmID<<": ";
		logFile<<"Transfer "<<dataP->amount;
		logFile<<" from account "<<dataP->id;	//id is account
		logFile<<" to account "<<dataP->targetID;
		logFile<<" new account balance is "<<dataP->balance;
		logFile<<" new target account balance is "<<dataP->targetBalance<<endl;
		break;
	}




	delete(dataP);
	logFile.close();
	pthread_mutex_unlock(&logMutex);
	return 0;
}
