#include "logger.h"
#include <fstream>
#include <stdio.h>	//required for bullshit printing

using namespace std;
void* writeToLog(void* _data){
	LogData* _data_ptr=static_cast<LogData*>(_data);
	printf("%d %d %d %d %d %d %d %d %d\n", _data_ptr->amount, _data_ptr->atmID, _data_ptr->balance, \
				_data_ptr->id, _data_ptr->);

	pthread_mutex_lock(&logMutex);
	ofstream logFile;
	logFile.open("log.txt");

	logFile.write("abc", 3);


	delete(_data_ptr);
	logFile.close();
	pthread_mutex_unlock(&logMutex);
	return 0;
}
