#ifndef LOGGER_H_
#define LOGGER_H_


#include <pthread.h>



extern pthread_mutex_t logMutex;

enum LogType {
	BAD_O,			//account open failed
	BAD_D,			//deposit failed
	BAD_W_PASSWORD,	//withdraw failed - wrong password
	BAD_W_BALANCE,	//withdraw failed - insufficient funds
	BAD_B,			//balance query failed
	BAD_T_PASSWORD, //transfer failed - wrong password
	BAD_T_BALANCE,	//transfer failed - insufficient funds
	BAD_ACCOUNT,

	OK_O,			//open okay
	OK_D,			//deposit okay
	OK_W,			//withdraw okay
	OK_B,			//balance okay
	OK_T,			//transfer okay
	COM				//took commision
};

class LogData{
public:
	int atmID;
	int id;
	int password;
	int balance;
	int amount;
	int targetID;
	int targetBalance;
	double comPrecent;
	LogType type;

	LogData(int _atmID, int _id, int _password, int _balance, \
				int _amount, int _targetID, int _targetBalance, double _comPrecent, LogType _type){
		atmID=_atmID+1;
		id=_id;
		password=_password;
		balance=_balance;
		amount=_amount;
		targetID=_targetID;
		targetBalance=_targetBalance;
		comPrecent = _comPrecent;
		type=_type;
	};
};


/*
 *Name:	writeToLog
 *Description: Used to write to log.txt
 *Parameters: data contains all data that might be needed, including type of data
 *Return: N/A
 */
void* writeToLog(void* data);



#endif /* LOGGER_H_ */
