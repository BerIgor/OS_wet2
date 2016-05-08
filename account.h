#ifndef ACCOUNT_H_
#define ACCOUNT_H_

class Account {
public:
	int accountNumber;
	int accountPassword;
	int accountBalance;

	//TODO: change to include mutex
	Account(int _accountNumber, int _accountPassword, int _accountBalance) :
			accountNumber(_accountNumber), accountPassword(_accountPassword), accountBalance(_accountBalance){}




};



#endif /* ACCOUNT_H_ */
