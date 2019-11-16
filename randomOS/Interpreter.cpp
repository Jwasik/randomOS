#include "Interpreter.h"

Interpreter::Interpreter(std::shared_ptr<Scheduler> scheduler, std::shared_ptr<Memory> memory, std::shared_ptr<FileSystem> filesystem) {

	this->scheduler = scheduler;
	this->memory = memory;
	this->filesystem = filesystem;

	this->PCB = nullptr;
	this->AX = 0;
	this->BX = 0;
	this->CX = 0;
	this->DX = 0;
	this->PC = 0;
	this->code = 0x00;
	this->arg.clear();
}

void Interpreter::loadPCB() {
	PCB = scheduler.getRunningProcess();
}
