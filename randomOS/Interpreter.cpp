#include "Interpreter.h"

/*Interpreter::Interpreter(std::shared_ptr<Scheduler> scheduler, std::shared_ptr<Memory> memory, std::shared_ptr<FileMenager> fileSystem, std::shared_ptr<ProcessManager> processManager) {

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
	this->changeToTerminated = false;
	this->instructionHex.clear();
	this->instructionString = "";
}
*/
void Interpreter::loadPCB() {
	PCB = RUNNING;
	AX = PCB->getRegisterA();
	BX = PCB->getRegisterB();
	CX = PCB->getRegisterC();
	DX = PCB->getRegisterD();
	PC = PCB->getInstructionCounter();
	PID = PCB->getPID();
	changeToTerminated = false;
	instructionString = "";
	instructionHex.clear();
}

void Interpreter::loadCode() {
	code = memory->getMemoryContent(PC, PID).second;
	PC++;
	instructionHex.push_back(code);
}

void Interpreter::interpret() {
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
		instructionString += "EFI";
		EFI();
		break;
	case 0x13:
		instructionString += "WFI";
		WFI();
		break;
	case 0x14:
		instructionString += "PFI";
		PFI();
		break;
	case 0x15:
		instructionString += "RFI";
		RFI();
		break;
	case 0x16:
		instructionString += "AFI";
		AFI();
		break;
	case 0x17:
		instructionString += "CPR";
		CPR();
		break;
	case 0xFF:
		instructionString += "NOP";
		NOP();
		break;
	default:
		instructionString += "ERR";
		throw 200;
		break;
	}
}

int8_t& Interpreter::loadArgAdrOrReg() {
	int8_t& adr = memory->getMemoryContent(PC, PID).second;
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

int8_t Interpreter::loadArgNum() {
	int8_t num = memory->getMemoryContent(PC, PID).second;
	PC++;

	instructionHex.push_back(num);
	instructionString += " " + std::to_string(num);

	return num;
}

std::string Interpreter::loadArgText(int n) {
	std::string text = "";
	char t;

	instructionString += " ";

	for (int i = 0; i < n; i++) {
		t = memory->getMemoryContent(PC, PID).second;
		PC++;
		text += t;

		instructionHex.push_back(t);
		instructionString += t;
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
	changeToTerminated = true;
}

void Interpreter::MOV() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	a = b;
}

void Interpreter::WRI() {
	int8_t& a = loadArgAdrOrReg();
	int8_t b = loadArgNum();
	a = b;
}

void Interpreter::ADD() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	if ((a + b) > maxValue) throw 201;
	if ((a + b) < minValue) throw 202;
	a = a + b;
}

void Interpreter::SUB() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	if ((a - b) > maxValue) throw 201;
	if ((a - b) < minValue) throw 202;
	a = a - b;
}

void Interpreter::MUL() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	if ((a * b) > maxValue) throw 201;
	if ((a * b) < minValue) throw 202;
	a = a * b;
}

void Interpreter::DIV() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	if (b == 0) throw 203;
	a = a / b;
}

void Interpreter::MOD() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	a = a % b;
}

void Interpreter::INC() {
	int8_t& a = loadArgAdrOrReg();
	if ((a + 1) > maxValue) throw 201;
	if ((a + 1) < maxValue) throw 202;
	a = a + 1;
}

void Interpreter::DEC() {
	int8_t& a = loadArgAdrOrReg();
	if ((a - 1) > maxValue) throw 201;
	if ((a - 1) < maxValue) throw 202;
	a = a - 1;
}

void Interpreter::JUM() {
	int8_t a = loadArgNum();
	PC = a;
}

void Interpreter::JUA() {
	int8_t& a = loadArgAdrOrReg();
	PC = a;
}

void Interpreter::JIF() {
	int8_t& a = loadArgAdrOrReg();
	int8_t b = loadArgNum();
	if (a == 0) PC = b;
}

void Interpreter::JIA() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	if (a == 0) PC = b;
}

void Interpreter::CFI() {
	std::string a = loadArgText(2);
	uint8_t error = fileSystem->createFile(a);
	if (error != 0) throw error;
}

void Interpreter::DFI() {
	std::string a = loadArgText(2);
	uint8_t error = fileSystem->deleteFile(a);
	if (error != 0) throw error;
}

void Interpreter::OFI() {
	std::string a = loadArgText(2);
	uint8_t error = fileSystem->openFile(a, PID);
	if (error == 67) PC--;
	else if (error != 0) throw error;
}

void Interpreter::SFI() {
	std::string a = loadArgText(2);
	uint8_t error = fileSystem->closeFile(a, PID);
	if (error != 0) throw error;
}

void Interpreter::EFI() {
	int8_t& a = loadArgAdrOrReg();
	uint8_t error = fileSystem->writeToEndFile(a, PID);
	if (error != 0) throw error;
}

