#include "ProcessManager.h"



ProcessManager::ProcessManager(std::shared_ptr <Scheduler> scheduler, std::shared_ptr <VirtualMemory> virtualMemory):
scheduler(scheduler), virtualMemory(virtualMemory)
{
	createInit();
}

ProcessManager::~ProcessManager(){}

int8_t ProcessManager::createInit()
{
	this->init = std::make_shared<PCB>("Init", 0, nullptr);
	init->setStateRunning();


	//adds the program code to init's memory
	std::string initCode = "JUM 0";
	std::vector<Page> initPages { Page((convertToMachine(initCode))) };
	virtualMemory->insertProgram(std::make_pair(0, initPages));
	//ads innit to scheduler
	DUMMY = this->init;

	
	RUNNING = this->init;
	addProcessToScheduler(this->init);
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

	//add the process to scheduler
	errorHandling= addProcessToScheduler(newProcess);
	if (errorHandling != 0) { return std::pair<int8_t, unsigned int>(errorHandling, 0); }

	// if all else went well, increment the free PID field, because the current one is now taken and linkt the process to a parent
	newProcess->setStateReady();
	parentPCB->addChild(newProcess);
	freePID++;
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
	deleteProcess(found);
	return 0;
}

int8_t ProcessManager::deleteProcess(const std::string & processName)
{



	//try to find the process by PID
	std::shared_ptr<PCB> found = getPCBByName(processName);

	//If the process that is to be deleted is init, it cannot be done
	if (found->getHasPID(0)) { return  ERROR_PM_INIT_CANNOT_BE_DELETED; }

	///if the process couldn't be found
	if (found == nullptr) { return ERROR_PM_PROCESS_COULD_NOT_BE_FOUND; }

	//call for reccurent deletion of the process and its children
	deleteProcess(found);
	return 0;
}


