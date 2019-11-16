#include "Interpreter.h"

Interpreter::Interpreter(std::shared_ptr<Scheduler> scheduler, std::shared_ptr<Memory> memory, std::shared_ptr<FileSystem> fileSystem, std::shared_ptr<ProcessManager> processManager) {

	this->scheduler = scheduler;
	this->memory = memory;
	this->fileSystem = fileSystem;
	this->processManager = processManager;

	this->PCB = nullptr;
	this->AX = 0;
	this->BX = 0;
	this->CX = 0;
	this->DX = 0;
	this->PC = 0;
	this->PID = 0;
	this->code = 0x00;
	this->isRET = false;
	this->instructionHex.clear();
	this->instructionString = "";
}

void Interpreter::loadPCB() {
	PCB = scheduler->getRunningProcess();
	AX = PCB->getRegisterA();
	BX = PCB->getRegisterB();
	CX = PCB->getRegisterC();
	DX = PCB->getRegisterD();
	PC = PCB->getInstructionCounter();
	PID = PCB->getPID();
	isRET = false;
	instructionString = "";
	instructionHex.clear();
}

void Interpreter::loadCode() {
	code = memory->getLogicalMemory(PC, PID);
	PC++;
	instructionHex.push_back(code);
}

int Interpreter::interpret() {
	switch (code) {
	case 0x00:
		instructionString += "RET";
		RET();
		break;
	case 0x01:
		instructionString += "MOV";
		MOV();
		break;
	case 0x02:
		instructionString += "WRI";
		WRI();
		break;
	case 0x03:
		instructionString += "ADD";
		ADD();
		break;
	case 0x04:
		instructionString += "SUB";
		SUB();
		break;
	case 0x05:
		instructionString += "MUL";
		MUL();
		break;
	case 0x06:
		instructionString += "DIV";
		DIV();
		break;
	case 0x07:
		instructionString += "MOD";
		MOD();
		break;
	case 0x08:
		instructionString += "INC";
		INC();
		break;
	case 0x09:
		instructionString += "DEC";
		DEC();
		break;
	case 0x0A:
		instructionString += "JUM";
		JUM();
		break;
	case 0x0B:
		instructionString += "JUA";
		JUA();
		break;
	case 0x0C:
		instructionString += "JIF";
		JIF();
		break;
	case 0x0D:
		instructionString += "JIA";
		JIA();
		break;
	case 0x0E:
		instructionString += "CFI";
		CFI();
		break;
	case 0x0F:
		instructionString += "DFI";
		DFI();
		break;
	case 0x10:
		instructionString += "OFI";
		OFI();
		break;
	case 0x11:
		instructionString += "SFI";
		SFI();
		break;
	case 0x12:
		instructionString += "WFI";
		WFI();
		break;
	case 0x13:
		instructionString += "CPR";
		CPR();
		break;
	case 0xFF:
		instructionString += "CPR";
		NOP();
		break;
	default:
		instructionString += "ERR";
		return -1;
		break;
	}
	return 0;
}

char& Interpreter::loadArgAdrOrReg() {
	char& adr = memory->getLogicalMemory(PC, PID);
	PC++;

	instructionHex.push_back(adr);

	switch (adr) {
	case 0xFF:
		instructionString += " AX";
		return AX;
		break;
	case 0xFE:
		instructionString += " BX";
		return BX;
		break;
	case 0xFD:
		instructionString += " CX";
		return CX;
		break;
	case 0xFC:
		instructionString += " DX";
		return DX;
		break;
	default:
		instructionString += " [" + std::to_string(adr) + "]";
		return adr;
		break;
	}
}

char Interpreter::loadArgNum() {
	char num = memory->getLogicalMemory(PC, PID);
	PC++;
	return num;
}

std::string Interpreter::loadArgText(int n) {
	std::string text = "";
	for (int i = 0; i < n; i++) {
		text += memory->getLogicalMemory(PC, PID);
		PC++;
	}
	return text;
}

void Interpreter::returnToPCB() {
	PCB->setRegisterA(AX);
	PCB->setRegisterB(BX);
	PCB->setRegisterC(CX);
	PCB->setRegisterD(DX);
	PCB->setInstructionCounter(PC);
}

// *****************************************
// ********** INSTRUKCJE ASEMBLER **********
// *****************************************

void Interpreter::RET() {
	isRET = true;
}

void Interpreter::MOV() {
	char& a = loadArgAdrOrReg();
	char& b = loadArgAdrOrReg();
	a = b;
}

void Interpreter::WRI() {
	char& a = loadArgAdrOrReg();
	char b = loadArgNum();
	a = b;
}

void Interpreter::ADD() {
	char& a = loadArgAdrOrReg();
	char& b = loadArgAdrOrReg();
	a = a + b;
}

void Interpreter::SUB() {
	char& a = loadArgAdrOrReg();
	char& b = loadArgAdrOrReg();
	a = a - b;
}

void Interpreter::MUL() {
	char& a = loadArgAdrOrReg();
	char& b = loadArgAdrOrReg();
	a = a * b;
}

void Interpreter::DIV() {
	char& a = loadArgAdrOrReg();
	char& b = loadArgAdrOrReg();
	a = a / b;
}

void Interpreter::MOD() {
	char& a = loadArgAdrOrReg();
	char& b = loadArgAdrOrReg();
	a = a % b;
}

void Interpreter::INC() {
	char& a = loadArgAdrOrReg();
	a = a + 1;
}

void Interpreter::DEC() {
	char& a = loadArgAdrOrReg();
	a = a - 1;
}

void Interpreter::JUM() {
	char a = loadArgNum(); 
	PC = a;
}

void Interpreter::JUA() {
	char& a = loadArgAdrOrReg();
	PC = a;
}

void Interpreter::JIF() {
	char& a = loadArgAdrOrReg();
	char b = loadArgNum();
	if (a == 0) PC = b;
}

void Interpreter::JIA() {
	char& a = loadArgAdrOrReg();
	char& b = loadArgAdrOrReg();
	if (a == 0) PC = b;
}

void Interpreter::CFI() {
	std::string a = loadArgText(2);
	fileSystem->createFile(a);
}

void Interpreter::DFI() {
	std::string a = loadArgText(2);
	fileSystem->deleteFile(a);
}

void Interpreter::OFI() {
	std::string a = loadArgText(2);
	fileSystem->openFile(a, PID);
}

void Interpreter::SFI() {
	std::string a = loadArgText(2);
	fileSystem->closeFile(a, PID);
}

void Interpreter::WFI() {
	char &a = loadArgAdrOrReg();
	fileSystem->writeToFile(std::to_string(a), PID);
}

void Interpreter::CPR() {
	std::string a = loadArgText(2);
	std::string b = loadArgText(2);
	processManager->fork(a, PID, b);
}
