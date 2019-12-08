#pragma once
#include "Includes.h"
#include "PCB.h"

static class Scheduler
{
public:
	uint16_t counter;

	Scheduler();
	~Scheduler();
	
	uint8_t schedule();

	uint8_t nextProcess(); //put new process in RUNNING, old goes to waiting queue
	uint8_t addProcess(std::shared_ptr<PCB> process, std::vector<std::shared_ptr<PCB>> *queue);
	uint8_t normalProcessPriorityChange(std::shared_ptr<PCB> process);

private:
	std::vector<std::shared_ptr<PCB>> active;
	std::vector<std::shared_ptr<PCB>> expired;
};