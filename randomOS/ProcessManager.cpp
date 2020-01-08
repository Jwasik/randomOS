#include "ProcessManager.h"
#include "Interpreter.h"



ProcessManager::ProcessManager(std::shared_ptr <Scheduler> scheduler, std::shared_ptr <VirtualMemory> virtualMemory, std::shared_ptr <FileMenager> fileManager):
scheduler(scheduler), virtualMemory(virtualMemory), fileManager(fileManager)
{
	createInit();
	freePID = 1;
}

ProcessManager::~ProcessManager(){}

int8_t ProcessManager::createInit()
{	
	this->init = std::make_shared<PCB>("Init", 0, nullptr);
	init->setStateRunning();

	//adds the program code to init's memory
	std::string initCode = "JUM 0";
	std::vector<Page> initPages { Page((Interpreter::convertToMachine(initCode))) };
	virtualMemory->insertProgram(std::make_pair(0, initPages));
	//ads innit to scheduler
	DUMMY = this->init;

	addProcessToScheduler(this->init);
	RUNNING = this->init;

	return 0;
}

std::pair<int8_t, unsigned int> ProcessManager::fork(const std::string& processName,const unsigned int& parentPID,const std::string& filePath)
{
	//helper variable for returning errors to shell
	//0 if no errors occur, else error code
	int8_t errorHandling = 0;

	//check if the process name isn't unsutable
	///too long, too short, only contains spaces, is already taken 
	errorHandling = isThisNameSutableForAProcess(processName);
	///if the process has an unsutable name, return appropriate error code
	if (errorHandling != 0){ return  std::make_pair(errorHandling, 0); } 

	//find the PCB of the parent by parentPID
	std::shared_ptr<PCB> parentPCB = getPCBByPID(parentPID);
	///if the parent process cannot be found return error
	if (parentPCB == nullptr) { return std::make_pair(ERROR_PM_PARENT_COULD_NOT_BE_FOUND, 0); }

	//assign the PID of the created process to the PIDOfTheCreatedProcess funtion argument
	unsigned int PIDOfTheCreatedProcess = freePID;

	//load the program code to be executed by the process into its memory pages
	errorHandling = loadProgramIntoMemory(filePath, PIDOfTheCreatedProcess);
	if (errorHandling != 0) { return std::pair<int8_t, unsigned int>(errorHandling, 0); }

	//create a new process
	std::shared_ptr<PCB> newProcess = std::make_shared<PCB>(processName, this->freePID, parentPCB);
	newProcess->setStateReady();

	//add the process to scheduler
	errorHandling= addProcessToScheduler(newProcess);
	if (errorHandling != 0) { return std::pair<int8_t, unsigned int>(errorHandling, 0); }

	// if all else went well, increment the free PID field, because the current one is now taken and linkt the process to a parent
	parentPCB->addChild(newProcess);
	freePID++;
	//call scheduler to update queues
	scheduler->schedule();
	return std::make_pair(0,PIDOfTheCreatedProcess);
}


int8_t ProcessManager::deleteProcess(const unsigned int& PID)
{

	//If the process that is to be deleted is init, it cannot be done
	if (PID == 0){ return  ERROR_PM_INIT_CANNOT_BE_DELETED;}

	//try to find the process by PID
	std::shared_ptr<PCB> found = getPCBByPID(PID);
	///if the process couldn't be found
	if (found == nullptr){ return ERROR_PM_PROCESS_COULD_NOT_BE_FOUND; }
	
	//call for reccurent deletion of the process and its children
	deleteProcess(found,this->fileManager, this->scheduler,  this->virtualMemory);
	return 0;
}

int8_t ProcessManager::deleteProcess(const std::string & processName)
{

	//try to find the process by PID
	std::shared_ptr<PCB> found = getPCBByName(processName);

	///if the process couldn't be found
	if (found == nullptr) { return ERROR_PM_PROCESS_COULD_NOT_BE_FOUND; }

	//If the process that is to be deleted is init, it cannot be done
	if (found->getHasPID(0)) { return  ERROR_PM_INIT_CANNOT_BE_DELETED; }

	//call for reccurent deletion of the process and its children
	deleteProcess(found, this->fileManager, this->scheduler, this->virtualMemory);
	return 0;
}


bool ProcessManager::deleteProcess(std::shared_ptr<PCB> process, const std::shared_ptr<FileMenager>& fileManager, const std::shared_ptr<Scheduler>& scheduler, const std::shared_ptr<VirtualMemory>& virtualMemory)
{
		//check if the process has any children and call for recursive deletion of all of them
		if (process->getHasChildren())
		{
			std::shared_ptr<PCB> child = process->getChildren()[0];
			deleteProcess(child, fileManager,scheduler, virtualMemory);
			deleteProcess(process, fileManager,scheduler, virtualMemory);
		}
		//if the process doesn't have children it can simply be deleted
		else
		{
			scheduler->deleteProcess(process->getPID());
			fileManager->closeProcessFiles(process->getPID());
			process->getParentPCB()->removeChild(process);
			//not sure if needed
			//virtualMemory->removeProgram(process->getPID());
			return true;
		}
	
	return false;
}

