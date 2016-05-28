
#ifndef COMMISSION_H_
#define COMMISSION_H_
#include <pthread.h>
#include <map>
#include <stdlib.h>
#include <cstdlib>
#include "ATM.h"
#include "logger.h"


void* CommissionCollect(void*);

#endif /* COMMISSION_H_ */
