#pragma once
#include "Includes.h"
#include "RUNNING.h"


#define DUMMY_TICS 2 //ammount of processor tics one runtime of dummy process takes
//ERROR CODES
//IN RANGE: <16,32)
#define ERROR_SH_PRIORITY_OUT_OF_RANGE 16
#define ERROR_SH_ADDED_PROCESS_DOES_NOT_EXIST 17


static class Scheduler
{
public:
	uint16_t counter=0;

	Scheduler();
	~Scheduler();
	
	uint8_t schedule();

	uint8_t nextProcess(); //put new process in RUNNING, old goes to waiting queue
	uint8_t addProcess(std::shared_ptr<PCB> process, std::shared_ptr<std::vector<std::shared_ptr<PCB>>> queue);


private:

	uint8_t result;
	std::shared_ptr<std::vector<std::shared_ptr<PCB>>> active;
	std::shared_ptr < std::vector<std::shared_ptr<PCB>>> expired;

	uint8_t normalProcessPriorityAndTimerChange(std::shared_ptr<PCB> process);
	
	friend class Shell;
};