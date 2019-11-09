#pragma once
#include <memory.h>
#include <string>
#include <vector>
#include <array>

class MemoryPage;

/********************************
 *  Process Control Block.      *
 *  used to manage a process.*
 ********************************/
class PCB
{
public:
	PCB();
	~PCB();

	/**
	* Proces States.
	* Holds information about the state of the process
	* 0- NEW |1- READY | 2-RUNNING | 3-WAITING | 4-TERMINATED
	* can be used outside the class by calling PCB::ProcessState::<STATENAME> ex. PCB::ProcessState::READY.
	*/
	enum class ProcessState {
		NEW, /**< the process has just been created, isnt't ready to run yet. */
		READY, /**< the process is ready to be running on the processor. */
		RUNNING, /**< the process is currently running on the processor. */
		WAITING,  /**< the process is waiting for an event. */
		TERMINATED /**< the process has finished its runtime. */
	};

	//GETTERS
	//state
	unsigned int getStateAsInt();
	ProcessState getStateAsEnum();
	bool getIsNew();
	bool getIsReady();
	bool getIsRunning();
	bool getIsWaiting();
	bool getIsTerminated();
	//PID
	unsigned int getPID();
	bool getHasPID(const unsigned int& PIDtoCompare);
	//name
	std::string getName();
	bool getHasName(const std::string& nameToCompare);
	//time spent waiting 
	unsigned int getTimeSpentWaiting();
	//parent
	std::shared_ptr<PCB> getParentPCB();
	unsigned int getPartentPID();
	bool getIsParentOf(unsigned int PID);

	//children
	std::vector<std::shared_ptr<PCB>> getChildren();
	bool getIsChildOf(unsigned int PID);
	//instructionCounter
	unsigned int getInstructionCounter();
	//registers
	std::array<int, 4> getRegisters();
	int getRegisterA();
	int getRegisterB();
	int getRegisterC();
	int getRegisterD();
	//memory

	//console
	/**
	 * Get data about the process.
	 * basically a to string method, for use in step-mode.
	 */
	std::string getInformation();

	//MODIFIERS
	//state
	bool setState(unsigned int stateToSet);
	bool setState(ProcessState stateToSet);
	bool setStateReady();
	bool setStateRunning();
	bool setStateWaiting();
	bool setStateTerminated();
	//PID
	bool setPID(unsigned int PIdtoSet);
	//name
	bool setName(std::string nameToSet);
	//time spent waiting 
	bool setTimeSpentWaiting(unsigned int timeToSet);
	bool incrementTimeSpentWaiting();
	//parent
	bool setParent(std::shared_ptr<PCB> parent);
	//children
	bool setNewChild(std::shared_ptr<PCB> child);
	bool setChildren(std::vector<std::shared_ptr<PCB>> chlidren);
	//instructionCounter
	bool setInstructionCounter(unsigned int instructionCounter);
	/**
	* Increment the value of IC.
	* increments the value of the instruction counter by +1.
	*/
	bool incrementInstructionCounter();
	/**
	* Increment the value of IC.
	* increments the value of the instruction counter by valueTOBeIncrementedByParameter.
	*/
	bool incrementInstructionCounter(unsigned int valueToBeIncrementedBy);
	//registers
	bool setRegisters(std::vector<unsigned int> registers);
	bool setRegisters(unsigned int A, unsigned int B, unsigned int C, unsigned int D);
	bool setRegisterA(unsigned int A);
	bool setRegisterB(unsigned int B);
	bool setRegisterC(unsigned int C);
	bool setRegisterD(unsigned int D);
	/**
	 * Resets the registers.
	 * set's all their values to 0.
	 */
	bool resetRegisters();
	//memory
	bool setMemoryPages(std::shared_ptr<MemoryPage> memory);
	bool setNewMemoryPage(MemoryPage memoryPage);
private:

	ProcessState state {ProcessState::NEW};// stan procesu do wykorzystania w kolejkowaniu
	unsigned int PID; // numer identyfikacji procesu
	std::string name; //nazwa procesu
	unsigned int basePriority; // bazowy priorytet - kolejkowanie
	unsigned int timeSpentWaiting;// czas spedzony w kolejce waiting
	std::shared_ptr<PCB> parent; //wskaünik na rodzica procesu

	/**
	 * Process' children.
	 * holds pointers to all the of process' children.
	 */
	std::vector<std::shared_ptr<PCB>> children; 

	/**
	 * Process' children.
	 * holds pointers to all the of process' children.
	 */
	unsigned int instructionCounter;

	/**
	 * Processor registers.
	 * holds the states of FOUR of processor's registers, initially filled with zero's
	 * [0] - register A |[1] - register B |[2] - register C |[3] - register D.
	 */
	std::array<int, 4> registers{0, 0, 0, 0}; 

	std::shared_ptr<MemoryPage> memory; //wskaüni na zasoby pamiÍci przydzielone procesowi
};