int8_t ProcessManager::addProcessToScheduler(const std::shared_ptr<PCB>& process)
{
	return this->scheduler->addProcess(process, nullptr);
}

int8_t ProcessManager::loadProgramIntoMemory(const std::string& filePath, const unsigned int& PID)
{
	//OPEN THE FILE CONTAINING SOURCE CODE
	std::ifstream programFile(filePath);
	//if the file cannot be opened throw appropriate error
	if (!programFile.good()) { return ERROR_PM_CANNOT_OPEN_SOURCE_CODE_FILE;}

	//get the first line containing number of pages needed for the program and initialise a page vector
	std::string line="";
	std::getline(programFile, line);
	std::vector<Page> programPages;
	programPages.resize(std::stoi(line));

	std::vector<int8_t> allMachineCode;
	//LOAD THE SOURCE CODE INTO PAGE VECTOR
	//translate all source code into machine code
	while (std::getline(programFile, line))
	{
		std::vector<int8_t> machineCodeLine = Interpreter::convertToMachine(line);
		for (auto e : machineCodeLine) { allMachineCode.push_back(e); }
	}

	int currentByte = 0;
	//divide it and push into the pages
	for (int i = 0; i < programPages.size(); i++) {
		for (int j = 0; j < PAGE_SIZE && currentByte < allMachineCode.size(); j++) {
			uint8_t errorCode = programPages[i].writeToPage(j,allMachineCode[currentByte]);
			if (errorCode != 0) { return errorCode; }
			currentByte++;
		}
	}

	virtualMemory->insertProgram(std::make_pair(PID, programPages));
	return 0;
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

std::shared_ptr<PCB> ProcessManager::getPCBByName(const std::string& processName)
{
	std::stack<std::shared_ptr<PCB>> allProcesses;
	allProcesses.push(init);

	std::shared_ptr<PCB> currentProcess;
	while (!allProcesses.empty())
	{
		currentProcess = allProcesses.top();
		allProcesses.pop();

		if (currentProcess->getHasName(processName)) { return currentProcess; }

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
	std::string result{""};

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

std::vector<std::shared_ptr<PCB>> ProcessManager::getAllWithState(PCB::ProcessState state)
{
	std::vector<std::shared_ptr<PCB>> result;

	std::stack<std::shared_ptr<PCB>> allProcesses;
	allProcesses.push(init);

	std::shared_ptr<PCB> currentProcess;
	while (!allProcesses.empty())
	{
		currentProcess = allProcesses.top();
		allProcesses.pop();
		if (currentProcess->getHasState(state))
		{
			result.push_back(currentProcess);
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


int8_t ProcessManager::isThisNameSutableForAProcess(const std::string & processName)
{
	//check if the name isn't empty
	if (processName.length()==0) { return ERROR_PM_PROCESS_NAME_CANNOT_BE_EMPTY; }
	//check if isn't too long
	if (processName.length()>MAX_PROCESS_NAME_LENGHT) { return ERROR_PM_PROCESS_NAME_TOO_LONG;}
	//check if doesn't contain notAllowedcharacters
	bool containsLetterFlag = 0;
	for (auto it = processName.begin(); it != processName.end(); it++) {
		if (*it == ' ') { return ERROR_PM_PROCESS_NAME_CONTAINS_UNALLOWED_CHARACTERS; }
		if ((*it >= 65 && *it < 90) || (*it >= 97 && *it < 122)) { containsLetterFlag = 1; }
	}
	if (!containsLetterFlag) { return ERROR_PM_PROCESS_NAME_HAS_TO_CONTAIN_AT_LEAST_ONE_LETTER; }

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

int ProcessManager::getPIDbyName(const std::string & processName)
{
	std::stack<std::shared_ptr<PCB>> allProcesses;
	allProcesses.push(init);

	std::shared_ptr<PCB> currentProcess;
	while (!allProcesses.empty())
	{
		currentProcess = allProcesses.top();
		allProcesses.pop();

		if (currentProcess->getHasName(processName)) { return currentProcess->getPID(); }

		//add all of its children to the queue
		std::vector<std::shared_ptr<PCB>> childrenOfCurrent = currentProcess->getChildren();
		for (auto child : childrenOfCurrent) {
			allProcesses.push(child);
		}
	}
	return -1;
}


