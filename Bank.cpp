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


std::map<int, Account> accounts;


/*
 *Input: arg#1 is # of ATMs
 *Input: arg#2 ... #ofATMs are input file names
 */
using namespace std;
int main(int argc, char* argv[]){


	//create ouput file
	fstream outputFile ("log.txt", fstream::out);	//NOTE: this also creates the file. Will overwrite


	//handle input
	//NOTE: argv[0] is prog. name; argv[1] is atm count; argv[2] is first file
	int ATMCount=atoi(argv[1]);
	if(ATMCount != (argc-2)){	//checks provided number of ATMs corresponds with number of provided input files
		printf("ERROR\n");	//TODO: handle error
		return -1;
	}

	//create threads for each ATM
	pthread_t ATMs[ATMCount];
	int trErr;
	for(int i=0; i< ATMCount; i++){
		printf("it %d\n", i);
		trErr=pthread_create(&ATMs[i], NULL, ATMOperator, argv[2+i]);
		if(trErr!=0){
			printf("ERROR\n");	//TODO: handle error
			return -1;
		}
	}


	//NOTE: unless we wait- results may vary
	//wait for ATM threads to end
	for(int i=0; i< ATMCount; i++){
		printf("waiting for thread %d\n", i);
		pthread_join(ATMs[i], NULL);
	}


	//close output file
	outputFile.close();

	return 0;
}

