#ifndef LOGGER_H_
#define LOGGER_H_


#include <pthread.h>


enum LogType {
	BAD_O,			//account open failed
	BAD_D,			//deposit failed
	BAD_W_PASSWORD,	//withdraw failed - wrong password
	BAD_W_BALANCE,	//withdraw failed - insufficient funds
	BAD_B,			//balance query failed
	BAD_T_PASSWORD, //transfer failed - wrong password
	BAD_T_BALANCE,	//transfer failed - insufficient funds

	OK_O,
	OK_D,
	OK_W,
	OK_B,
	OK_T
};

extern pthread_mutex_t logMutex;

class LogData{
private:
	int atmID;
	int id;
	int password;
	int balance;
	int amount;
	int targetID;
	int targetBalance;
	LogType type;
	//put here all the shit
public:	//TODO: finish
	LogData(int _atmID, int _id, int _password, int _balance){};
};


/*
 *Name:	writeToLog
 *Description: Used to write to log.txt. Designed to be called in parallel
 *Parameters: data contains all data that might be needed, including type of data
 *Return: N/A
 */
void* writeToLog(void* data);



#endif /* LOGGER_H_ */
