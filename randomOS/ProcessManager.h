#pragma once
#include "Includes.h"
#include "PCB.h"



/********************************
*        POSSIBLE ERRORS        *
*      (in range[32 - 64])      *
********************************/

#define ERROR_PM_PROCESS_NAME_TAKEN  32 //a process with such name already exists (when trying to fork a new process)
#define ERROR_PM_PROCESS_NAME_TOO_LONG 33
#define ERROR_PM_PROCESS_NAME_CANNOT_BE_EMPTY 34
#define ERROR_PM_PROCESS_NAME_CONTAINS_UNALLOWED_CHARACTERS 35 //for now those are \s or \.
#define ERROR_PM_PARENT_COULD_NOT_BE_FOUND 36 //no process matches the given parentPID (when trying to fork a new process)
#define ERROR_PM_INIT_CANNOT_BE_DELETED 37
#define ERROR_PM_PROCESS_COULD_NOT_BE_FOUND 38


/*****************************************
 *  ProcessManager.                      *
 *  used to create and delete processes. *
 *****************************************/
class ProcessManager
{
public:
	ProcessManager();
	~ProcessManager();

	/********************************
	*           MAIN METHODS        *
	********************************/

	/**
	* Creates the default process.
	* @return Errorcode explaining why it could not be created, 0 if all goes well.
	*/
	int8_t createInit();

	/**
	* Creates a new process.
	* @return a pair of ErrorCode (.first) and the PID of the newly created process (.second).
	*/
	std::pair<int8_t, unsigned int> fork(const std::string& processName, const unsigned int& parentPID, const std::string& filePath);

	/**
	* Deletes a process selected by PID.
	* @return Errorcode explaining why it could not be deleted, 0 if all goes well.
	*/
	int8_t deleteProcess(const unsigned int& PID);

	
	/********************************
	*      PRINTING TO CONSOLE      *
	********************************/
	std::string displayTree();
	std::string displayWithState(const PCB::ProcessState& state);
	std::pair<int8_t,std::string> displayDetailsOfAProcess(const unsigned int& PID);
	std::pair<int8_t, std::string> displayDetailsOfAProcess(const std::string& name);

	/********************************
	*            GETTERS            *
	********************************/
	std::shared_ptr<PCB> getPCBByPID(const unsigned int& PID);
	std::shared_ptr<PCB> getPCBByName(const std::string& name);
	std::shared_ptr<PCB> getInit();


private:
	std::shared_ptr<PCB> init;
	unsigned int freePID = 1;

	int8_t isThisNameSutableForAProcess(const std::string& processName);
	int8_t isProcessNameUnique(const std::string& processName);
	bool deleteProcess(const std::shared_ptr<PCB>& process);
	std::string getIndentation(const unsigned int& ammountOfIndentation, const bool& endsInProcess, const std::vector<unsigned int>& skipsIndentionBites);
	
	int8_t addProcessToScheduler(const std::shared_ptr<PCB>& process);
	std::shared_ptr<std::vector<MemoryPage>> loadProgramIntoMemory(const std::string& filePath);

	struct informationForDisplay;
};

