#pragma once
#include "Includes.h"
#include "PCB.h"
#include "VirtualMemory.h"
#include "Scheduler.h"
#include "RUNNING.h"

//POSSIBLE ERRORS (in range[32 - 64])
#define ERROR_PM_PROCESS_NAME_TAKEN  32 //a process with such name already exists (when trying to fork a new process)
#define ERROR_PM_PROCESS_NAME_TOO_LONG 33
#define ERROR_PM_PROCESS_NAME_CANNOT_BE_EMPTY 34
#define ERROR_PM_PROCESS_NAME_CONTAINS_UNALLOWED_CHARACTERS 35
#define ERROR_PM_PARENT_COULD_NOT_BE_FOUND 36 //no process matches the given parentPID (when trying to fork a new process)
#define ERROR_PM_INIT_CANNOT_BE_DELETED 37
#define ERROR_PM_PROCESS_COULD_NOT_BE_FOUND 38
#define ERROR_PM_THIS_PROCESS_HAS_OPEN_FILES 39
#define ERROR_PM_CANNOT_OPEN_SOURCE_CODE_FILE 40
#define ERROR_PM_CODE_DOESNT_FIT_INTO_NUMBER_OF_DECLARED_PAGES 41
#define ERROR_PM_PROCESS_NAME_HAS_TO_CONTAIN_AT_LEAST_ONE_LETTER 42


/*****************************************
 *  ProcessManager.                      *
 *  used to create and delete processes. *
 *****************************************/
class ProcessManager
{
public:
	ProcessManager(std::shared_ptr <Scheduler> scheduler,std::shared_ptr <VirtualMemory> virtualMemory);
	~ProcessManager();

	/********************************
	*           MAIN METHODS        *
	********************************/
	int8_t createInit();
	std::pair<int8_t, unsigned int> fork(const std::string& processName, const unsigned int& parentPID, const std::string& filePath);
	int8_t deleteProcess(const unsigned int& PID);
	int8_t deleteProcess(const std::string& processName);

	
	/********************************
	*      PRINTING TO CONSOLE      *
	********************************/
	std::string displayTree();
	/**
	* Returns all processes as a plain list (no tree formatting)
	*/
	std::string displayProcesses();
	std::string displayWithState(PCB::ProcessState state);

	/********************************
	*            GETTERS            *
	********************************/
	std::shared_ptr<PCB> getPCBByPID(const unsigned int& PID);
	std::shared_ptr<PCB> getPCBByName(const std::string& processName);
	std::shared_ptr<PCB> getInit();

	//used in scheduler
	static bool deleteProcess(const std::shared_ptr<PCB>& process);

private:
	//pointers to other modules
	std::shared_ptr <Scheduler> scheduler = NULL;
	std::shared_ptr <VirtualMemory> virtualMemory = NULL;

	std::shared_ptr<PCB> init;
	unsigned int freePID;


	int8_t isThisNameSutableForAProcess(const std::string& processName);
	int8_t isProcessNameUnique(const std::string& processName);
	std::string getIndentation(const unsigned int& ammountOfIndentation, const bool& endsInProcess, const std::vector<unsigned int>& skipsIndentionBites);
	


	int8_t addProcessToScheduler(const std::shared_ptr<PCB>& process);
	int8_t loadProgramIntoMemory(const std::string& filePath, const unsigned int& PID);
	int getPIDbyName(const std::string& processName);
};

