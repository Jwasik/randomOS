#include "PCB.h"

PCB::PCB()
{
}

PCB::~PCB()
{
}

PCB::PCB(std::string name, unsigned int PID, std::shared_ptr<PCB> parent):
	name(name), PID(PID),parent(parent)
{
	this->setStateReady();
}

PCB::ProcessState PCB::getStateAsEnum(){ return this->state;}

unsigned int PCB::getStateAsInt() { return (unsigned  int) this->state; }

bool PCB::getIsNew()
{ 
	if (this->state == ProcessState::NEW) { return true;}
	return false;
}
bool PCB::getIsReady()
{
	if (this->state == ProcessState::READY) { return true; }
	return false;
}
bool PCB::getIsRunning()
{
	if (this->state == ProcessState::RUNNING) { return true; }
	return false;
}
bool PCB::getIsWaiting()
{
	if (this->state == ProcessState::WAITING) { return true; }
	return false;
}
bool PCB::getIsTerminated()
{
	if (this->state == ProcessState::TERMINATED) { return true; }
	return false;
}

unsigned int PCB::getPID() { return this->PID; }

bool PCB::getHasPID(const unsigned int & PIDtoCompare)
{
	if (this->PID==PIDtoCompare) {return true; }
	return false;
}

std::string PCB::getName(){ return this->name;}

bool PCB::getHasName(const std::string& nameToCompare)
{
	if (this->name._Equal(nameToCompare)) { return true; }
	return false;	
}

std::string PCB::getNameAndPIDString()
{
	return this->name +" [PID: "+std::to_string(this->PID)+"]";
}

unsigned int PCB::getTimeSpentWaiting(){ return this->timeSpentWaiting;}

std::shared_ptr<PCB> PCB::getParentPCB(){
	return this->parent;
}

unsigned int PCB::getPartentPID(){
	if (this->parent != nullptr)
	{
		return this->parent->getPID();
	}
	return -1;
}

bool PCB::getIsParentOf(unsigned int PID)
{
	for (auto e : this->children) {
		if (e->getPID() == PID) { return true; }
	}
	return false;
}

std::vector<std::shared_ptr<PCB>> PCB::getChildren()
{
	return this->children;
}

std::vector<std::shared_ptr<PCB>> PCB::getChildrenInReverseOrder()
{
	std::vector<std::shared_ptr<PCB>> temp = getChildren();
	auto first = temp.begin();
	auto last = temp.end();
	while ((first != last) && (first != --last)) {
		std::iter_swap(first++, last);
	}

	return temp;
}

bool PCB::getIsChildOf(unsigned int PID)
{
	if (this->parent->getPID()==PID) { return true; }
	return false;
}

bool PCB::getHasChildren()
{
	if (this->children.size() > 0) { return true; }
	return false;
}

unsigned int PCB::getLastChildPID()
{
	if (children.size() > 0)
	{
		return children[children.size()-1]->PID;
	}
}

bool PCB::getIsLastChild()
{
	if (this->parent != nullptr) {
		if (this->parent->getLastChildPID() == this->PID)
		{
			return true;
		}
	}
		
	return false;
}

unsigned int PCB::getInstructionCounter(){ return this->instructionCounter;}
std::array<int, 4> PCB::getRegisters(){ return this->registers;}

int PCB::getRegisterA(){ return this->registers[0];}

int PCB::getRegisterB()
{
	return this->registers[1];
}

int PCB::getRegisterC()
{
	return this->registers[2];
}

int PCB::getRegisterD()
{
	return this->registers[3];
}


std::string PCB::getInformation()
{
	return
		"\nName: " + this->name +
		"\nPID: " + std::to_string(this->PID) +
		"\nState: " + getStateAsString() +
		"\nParent: " + getParentAsString() +
		"\nChildren: " + getChildrenAsString()+
		"\nIC: " + std::to_string(this->instructionCounter)+
		"\nRegisters " +"["+std::to_string(getRegisterA())+"]"+"[" + std::to_string(getRegisterB()) + "]" + "[" + std::to_string(getRegisterC()) + "]" + "[" + std::to_string(getRegisterD()) + "]"
		;
}

bool PCB::setState(const unsigned int& stateToSet)
{ 
	if (stateToSet<NUMBER_OF_PROCESS_STATES) {
		this->state = (ProcessState)stateToSet;
		return true;
	}
	return false;
}
bool PCB::setState(const ProcessState& stateToSet){ this->state = stateToSet; return true;}

bool PCB::setStateReady() { this->state = ProcessState::READY; return true; }
bool PCB::setStateRunning() { this->state = ProcessState::RUNNING; return true;}
bool PCB::setStateWaiting() { this->state = ProcessState::WAITING; return true;}
bool PCB::setStateTerminated() { this->state = ProcessState::TERMINATED; return true;}

