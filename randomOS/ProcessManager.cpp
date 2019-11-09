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
	//addProcessToScheduler(newProcess);
}

_int8 ProcessManager::fork(const std::string& processName,const unsigned int& parentPID,const std::string& fileName)
{
	std::shared_ptr<PCB> parentPCB = findPCBByPID(parentPID);
	if (parentPCB != nullptr)
	{
		std::shared_ptr<PCB> newProcess = std::make_shared<PCB>(processName, this->freePID, parentPCB);
		parentPCB->addChild(newProcess);

		//WAITING FOR OTHER MODULES
		//void loadProgramIntoMemory(string fileName);
		//addProcessToScheduler(newProcess);
		freePID++;
		return true;
	}
	//if the given parent cannot be found
	return false;
}

_int8 ProcessManager::deleteProcess(const unsigned int& PID)
{
	//If the process that is to be deleted is init, it cannot be done
	if (PID == 0)
	{
		return false; 
	}
	std::shared_ptr<PCB> found = findPCBByPID(PID);
	if (found != nullptr) 
	{
		deleteProcess(found);
	}
	//if the process couldn't be found
	return false;
}


bool ProcessManager::deleteProcess(const std::shared_ptr<PCB>& process)
{
	if (checkIfProcessCanBeClosed(process))
	{
		//check if the process has any children
		if (process->getHasChildren())
		{
			std::shared_ptr<PCB> child = process->getChildren()[0];
			if (checkIfProcessCanBeClosed(child))
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
		else
		{
			//freeMemoryFromProcess(process)
			//deleteProcessFromScheduler(process)
			process->getParentPCB()->removeChild(process);
			return true;
		}
	}
	return false;
}

std::shared_ptr<PCB> ProcessManager::findPCBByPID(const unsigned int& PID)
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
	std::string result{};

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


std::string ProcessManager::getIndentation(const unsigned int& ammountOfIndentation,const bool& endsInProcess, const std::vector<unsigned int>& skipsIndentionBites)
{
	if (ammountOfIndentation == 0) { return ""; }
	std::string result{"\n"};
	for (int i = 0; i < ammountOfIndentation; i++) {
		result += " |";
	}
	for (auto e : skipsIndentionBites) {
		result.at((e*2)) = ' ';
	}
	if(endsInProcess){ return result += "->"; }
	
	return result += "  ";
}

bool ProcessManager::checkIfProcessCanBeClosed(const std::shared_ptr<PCB>& process)
{
	//check for open files in file manager
	return true;
}



