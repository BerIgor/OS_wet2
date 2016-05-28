
#ifndef COMMISSION_H_
#define COMMISSION_H_
#include <pthread.h>
#include <map>
#include <stdlib.h>
#include <cstdlib>
#include "ATM.h"
#include "logger.h"

/*DESCRIPTION: function to take commission every 3s
 * will end when all ATMs end operations
 *PARAM: N/A
 *RETURN: N/A
 */
void* CommissionCollect(void*);

#endif /* COMMISSION_H_ */
