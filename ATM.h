#ifndef ATM_H_
#define ATM_H_

#include <map>
#include "account.h"
#include <string>



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

	string getInputFileName() const {
		return inputFileName;
	}

	int getID() const {
		return id;
	}

	void setInputFileName(string nameToSet){
		inputFileName=nameToSet;
	}

	void setID(int idToSet){
		id=idToSet;
	}
};


extern std::map<int, Account> accounts;

void* ATMOperator(void* inputData);


#endif /* ATM_H_ */
