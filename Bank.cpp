


#include <pthread.h>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "ATM.h"
#include "screenPrinter.h"
#include "commission.h"


using namespace std;

//*******************MUTEX*******************************
pthread_mutex_t logMutex;
pthread_mutex_t mapWriteLock;
pthread_mutex_t mapReadLock;
pthread_mutex_t bankBalanceWriteLock;
pthread_mutex_t bankBalanceReadLock;
//*******************************************************

extern bool allDone;
map<int, Account> accounts;
int map_read_cnt = 0;
int bankBalance = 0;
int balance_read_cnt = 0;

/*
 *Input: arg#1 is # of ATMs
 *Input: arg#2 ... #ofATMs are input file names
 */

int main(int argc, char* argv[]){
	allDone=false;
	//check insufficient parameters
	if(argc<2){
		printf("ERROR: not enough arguments\n");
		return -1;
	}
	//handle input
	//NOTE: argv[0] is prog. name; argv[1] is atm count; argv[2] is first file
	int ATMCount=atoi(argv[1]);
	if(ATMCount != (argc-2)){	//checks provided number of ATMs corresponds with number of provided input files
		printf("ERROR: illegal arguments\n");
		return -1;
	}


	//initialize mutex'es
	pthread_mutex_init(&logMutex, NULL);
	pthread_mutex_init(&mapWriteLock, NULL);
	pthread_mutex_init(&mapReadLock, NULL);
	pthread_mutex_init(&bankBalanceWriteLock, NULL);
	pthread_mutex_init(&bankBalanceReadLock, NULL);


	//create ouput file
	fstream outputFile ("log.txt", fstream::out);	//NOTE: this also creates the file. Will overwrite
	if(!outputFile){
		fputs("Error opening log.txt\n", stderr);
	}

	//create thread for screen printing
	pthread_t screenPrinterThread;
	int needed_parameter=9;
	int trErr=pthread_create(&screenPrinterThread, NULL, printToScreen, &needed_parameter);
	if(trErr!=0){
		fputs("ERROR: failed to create screenPrinter thread\n", stderr);
		return -1;
	}

	//create thread for commissions
	pthread_t commissionThread;
	int parameter=2;
	trErr=pthread_create(&commissionThread, NULL, CommissionCollect, &parameter);
	if(trErr!=0){
		fputs("ERROR: failed to create commission thread\n", stderr);
		return -1;
	}

	//create threads for each ATM
	ATMData dataForATMs[ATMCount];	//array to hold data (so as to not lose it)
	pthread_t ATMs[ATMCount];		//array to hold threads
	for(int i=0; i< ATMCount; i++){
		//update data array
		dataForATMs[i].setID(i);
		dataForATMs[i].setInputFileName(argv[2+i]);
		//create thread
		trErr=pthread_create(&ATMs[i], NULL, ATMOperator, &dataForATMs[i]);
		if(trErr!=0){
			fputs("ERROR: failed creating ATM thread\n", stderr);
			return -1;
		}
	}


	//NOTE: unless we wait- results may vary
	//wait for ATM threads to end
	for(int i=0; i< ATMCount; i++){
		pthread_join(ATMs[i], NULL);
	}
	allDone=true;
	//wait for screenPrinter to end
	pthread_join(screenPrinterThread,NULL);
	pthread_join(commissionThread,NULL);
	//destroy all mutex
	pthread_mutex_destroy(&logMutex);
	pthread_mutex_destroy(&mapWriteLock);
	pthread_mutex_destroy(&mapReadLock);
	pthread_mutex_destroy(&bankBalanceWriteLock);
	pthread_mutex_destroy(&bankBalanceReadLock);
	//close output file
	outputFile.close();

	return 0;
}

