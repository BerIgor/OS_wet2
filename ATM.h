#ifndef ATM_H_
#define ATM_H_

#include <map>
#include "account.h"


extern std::map<int, Account> accounts;

void* ATMOperator(void* inputFileName);


#endif /* ATM_H_ */
