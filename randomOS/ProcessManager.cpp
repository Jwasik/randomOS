#include "ProcessManager.h"



ProcessManager::ProcessManager()
{
	createInit();
}


ProcessManager::~ProcessManager()
{
}

void ProcessManager::createInit()
{
	this->init = std::make_shared<PCB>("Init", 0, nullptr);
	//adds the program code to init's memory
	init->setMemoryPages(loadProgramIntoMemory("init_Path"));
	//ads innit to scheduler
	addProcessToScheduler(this->init);
}

std::pair<int8_t, unsigned int> ProcessManager::fork(const std::string& processName,const unsigned int& parentPID,const std::string& filePath)
{
	//helper variable for returning errors to shell
	//0 if no errors occur, else error code
	_int8 errorHandling = 0;

	//check if the process name isn't unsutable
	//too long, too short, only contains spaces, is already taken 
	errorHandling = isThisNameSutableForAProcess(processName);
	if (errorHandling == 0)
	{
		//find the PCB of the parent by parentPID
		std::shared_ptr<PCB> parentPCB = getPCBByPID(parentPID);
		if (parentPCB != nullptr)
		{
			//create a new process
			std::shared_ptr<PCB> newProcess = std::make_shared<PCB>(processName, this->freePID, parentPCB);
			///assign the PID of the created process to the PIDOfTheCreatedProcess funtion argument
			unsigned int PIDOfTheCreatedProcess = freePID;
			///add the newly created process as a child of its parent
			parentPCB->addChild(newProcess);

			//load the program code to be executed by the process into its memory pages
			newProcess->setMemoryPages(loadProgramIntoMemory(filePath));
			//add the process 
			addProcessToScheduler(newProcess);

			//increment the free PID field, because the current one is now taken
			freePID++;
			return std::make_pair(errorHandling,PIDOfTheCreatedProcess);
		}
		//if the given parent cannot be found
		return std::make_pair(ERROR_PM_PARENT_COULD_NOT_BE_FOUND,0);
	}
	//if the process has an unsutable name, return appropriate error code
	return  std::make_pair(errorHandling, 0);
}


int8_t ProcessManager::deleteProcess(const unsigned int& PID)
{
	//helper variable for returning errors to shell
	//0 if no errors occur, else error code
	_int8 errorHandling = 0;

	//If the process that is to be deleted is init, it cannot be done
	if (PID == 0){ return  ERROR_PM_INIT_CANNOT_BE_DELETED;}

	//try to find the process by PID
	std::shared_ptr<PCB> found = getPCBByPID(PID);
	if (found != nullptr) 
	{
		errorHandling = checkIfProcessCanBeClosed(found);
		if(errorHandling==0)
		{
			//call for reccurent deletion of the process and its children
			deleteProcess(found);
			return 0;
		}
		return errorHandling;
	}
	//if the process couldn't be found
	return ERROR_PM_PROCESS_COULD_NOT_BE_FOUND;
}


bool ProcessManager::deleteProcess(const std::shared_ptr<PCB>& process)
{
		//check if the process has any children
		if (process->getHasChildren())
		{
			std::shared_ptr<PCB> child = process->getChildren()[0];
			if (checkIfProcessCanBeClosed(child)==0)
			{
				deleteProcess(child);
				deleteProcess(process);
			}
			else
			{
				child->setParent(init);
				init->addChild(child);
				process->removeChild(child);
				deleteProcess(process);
			}
		}
		//if the process doesn't have children it can simply be deleted
		else
		{
			//freeMemoryFromProcess(process)
			//deleteProcessFromScheduler(process)
			process->getParentPCB()->removeChild(process);
			return true;
		}
	
	return false;
}

int8_t ProcessManager::addProcessToScheduler(const std::shared_ptr<PCB>& process)
{
	return 0;
}

std::shared_ptr<std::vector<MemoryPage>> ProcessManager::loadProgramIntoMemory(const std::string& filePath)
{
	//WAITING FOR IMPLEMENTATION IN MEMORYMANAGEMENT MODULE
	return nullptr;
}



std::shared_ptr<PCB> ProcessManager::getPCBByPID(const unsigned int& PID)
{
	std::stack<std::shared_ptr<PCB>> allProcesses;
	allProcesses.push(init);

	std::shared_ptr<PCB> currentProcess;
	while (!allProcesses.empty()) 
	{
		currentProcess = allProcesses.top();
		allProcesses.pop();

		if (currentProcess->getHasPID(PID)) { return currentProcess; }

		//add all of its children to the queue
		std::vector<std::shared_ptr<PCB>> childrenOfCurrent = currentProcess->getChildren();
		for (auto child : childrenOfCurrent) {
			allProcesses.push(child);
		}
	}
	return nullptr;
}

