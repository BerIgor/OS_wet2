#ifndef ACCOUNT_H_
#define ACCOUNT_H_
/*bla bla bla*/
#include <pthread.h>
#include <map>
#include <unistd.h>
#include <math.h> //for round function



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
		sleep(1);
	}

	~Account(){
		pthread_mutex_destroy(&writeLock);
		pthread_mutex_destroy(&readLock);
	}


	/*Deposit method
	 * Increases the balance with the deposit.
	 * param[in] account password
	 * param[in] money amount
	 * returns -1 on failure - wrong password, new balance on success*/
	int deposit(int password_, int amount){
		//READERS WRITERS 2016S
		if (password != password_){
			sleep(1);
			return -1;
		}
		pthread_mutex_lock(&writeLock);
		balance = balance + amount;
		int bal = balance;
		sleep(1);
		pthread_mutex_unlock(&writeLock);
		return bal;
	}

	/*Withdrawal method
	 * Decreases the balance with the amount.
	 * param[in] account password
	 * param[in] money amount to withdrawal (POSITIVE!)
	 * returns -1 on failure - wrong password, -2 - not enough money, new balance on success*/
	int withdrawal(int password_, int amount){
		//READERS WRITERS 2016S
		if (password != password_){
			sleep(1);
			return -1;
		}
		pthread_mutex_lock(&readLock);
		read_cnt++;
		if (read_cnt == 1){
			pthread_mutex_lock(&writeLock);
		}
		pthread_mutex_unlock(&readLock);
		int bal = balance;
		pthread_mutex_lock(&readLock);
		read_cnt--;
		if (read_cnt == 0){
			pthread_mutex_unlock(&writeLock);
		}
		pthread_mutex_unlock(&readLock);
		if (bal - amount < 0){
			sleep(1);
			return -2;
		}
		pthread_mutex_lock(&writeLock);
		balance = balance - amount;
		bal = balance;
		sleep(1);
		pthread_mutex_unlock(&writeLock);
		return bal;
	}

	/*Get balance from ATM method
	 * returns current balance.
	 * param[in] account password
	 * returns -1 on failure - wrong password, current balance on success*/
	int get_balance_atm(int password_){
		//READERS WRITERS 2016S
		if (password != password_){
			sleep(1);
			return -1;
		}
		pthread_mutex_lock(&readLock);
		read_cnt++;
		if (read_cnt == 1){
			pthread_mutex_lock(&writeLock);
		}
		pthread_mutex_unlock(&readLock);
		sleep(1);
		int bal = balance;
		pthread_mutex_lock(&readLock);
		read_cnt--;
		if (read_cnt == 0){
			pthread_mutex_unlock(&writeLock);
		}
		pthread_mutex_unlock(&readLock);
//		sleep(1);
		return bal;
	}

	/*FAST Get balance method
	 * returns current balance. */
	int get_balance(){
		//READERS WRITERS 2016S
		pthread_mutex_lock(&readLock);
		read_cnt++;
		if (read_cnt == 1){
			pthread_mutex_lock(&writeLock);
		}
		pthread_mutex_unlock(&readLock);
		int bal = balance;
		pthread_mutex_lock(&readLock);
		read_cnt--;
		if (read_cnt == 0){
			pthread_mutex_unlock(&writeLock);
		}
		pthread_mutex_unlock(&readLock);
		return bal;
	}

	/*Transfer method
	 * Moves amount from this account to transfer_to account.
	 * param[in] account password
	 * param[in] account to transfer money to
	 * param[in] money amount to transfer (POSITIVE!)
	 * param[out] 
	 * returns -1 on failure - wrong password, -2 failure - not enough money, account balance on success*/
	int transfer(int password_, Account &transfer_to, int amount, int *targetBal){
		//READERS WRITERS 2016S
		if (password != password_){
			sleep(1);
			return -1;
		}
		if (transfer_to.number == number){
			pthread_mutex_lock(&writeLock);
			int bal = balance;
			(*targetBal) = transfer_to.balance;
			pthread_mutex_unlock(&writeLock);
			sleep(1);
			return bal;
		}
		pthread_mutex_lock(&readLock);
		read_cnt++;
		if (read_cnt == 1){
			pthread_mutex_lock(&writeLock);
		}
		pthread_mutex_unlock(&readLock);
		int bal = balance;
		pthread_mutex_lock(&readLock);
		read_cnt--;
		if (read_cnt == 0){
			pthread_mutex_unlock(&writeLock);
		}
		pthread_mutex_unlock(&readLock);
		if (bal - amount < 0){
			sleep(1);
			return -2;
		}
		if (number < transfer_to.number){
			pthread_mutex_lock(&writeLock);
			pthread_mutex_lock(&transfer_to.writeLock);
		}
		else {
			pthread_mutex_lock(&transfer_to.writeLock);
			pthread_mutex_lock(&writeLock);
		}
		balance = balance - amount;
		bal = balance;
		transfer_to.balance = transfer_to.balance + amount;
		(*targetBal) = transfer_to.balance;
		sleep(1);
		if (number < transfer_to.number){
			pthread_mutex_unlock(&writeLock);
			pthread_mutex_unlock(&transfer_to.writeLock);
		}
		else {
			pthread_mutex_unlock(&transfer_to.writeLock);
			pthread_mutex_unlock(&writeLock);
		}
		return bal;
	}

	/*Commission method
	 * takes a precent of commision to the bank
	 * param[in] precent to take
	 * returns the amount taken*/
	int takeCommission(double precent){
	//READERS WRITERS 2016S
		pthread_mutex_lock(&writeLock);
		int took = (int)round(balance*precent);
		balance = balance - took;
		pthread_mutex_unlock(&writeLock);
		return took;
	}

};


extern map<int, Account> accounts;

#endif /* ACCOUNT_H_ */
