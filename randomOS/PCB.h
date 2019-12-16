#pragma once
#include "Includes.h"

class MemoryPage;

#define NUMBER_OF_PROCESS_STATES 5
#define MAX_PROCESS_NAME_LENGHT 15

/********************************
 *  Process Control Block.      *
 *  used to manage a process.   *
 ********************************/
class PCB
{
public:
	/********************************
	*         PROCESSSTATE          *
	********************************/
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
		TERMINATED, /**< the process has finished its runtime. */
	};
	

	/********************************
	*         CONSTRUCTORS          *
	********************************/
	PCB();
	~PCB();
	/**
	 * Basic constructor.
	 * @param name the name of the process that is to be created.
	 * @param PID the PID that is to be assigned to the process.
	 * @param parent a poitner to the parent of the process.
	 */
	PCB(std::string name, unsigned int PID, std::shared_ptr<PCB> parent);


	/********************************
	*       PUBLIC VARIABLES        *
	********************************/
	//used in scheduling
	uint8_t priority;
	uint8_t basePriority;
	int counter;

	/********************************
	*            GETTERS            *
	********************************/
	//----------|| STATE ||----------||
	unsigned int getStateAsInt();
	ProcessState getStateAsEnum();
	bool getIsNew();
	bool getIsReady();
	bool getIsRunning();
	bool getIsWaiting();
	bool getIsTerminated();
	bool getHasState(ProcessState state);
	

	//-----------|| PID ||-------------||
	unsigned int getPID();
	bool getHasPID(const unsigned int& PIDtoCompare);
	

	//-----------|| NAME ||------------||
	std::string getName();
	bool getHasName(const std::string& nameToCompare);
	std::string getNameAndPIDString();


	//---------|| PARENT ||------------||
	std::shared_ptr<PCB> getParentPCB();
	unsigned int getPartentPID();
	bool getIsParentOf(unsigned int PID);


	//---------|| CHILDREN ||----------||
	std::vector<std::shared_ptr<PCB>> getChildren();
	std::vector<std::shared_ptr<PCB>> getChildrenInReverseOrder();
	bool getIsChildOf(unsigned int PID);
	bool getHasChildren();
	unsigned int getLastChildPID();
	bool getIsLastChild();
	

	//----|| INSTRUCTION COUNTER ||----||
	unsigned int getInstructionCounter();


	//---------|| REGISTERS ||---------||
	std::array<int, 4> getRegisters();
	int getRegisterA();
	int getRegisterB();
	int getRegisterC();
	int getRegisterD();
	

	//----------|| MEMORY ||----------||


	//----------|| CONSOLE ||---------||
	/**
	 * Get data about the process.
	 * basically a to string method, for use in step-mode.
	 */
	std::string getInformation();


	/********************************
	 *          MODIFIERS           *
	 ********************************/
	//----------|| STATE ||----------||
	bool setState(const unsigned int& stateToSet);
	bool setState(const ProcessState& stateToSet);
	bool setStateReady();
	bool setStateRunning();
	bool setStateWaiting();
	bool setStateTerminated();
	

	//-------=---||  PID  ||-----------||


	//------------|| NAME ||-----------||
	bool setName(const std::string& nameToSet);
	

	//----------|| PARENT ||-----------||
	bool setParent(const std::shared_ptr<PCB>& parent);


	//---------|| CHILDREN ||----------||
	bool addChild(const std::shared_ptr<PCB>& child);
	bool addChildren(const std::vector<std::shared_ptr<PCB>>& chlidren);
	bool removeChild(const std::shared_ptr<PCB>& child);
	/**
	* Overwrites current children of the process.
	*/
	bool setChildren(const std::vector<std::shared_ptr<PCB>>& chlidren);


	//----|| INSTRUCTION COUNTER ||----||
	bool setInstructionCounter(const unsigned int& instructionCounter);
	/**
	* Increment the value of IC.
	* increments the value of the instruction counter by +1.
	*/
	bool incrementInstructionCounter();
	/**
	* Increment the value of IC by argument.
	* increments the value of the instruction counter by valueTOBeIncrementedByParameter.
	*/
	bool incrementInstructionCounter(const unsigned int& valueToBeIncrementedBy);


	//---------|| REGISTERS ||---------||
	bool setRegisters(const std::vector<unsigned int>& registers);
	bool setRegisters(const std::array<int, 4>& registers);
	bool setRegisters(const unsigned int& A, const unsigned int& B, const  unsigned int& C, const unsigned int& D);
	bool setRegisterA(const unsigned int& A);
	bool setRegisterB(const unsigned int& B);
	bool setRegisterC(const unsigned int& C);
	bool setRegisterD(const unsigned int& D);
	/**
	 * Resets the registers.
	 * set's all their values to 0.
	 */
	bool resetRegisters();
	

	//---------|| MEMORY ||---------||
	bool setMemoryPages(const std::shared_ptr<std::vector<MemoryPage>>& memory);
private:

	/********************************
	 *          DATA FIELDS         *
	 ********************************/
	unsigned int PID=10000;
	std::string name {};
	ProcessState state {ProcessState::NEW};
	std::shared_ptr<PCB> parent=nullptr;
	std::vector<std::shared_ptr<PCB>> children;
	unsigned int basePriority=0; 
	unsigned int instructionCounter=0;
	/**
	 * Processor registers.
	 * holds the states of FOUR of processor's registers, initially filled with zero's
	 * [0] - register A |[1] - register B |[2] - register C |[3] - register D.
	 */
	std::array<int, 4> registers{0, 0, 0, 0}; 
	std::shared_ptr<std::vector<MemoryPage>> memory=nullptr; 


	/********************************
	 *      ADDITIONAL METHODS      *
	 ********************************/
	/**
	 * Check for overflow when adding two unsigned ints.
	 * will return 0 if the operation WILL OVERFLOW, and 1 if it won't.
	 */
	bool checkWontOverflowUnsignedInt(unsigned int currentValue, unsigned int valueToBeAdded);
	/**
	* Check for overflow when adding two ints.
	* will return 0 if the operation WILL OVERFLOW, and 1 if it won't.
	*/
	bool checkWontOverflowInt(int currentValue, int valueToBeAdded);
	std::string getStateAsString();
	std::string getChildrenAsString();
	std::string getParentAsString();
};

