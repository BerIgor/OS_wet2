#ifndef ACCOUNT_H_
#define ACCOUNT_H_
/*bla bla bla*/
#include <pthread.h>
#include <map>


using namespace std;

class Account {
public:
	int number;
	int password;
	int balance;
	pthread_mutex_t writeLock;
	pthread_mutex_t readLock;
	int read_cnt;

	Account(int _accountNumber, int _accountPassword, int _accountBalance) :
			number(_accountNumber), password(_accountPassword), balance(_accountBalance){
		read_cnt=0;
		pthread_mutex_init(&writeLock, NULL);
		pthread_mutex_init(&readLock, NULL);
	}

	~Account(){
		pthread_mutex_destroy(&writeLock);
		pthread_mutex_destroy(&readLock);
	}

//	Account& transfer(Account& targetAccount){
//
//	}

};


extern map<int, Account> accounts;

#endif /* ACCOUNT_H_ */
