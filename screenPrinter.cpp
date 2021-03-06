#include "screenPrinter.h"
#include "account.h"
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include "ATM.h"
#include <iomanip>

using namespace std;

bool allDone;
extern int balance_read_cnt;
extern int bankBalance;
extern pthread_mutex_t bankBalanceWriteLock;
extern pthread_mutex_t bankBalanceReadLock;


#include <iostream>	//TODO: remove. only used for testing

/*
 *Name:	accountSorter
 *Description: Used to compare accounts by their ID
 *Parameters: Accounts to be compared
 *Return: true if id of account1 is larger than id of account2
 */
class accountSorter : public Account {
	bool operator()(Account* account1, Account* account2){
		int id1=account1->number;
		int id2=account2->number;
		return (id1>id2);
	}
};




void* printToScreen(void* nothing){
	//initialize usleep parameters
	unsigned int printSleep=500000;

	//repeat until Bank program is still running
	while(allDone==false){
		usleep(printSleep);
		//create vector
		vector<Account*> aVector;	//named it aVector instead of accountsVector- because it's shorter
		//insert all elements from accounts map into vector
		readLock();
		for(map<int, Account>::iterator it=accounts.begin(); it!=accounts.end(); it++){
			aVector.push_back(&(it->second));
		}
		readUnLock();

		//clear screen; move cursor to top left;
		printf("\033[2J");
		printf("\033[1;1H");


		cout<< "‫‪Current Bank Status‬‬" <<endl;
		int id = 0;
		int balance = 0;
		int password = 0;
		for(vector<Account*>::iterator vit=aVector.begin(); vit!=aVector.end(); vit++){
			id = (*vit)->number;
			balance = (*vit)->get_balance();
			password = (*vit)->password;
			cout<< "Account ";
			cout<< id;
			cout<< ": Balance - ";
			cout<< setfill(' ') <<setw(4) <<balance;
			cout<< " $ , Account Password - ";
			cout<< setfill('0') << setw(4) << password << endl;
		}
		pthread_mutex_lock(&bankBalanceReadLock);
		balance_read_cnt++;
		if (balance_read_cnt == 1){
			pthread_mutex_lock(&bankBalanceWriteLock);
		}
		pthread_mutex_unlock(&bankBalanceReadLock);
		int money = bankBalance;
		pthread_mutex_lock(&bankBalanceReadLock);
		balance_read_cnt--;
		if (balance_read_cnt == 0){
			pthread_mutex_unlock(&bankBalanceWriteLock);
		}
		pthread_mutex_unlock(&bankBalanceReadLock);

		cout << "The Bank has " << money << " $"<<endl;
	}


	return 0;
}
