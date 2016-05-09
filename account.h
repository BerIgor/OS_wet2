#ifndef ACCOUNT_H_
#define ACCOUNT_H_

using namespace std;

class Account {
public:
	int number;
	int password;
	int balance;

	//TODO: change to include mutex
	Account(int _accountNumber, int _accountPassword, int _accountBalance) :
			number(_accountNumber), password(_accountPassword), balance(_accountBalance){}




};



#endif /* ACCOUNT_H_ */
