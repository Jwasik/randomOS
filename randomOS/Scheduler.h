#pragma once
#include "Includes.h"
#include "PCB.h"


class Prosessor
{
public:
	int registerA;
	int registerB;
	int registerC;
	int registerD;

	std::shared_ptr<PCB> RUNNING;

	Prosessor();
	~Prosessor();
	

	int schedule();
	int nexstProcess();



private:
	std::vector<std::shared_ptr<PCB>> ready;
	std::vector<std::shared_ptr<PCB>> waiting;







}