void Interpreter::WFI() {
	int8_t& a = loadArgAdrOrReg();
	int8_t b = loadArgNum();
	uint8_t error = fileSystem->writeToFile(a, b, PID);
	if (error != 0) throw error;
}

void Interpreter::PFI() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	uint8_t error = fileSystem->writeToFile(a, b, PID);
	if (error != 0) throw error;
}

void Interpreter::RFI() {
	int8_t& a = loadArgAdrOrReg();
	int8_t b = loadArgNum();
	uint8_t error = fileSystem->readFile(a, b, 1, PID);
	if (error != 0) throw error;
}

void Interpreter::AFI() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	uint8_t error = fileSystem->readFile(a, b, 1, PID);
	if (error != 0) throw error;
}

void Interpreter::CPR() {
	std::string a = loadArgText(2);
	std::string b = loadArgText(2);
	uint8_t error = processManager->fork(a, PID, b).first;
	if (error != 0) throw error;
}

void Interpreter::NOP() {}

// ******************************************
// ******************* GO *******************
// ******************************************

Interpreter::Interpreter(std::shared_ptr<Memory> memory, std::shared_ptr<FileMenager> filesystem, std::shared_ptr<ProcessManager> processManager) : memory(memory), fileSystem(filesystem), processManager(processManager)
{
}

uint8_t Interpreter::go() {
	try {
		loadPCB();
		loadCode();
		interpret();
		returnToPCB();
		if (changeToTerminated) PCB->setStateTerminated();
	}
	catch (uint8_t e) {
		PCB->setStateTerminated();
		return e;
	}
	return 0;
}

// *******************************************
// **************** KONWERSJA ****************
// *******************************************

std::vector<uint8_t> Interpreter::convertToMachine(std::string m) {
	std::vector<uint8_t> machine;
	std::vector<std::string> arg;

	std::string code = m.substr(0, 3);

	if (m.length() > 3) {

		for (int i = 3; i < m.length(); i++) {
			if (m[i] >= 48 && m[i] <= 57) {
				arg.push_back("");
				for (int j = i; m[j] != ' '; j++) {
					arg.back() += m[j];
				}
				i += arg.back().length();
			}
			else if (m[i] == '[') {
				arg.push_back("");
				for (int j = i + 1; m[j] != ']'; j++) {
					arg.back() += m[j];
				}
				i += arg.back().length() + 1;
			}
			else if (m[i] >= 65 && m[i] <= 68) {
				arg.push_back("");
				arg.back() += m[i];
				arg.back() += m[i + 1];
				i += 2;
			}
			else if (m[i] == '"') {
				arg.push_back("");
				arg.back() += m[i + 1];
				arg.back() += m[i + 2];
				i += 3;
			}

		}
	}

	if (code == "RET") machine.push_back(0x00);
	if (code == "MOV") machine.push_back(0x01);
	if (code == "WRI") machine.push_back(0x02);
	if (code == "ADD") machine.push_back(0x03);
	if (code == "SUB") machine.push_back(0x04);
	if (code == "MUL") machine.push_back(0x05);
	if (code == "DIV") machine.push_back(0x06);
	if (code == "MOD") machine.push_back(0x07);
	if (code == "INC") machine.push_back(0x08);
	if (code == "DEC") machine.push_back(0x09);
	if (code == "JUM") machine.push_back(0x0A);
	if (code == "JUA") machine.push_back(0x0B);
	if (code == "JIF") machine.push_back(0x0C);
	if (code == "JIA") machine.push_back(0x0D);
	if (code == "CFI") machine.push_back(0x0E);
	if (code == "DFI") machine.push_back(0x0F);
	if (code == "OFI") machine.push_back(0x10);
	if (code == "SFI") machine.push_back(0x11);
	if (code == "EFI") machine.push_back(0x12);
	if (code == "WFI") machine.push_back(0x13);
	if (code == "PFI") machine.push_back(0x14);
	if (code == "RFI") machine.push_back(0x15);
	if (code == "AFI") machine.push_back(0x16);
	if (code == "CPR") machine.push_back(0x17);
	if (code == "NOP") machine.push_back(0xFF);

	if (arg.size() > 0) {
		for (int i = 0; i < arg.size(); i++) {
			if (arg[i] == "AX") machine.push_back(0xFF);
			else if (arg[i] == "BX") machine.push_back(0xFE);
			else if (arg[i] == "CX") machine.push_back(0xFD);
			else if (arg[i] == "DX") machine.push_back(0xFC);
			else if (arg[i][0] >= 65 && arg[i][0] <= 90) {
				machine.push_back(arg[i][0]);
				machine.push_back(arg[i][1]);
			}
			else machine.push_back(std::stoi(arg[i]));
		}
	}
	return machine;
}