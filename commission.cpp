#include "commission.h"
#include <stdio.h>

extern int bankBalance;
extern pthread_mutex_t bankBalanceWriteLock;
extern bool allDone;


void* CommissionCollect(void*){
	while(allDone == false){
		sleep(3);
		if (allDone == true){
			break;
		}
		srand (time(NULL));
		double com = 2 * (rand()/(double)RAND_MAX) + 2;
//		printf("the bank is taking %f commission\n",com);
		readLock();
		for(map<int, Account>::iterator it=accounts.begin(); it!=accounts.end(); it++){
			int took = (*it).second.takeCommission(com/100);
			pthread_mutex_lock(&bankBalanceWriteLock);
			bankBalance = bankBalance + took;
			pthread_mutex_unlock(&bankBalanceWriteLock);
			int id = (*it).second.number;
//			printf("Bank: commissions of %f % were charged, the bank gaind %d $ from account %d\n"\
//					, com, took, id);
			LogData* data=new LogData(-1, id, -1, took, -1, -1, -1, com, COM);
			writeToLog((void*)data);
		}
		readUnLock();
	}
	return 0;
}