std::string ProcessManager::displayTree()
{
	std::string result{"\n"};

	struct informationForDisplay {
		std::shared_ptr<PCB> process;
		unsigned int ammountOfIndention;
		std::vector<unsigned int> skipsIndentionBites;

		informationForDisplay() {};
		informationForDisplay(std::shared_ptr<PCB> process, unsigned int ammountOfIndention): 
		process(process), ammountOfIndention(ammountOfIndention){}
		informationForDisplay(std::shared_ptr<PCB> process, unsigned int ammountOfIndention, std::vector<unsigned int> skipsIndentionBites) :
		process(process), ammountOfIndention(ammountOfIndention), skipsIndentionBites(skipsIndentionBites) {}

		void addSkip(unsigned int skip) { skipsIndentionBites.push_back(skip); }
		std::string skipsIndentionBitesToString() {
			std::string result{" ("};
			for (auto s : skipsIndentionBites)
			{
				result += std::to_string(s)+ "" ;
			}
			return result + ")";
		}
	};

	std::stack<informationForDisplay> toBeDisplayed;
	toBeDisplayed.push(informationForDisplay(init,0));

	informationForDisplay currentlyDisplayed;
	int parentsAmmountOfIndentation = 0;
	
	while (!toBeDisplayed.empty())
	{
		currentlyDisplayed = toBeDisplayed.top();
		toBeDisplayed.pop();
		
		parentsAmmountOfIndentation = currentlyDisplayed.ammountOfIndention;
		bool isLastInCurrentIndention = false;
		if(currentlyDisplayed.process->getIsLastChild()){ isLastInCurrentIndention = true; }

		result +=  getIndentation(parentsAmmountOfIndentation, 0, currentlyDisplayed.skipsIndentionBites);
		result += getIndentation(parentsAmmountOfIndentation, 1, currentlyDisplayed.skipsIndentionBites) + currentlyDisplayed.process->getNameAndPIDString();
		
		//add all of its children to the queue
		std::vector<std::shared_ptr<PCB>> childrenOfCurrent = currentlyDisplayed.process->getChildrenInReverseOrder();
		for (auto child : childrenOfCurrent) {
			informationForDisplay temp(child, parentsAmmountOfIndentation + 1, currentlyDisplayed.skipsIndentionBites);
			if (isLastInCurrentIndention){temp.addSkip(parentsAmmountOfIndentation); }
			toBeDisplayed.push(temp);
		}
	
	}
	return result;
}

std::string ProcessManager::displayProcesses()
{
	std::string result{ "\n" };

	std::stack<std::shared_ptr<PCB>> allProcesses;
	allProcesses.push(init);

	std::shared_ptr<PCB> currentProcess;
	while (!allProcesses.empty())
	{
		currentProcess = allProcesses.top();
		allProcesses.pop();
		result += "\n-" + currentProcess->getNameAndPIDString();

		//add all of its children to the queue
		std::vector<std::shared_ptr<PCB>> childrenOfCurrent = currentProcess->getChildren();
		for (auto child : childrenOfCurrent) {
			allProcesses.push(child);
		}
	}
	return result;
}

std::string ProcessManager::displayWithState(PCB::ProcessState state)
{
	std::string result{ "\n" };

	std::stack<std::shared_ptr<PCB>> allProcesses;
	allProcesses.push(init);

	std::shared_ptr<PCB> currentProcess;
	while (!allProcesses.empty())
	{
		currentProcess = allProcesses.top();
		allProcesses.pop();
		if (currentProcess->getHasState(state))
		{
			if(state == PCB::ProcessState::RUNNING){ result += currentProcess->getInformation(); }
			else{ result += "\n-" + currentProcess->getNameAndPIDString(); }
		}

		//add all of its children to the queue
		std::vector<std::shared_ptr<PCB>> childrenOfCurrent = currentProcess->getChildren();
		for (auto child : childrenOfCurrent) {
			allProcesses.push(child);
		}
	}
	return result;
}

std::shared_ptr<PCB> ProcessManager::getInit()
{
	return this->init;
}


std::string ProcessManager::getIndentation(const unsigned int& ammountOfIndentation,const bool& endsInProcess, const std::vector<unsigned int>& skipsIndentionBites)
{
	if (ammountOfIndentation == 0) { return ""; }
	std::string result{"\n"};
	for (unsigned int i = 0; i < ammountOfIndentation; i++) {
		result += " |";
	}
	for (auto e : skipsIndentionBites) {
		result.at((e*2)) = ' ';
	}
	if(endsInProcess){ return result += "->"; }
	
	return result += "  ";
}

int8_t ProcessManager::checkIfProcessCanBeClosed(const std::shared_ptr<PCB>& process)
{
	//check for open files in file manager
	return 0;
}

int8_t ProcessManager::isThisNameSutableForAProcess(const std::string & processName)
{
	//check if the name isn't empty
	if (processName.length()==0) { return ERROR_PM_PROCESS_NAME_CANNOT_BE_EMPTY; }
	//check if isn't too long
	if (processName.length()>MAX_PROCESS_NAME_LENGHT) { return ERROR_PM_PROCESS_NAME_TOO_LONG;}
	//check if doesn't contain notAllowedcharacters
	for (auto it = processName.begin(); it != processName.end(); it++) {
		if (*it == ' ') { return ERROR_PM_PROCESS_NAME_CONTAINS_UNALLOWED_CHARACTERS; }
	}

	return isProcessNameUnique(processName);
}

int8_t ProcessManager::isProcessNameUnique(const std::string & processName)
{

	std::stack<std::shared_ptr<PCB>> allProcesses;
	allProcesses.push(init);

	std::shared_ptr<PCB> currentProcess;
	while (!allProcesses.empty())
	{
		currentProcess = allProcesses.top();
		allProcesses.pop();

		if (currentProcess->getHasName(processName)) { return ERROR_PM_PROCESS_NAME_TAKEN; }

		//add all of its children to the queue
		std::vector<std::shared_ptr<PCB>> childrenOfCurrent = currentProcess->getChildren();
		for (auto child : childrenOfCurrent) {
			allProcesses.push(child);
		}
	}
	//if the process cannot be found return 0 
	return 0;
}


