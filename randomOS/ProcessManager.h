#pragma once
#include "PCB.h"


/*

	POSSIBLE ERRORS




*/


/********************************
 *  ProcessManager.             *
 *  used to manage processes.   *
 ********************************/
class ProcessManager
{
private:
	std::shared_ptr<PCB> init;
	unsigned int freePID=1;

	std::shared_ptr<PCB> findPCBByPID(const unsigned int& PID);
	std::string getIndentation(const unsigned int& ammountOfIndentation, const bool& endsInProcess, const std::vector<unsigned int>& skipsIndentionBites);
	bool checkIfProcessCanBeClosed(const std::shared_ptr<PCB>& process);
	bool deleteProcess(const std::shared_ptr<PCB>& process);
public:
	ProcessManager();
	~ProcessManager();

	void createInit();
	_int8 fork(const std::string& processName, const unsigned int& parentPID, const std::string& fileName);
	_int8 deleteProcess(const unsigned int& PID);

	std::string displayTree();

};

