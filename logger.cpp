#include "logger.h"
#include <fstream>

using namespace std;
void* writeToLog(void* _data){
	LogData* _data_ptr=static_cast<LogData*>(_data);


	pthread_mutex_lock(&logMutex);
	ofstream logFile;
	logFile.open("log.txt");

	logFile.write("abc", 3);


	delete(_data_ptr);
	logFile.close();
	pthread_mutex_unlock(&logMutex);
	return 0;
}
