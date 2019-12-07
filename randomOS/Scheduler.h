#pragma once
#include "Includes.h"
#include "PCB.h"

static class Scheduler
{
public:
	uint16_t caunter;

	Scheduler();
	~Scheduler();
	
	uint8_t schedule();
	uint8_t nextProcess();
	uint8_t addProcess(std::shared_ptr<PCB> process, std::vector<std::shared_ptr<PCB>> *queue);


private:
	std::vector<std::shared_ptr<PCB>> ready;
	std::vector<std::shared_ptr<PCB>> waiting;
};