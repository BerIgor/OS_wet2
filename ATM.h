#ifndef ATM_H_
#define ATM_H_


#include "account.h"
#include <string>

void readLock();
void writeLock();
void readUnLock();
void writeUnLock();

class ATMData {
private:
	string inputFileName;
	int id;

public:
	//c'tor
	ATMData(int _id, string _inputFileName) : inputFileName(_inputFileName), id(_id) {}

	//default c'tor
	ATMData(void){
		inputFileName="";
		id=-1;
	}

	/*DSCRIPTION: method will return the inputFileName*/
	string getInputFileName() const {
		return inputFileName;
	}

	/*DSCRIPTION: method will return the ATM id*/
	int getID() const {
		return id;
	}

	/*DSCRIPTION: method will set the input file name*/
	void setInputFileName(string nameToSet){
		inputFileName=nameToSet;
	}

	/*DSCRIPTION: method will set the ATM id*/
	void setID(int idToSet){
		id=idToSet;
	}
};



/*DESCRIPTION: Function to handle all ATM operation
 *PARAM: inputData of ATMData type will contain all relevant data to ATMs' operation
 */
void* ATMOperator(void* inputData);


#endif /* ATM_H_ */
