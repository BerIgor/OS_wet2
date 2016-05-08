#include "ATM.h"
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>	//required for exit()
#include <iostream> //required for std::cout

using namespace std;

void* ATMOperator(void* inputFileName){
	printf("the name i got is %s\n", (char*)inputFileName);
	fstream fs;
	try{
	fs.open((char*)inputFileName, fstream::out);
	}
	catch (ios_base::failure &e){
		cout << e.what() << endl;	//TODO: see about possible errors (opening the same file- must be set in thread attributes)
		exit(-1);
	}



	pthread_exit(NULL);
}