bool ProcessManager::deleteProcess(const std::shared_ptr<PCB>& process)
{
		//check if the process has any children
		if (process->getHasChildren())
		{
			std::shared_ptr<PCB> child = process->getChildren()[0];
			deleteProcess(child);
			deleteProcess(process);
		}
		//if the process doesn't have children it can simply be deleted
		else
		{
			//freeMemoryFromProcess(process)
			//deleteProcessFromScheduler(process)
			//freeUpAnySemaphores(process)
			process->getParentPCB()->removeChild(process);
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


	//LOAD THE SOURCE CODE INTO PAGE VECTOR

	bool fileHasEnded = 0;//flag variable to keep track of whether the file has been read yet
	std::queue<uint8_t> overflownBytes; // helper stack to keep the bytes that won't fit into page that is currently being filled


	//iterate over the pages reserved for the program until they are filled or the source file ends
	for (int i = 0; i < programPages.size() && !fileHasEnded; i++) {
		int byteCounter = 0; //used to keep track of how many bytes have been loaded into the current page
		bool workOnThisPage = 1; //flag variable to know wheter one should continue loading into the current page

		//if there is anything "leftover" from the last page load it into current before reading from file
		while(!overflownBytes.empty())
		{
			int8_t errorCode = programPages[i].writeToPage(byteCounter, overflownBytes.front());
			if (errorCode != 0) { return errorCode; }
			overflownBytes.pop();
			byteCounter++;
		}

		//read the file line by line, translate to machineCode and push back to current page
		while(workOnThisPage)
		{
			//READING & CONVERTING THE LINE FROM SOURCE FILE
			//try to read a line from the file, if it cannot be read (reached end of file) break out of both loops
			if (!std::getline(programFile, line)) 
			{ 
				fileHasEnded = 1; 
				break;
			}
			//convert the read line into machine code
			std::vector<uint8_t> machineCodeLine = convertToMachine(line);


			//CHECKING IF BYTES WILL FIT IN CURRENT PAGE
			int checkForOverFlow = byteCounter + machineCodeLine.size();
			int overflowingBytes = checkForOverFlow- PAGE_SIZE;
			//if the number of bytes that is to be written to the current page exceedes page size 
			//save the overflowing bytes in the overflowBytes queue
			if(overflowingBytes>0)
			{
				while (overflowingBytes!= 0)
				{
					//the overflowing bytes are the last bytes in the machineCode array
					overflownBytes.push(machineCodeLine[machineCodeLine.size() -1- overflowingBytes]);
					overflowingBytes--;
				}
			}

			//WRITING THE BYTES INTO CURRENT PAGE (ommiting the overflowing bytes by checking against page_size)
			for (int j = 0; j<machineCodeLine.size() && byteCounter<PAGE_SIZE; j++)
			{
				uint8_t errorCode= programPages[i].writeToPage(byteCounter,machineCodeLine[j]);
				if (errorCode != 0) { return errorCode;}
				byteCounter++;
			}
			//if the page has been fully filled
			if (overflowingBytes == 0) { workOnThisPage = 0; }
		}
	}

	//if there is still come code left in the file throw an error
	if (std::getline(programFile, line)){return ERROR_PM_CODE_DOESNT_FIT_INTO_NUMBER_OF_DECLARED_PAGES;}

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

std::string ProcessManager::displayWithState(PCB::ProcessState state)
{
	std::string result{ "" };

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

//code copied from Interpreter as to avoid a circular include (interpreter needs processManager mathods)
std::vector<uint8_t> ProcessManager::convertToMachine(std::string m) {
	std::vector<uint8_t> machine;
	std::vector<std::string> arg;

	std::string code = m.substr(0, 3);

	if (m.length() > 3) {

		for (int i = 3; i < m.length(); i++) {
			if (m[i] >= 48 && m[i] <= 57) {
				arg.push_back("");
				for (int j = i; m[j] != ' ' && j<m.size(); j++) {
					arg.back() += m[j];
				}
				i += arg.back().length();
			}
			else if (m[i] == '[') {
				arg.push_back("");
				for (int j = i + 1; m[j] != ']'; j++) {
					arg.back() += m[j];
				}
				i += arg.back().length() + 1;
			}
			else if (m[i] >= 65 && m[i] <= 68) {
				arg.push_back("");
				arg.back() += m[i];
				arg.back() += m[i + 1];
				i += 2;
			}
			else if (m[i] == '"') {
				arg.push_back("");
				arg.back() += m[i + 1];
				arg.back() += m[i + 2];
				i += 3;
			}

		}
	}

	if (code == "RET") machine.push_back(0x00);
	if (code == "MOV") machine.push_back(0x01);
	if (code == "WRI") machine.push_back(0x02);
	if (code == "ADD") machine.push_back(0x03);
	if (code == "SUB") machine.push_back(0x04);
	if (code == "MUL") machine.push_back(0x05);
	if (code == "DIV") machine.push_back(0x06);
	if (code == "MOD") machine.push_back(0x07);
	if (code == "INC") machine.push_back(0x08);
	if (code == "DEC") machine.push_back(0x09);
	if (code == "JUM") machine.push_back(0x0A);
	if (code == "JUA") machine.push_back(0x0B);
	if (code == "JIF") machine.push_back(0x0C);
	if (code == "JIA") machine.push_back(0x0D);
	if (code == "CFI") machine.push_back(0x0E);
	if (code == "DFI") machine.push_back(0x0F);
	if (code == "OFI") machine.push_back(0x10);
	if (code == "SFI") machine.push_back(0x11);
	if (code == "EFI") machine.push_back(0x12);
	if (code == "WFI") machine.push_back(0x13);
	if (code == "PFI") machine.push_back(0x14);
	if (code == "RFI") machine.push_back(0x15);
	if (code == "AFI") machine.push_back(0x16);
	if (code == "CPR") machine.push_back(0x17);
	if (code == "NOP") machine.push_back(0xFF);

	if (arg.size() > 0) {
		for (int i = 0; i < arg.size(); i++) {
			if (arg[i] == "AX") machine.push_back(0xFF);
			else if (arg[i] == "BX") machine.push_back(0xFE);
			else if (arg[i] == "CX") machine.push_back(0xFD);
			else if (arg[i] == "DX") machine.push_back(0xFC);
			else if (arg[i][0] >= 65 && arg[i][0] <= 90) {
				machine.push_back(arg[i][0]);
				machine.push_back(arg[i][1]);
			}
			else machine.push_back(std::stoi(arg[i]));
		}
	}
	return machine;
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