bool PCB::setPID(const unsigned int& PIDtoSet)
{
	this->PID = PIDtoSet;
	return true;
}

bool PCB::setName(const std::string& nameToSet)
{
	if (nameToSet.length() < MAX_PROCESS_NAME_LENGHT)
	{
		this->name = nameToSet;
		return true;
	}
	return false;
}

bool PCB::setTimeSpentWaiting(const unsigned int& timeToSet)
{
	this->timeSpentWaiting = timeToSet;
	return true;
}

bool PCB::incrementTimeSpentWaiting(const unsigned int& timeToBeIncrementedBy)
{
	if (checkWontOverflowUnsignedInt(timeSpentWaiting, timeToBeIncrementedBy)) {
		return false;
	}
	this->timeSpentWaiting += timeToBeIncrementedBy;
	return true;
}

bool PCB::setParent(const std::shared_ptr<PCB>& parent)
{
	this->parent = parent;
	return true;
}

bool PCB::addChild(const std::shared_ptr<PCB>& child)
{
	this->children.push_back(child);
	return true;
}

bool PCB::addChildren(const std::vector<std::shared_ptr<PCB>>& chlidren)
{
	for (auto child : children) {
		this->children.push_back(child);
	}
	return true;
}

bool PCB::removeChild(const std::shared_ptr<PCB>& child)
{
	for (int i=0;i<children.size();i++)
	{
		if (child->getPID() == children[i]->getPID())
		{
			children.erase(children.begin() + i);
			return true;
		}
	}
	return false;
}

bool PCB::setChildren(const std::vector<std::shared_ptr<PCB>>& chlidren)
{
	this->children = children;
	return true;
}

bool PCB::setInstructionCounter(const unsigned int& instructionCounter)
{
	this->instructionCounter = instructionCounter;
	return true;
}

bool PCB::incrementInstructionCounter()
{
	this->instructionCounter += 1;
	return true;
}

bool PCB::incrementInstructionCounter(const unsigned int& valueToBeIncrementedBy)
{
	if (checkWontOverflowUnsignedInt(this->instructionCounter, valueToBeIncrementedBy)) {
		this->instructionCounter += valueToBeIncrementedBy;
		return true;
	}
	return false;
}

bool PCB::setRegisters(const std::vector<unsigned int>& registers)
{
	if (registers.size() > registers.size()) { return false; }

	for (int i = 0; i < registers.size();i++) {
		this->registers[i] = registers[i];
	}
	return true;
}

bool PCB::setRegisters(const std::array<int, 4>& registers)
{
	if (registers.size() > registers.size()) { return false; }

	for (int i = 0; i < registers.size(); i++) {
		this->registers[i] = registers[i];
	}
	return true;
}

bool PCB::setRegisters(const unsigned int& A, const unsigned int& B, const unsigned int& C, const unsigned int& D)
{
	this->registers[0] = A;
	this->registers[1] = B;
	this->registers[2] = C;
	this->registers[3] = D;
	return true;
}

bool PCB::setRegisterA(const unsigned int& A)
{
	this->registers[0] = A;
	return true;
}

bool PCB::setRegisterB(const unsigned int& B)
{
	this->registers[1] = B;
	return true;
}

bool PCB::setRegisterC(const unsigned int& C)
{
	this->registers[2] = C;
	return true;
}

bool PCB::setRegisterD(const unsigned int& D)
{
	this->registers[3] = D;
	return true;
}

bool PCB::resetRegisters()
{
	for (int i = 0; i < registers.size();i++) {
		registers[i] = 0;
	}
	return true;
}

bool PCB::setMemoryPages(const std::shared_ptr<std::vector<MemoryPage>>& memory)
{
	this->memory = memory;
	return true;
}

bool PCB::checkWontOverflowUnsignedInt(unsigned int currentValue, unsigned int valueToBeAdded)
{
	if (currentValue > UINT_MAX - valueToBeAdded) { return false; }
	return true;
}

bool PCB::checkWontOverflowInt(int currentValue, int valueToBeAdded)
{
	if (currentValue > INT_MAX - valueToBeAdded) { return false; }
	return true;
}

std::string PCB::getStateAsString()
{
	std::vector<std::string> states{ "NEW","READY","RUNNING","WAITING","TERMINATED" };
	return states[(int) this->state];
}

std::string PCB::getChildrenAsString()
{
	if (children.size() < 1) { return "----"; }
	std::string result{};
	for (auto child : children) {
		result += "\n  "+child->getName() + "  (" + std::to_string(child->PID) + ")";
	}

	return result;
}

std::string PCB::getParentAsString()
{
	std::string result{"---"};

	if (this->parent != nullptr)
	{
		result = this->parent->getName() + "  (" + std::to_string(this->parent->getPID())+")";
	}
	return result;
}
