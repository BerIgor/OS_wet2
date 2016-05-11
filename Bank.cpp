/*
 * Hi there! Do you have beautiful long hair? Are you using Eclipse? Is your name Olga? Then the following message is for you
 * You need to include the pthread library into Eclipse for shit to work:
 * Project -> properties -> C/C++ Build -> Settings -> Cross G++ Linker -> add -lpthread (you only type in 'pthread')
 */


#include <pthread.h>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "ATM.h"
#include "screenPrinter.h"


using namespace std;

//*******************MUTEX*******************************
pthread_mutex_t logMutex;
//*******************************************************

extern bool allDone;
map<int, Account> accounts;

/*
 *Input: arg#1 is # of ATMs
 *Input: arg#2 ... #ofATMs are input file names
 */

int main(int argc, char* argv[]){
	allDone=false;
	//check insufficient parameters
	if(argc<2){
		printf("ERROR\n");	//TODO: handle error
		return -1;
	}
	//handle input
	//NOTE: argv[0] is prog. name; argv[1] is atm count; argv[2] is first file
	int ATMCount=atoi(argv[1]);
	if(ATMCount != (argc-2)){	//checks provided number of ATMs corresponds with number of provided input files
		printf("ERROR\n");	//TODO: handle error
		return -1;
	}


	//initialize mutex'es
	pthread_mutex_init(&logMutex, NULL);


	//create ouput file
	fstream outputFile ("log.txt", fstream::out);	//NOTE: this also creates the file. Will overwrite

	//create thread for screen printing
	pthread_t screenPrinterThread;
	int bullshit_parameter=9;
	int trErr=pthread_create(&screenPrinterThread, NULL, printToScreen, &bullshit_parameter);
	if(trErr!=0){
		printf("ERROR\n");	//TODO: handle error
		return -1;
	}
	//create threads for each ATM
	ATMData dataForATMs[ATMCount];	//array to hold data (so as to not lose it)
	pthread_t ATMs[ATMCount];		//array to hold threads
//	int trErr;
	for(int i=0; i< ATMCount; i++){
		//update data array
		dataForATMs[i].setID(i);
		dataForATMs[i].setInputFileName(argv[2+i]);
		//create thread
		trErr=pthread_create(&ATMs[i], NULL, ATMOperator, &dataForATMs[i]);
		if(trErr!=0){
			printf("ERROR\n");	//TODO: handle error
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
	//close output file
	outputFile.close();

	return 0;
}

