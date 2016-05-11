#include "screenPrinter.h"
#include "account.h"
#include <vector>
#include <unistd.h>

bool allDone;







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

	///////////////////*THE FOLLOWING SECTION IS JUST FOR TESTING*/////
	Account newAccount1(1234, 1234, 0);
	accounts.insert(pair<int, Account>(1234,newAccount1));
	Account newAccount2(5678, 5678, 0);
	accounts.insert(pair<int, Account>(5678,newAccount2));
	Account newAccount3(0000, 0000, 0);
	accounts.insert(pair<int, Account>(0000,newAccount3));
	/////////////////////*END OF TESTING SECTION*///////////////////////

//actual function starts here


	//initialize usleep parameters
	unsigned int printSleep=50000;

	//repeat until Bank program is still running
	while(allDone==false){
		usleep(printSleep);
		//create vector
		vector<Account*> aVector;	//named it aVector instead of accountsVector- because it's shorter
		//insert all elements from accounts map into vector
		for(map<int, Account>::iterator it=accounts.begin(); it!=accounts.end(); it++){
			aVector.push_back(&(it->second));
		}

		//following is an example of use
		for(vector<Account*>::iterator vit=aVector.begin(); vit!=aVector.end(); vit++){
			cout<< (*vit)->number <<endl;
		}
	}


	return 0;
}
