#include "ATM.h"
#include <stdio.h>
#include <string>

void* ATMOperator(void* inputFileName){
	printf("the name i got is %s\n", (char*)inputFileName);
	pthread_exit(NULL);
}
