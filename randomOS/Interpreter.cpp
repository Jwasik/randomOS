#include "Interpreter.h"

Interpreter::Interpreter(std::shared_ptr<Memory> memory, std::shared_ptr<FileMenager> fileSystem, std::shared_ptr<ProcessManager> processManager) {

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
	uint8_t error = memory->getMemoryContent(PID, PC).first;
	if (error != 0) throw error;
	code = memory->getMemoryContent(PID, PC).second;
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
		instructionString += "IFS";
		IFS();
		break;
	case 0x0C:
		instructionString += "IFE";
		IFE();
		break;
	case 0x0D:
		instructionString += "IFB";
		IFB();
		break;
	case 0x0E:
		instructionString += "IFN";
		IFN();
		break;
	case 0x0F:
		instructionString += "CFI";
		CFI();
		break;
	case 0x10:
		instructionString += "DFI";
		DFI();
		break;
	case 0x11:
		instructionString += "OFI";
		OFI();
		break;
	case 0x12:
		instructionString += "SFI";
		SFI();
		break;
	case 0x13:
		instructionString += "EFI";
		EFI();
		break;
	case 0x14:
		instructionString += "WFI";
		WFI();
		break;
	case 0x15:
		instructionString += "PFI";
		PFI();
		break;
	case 0x16:
		instructionString += "RFI";
		RFI();
		break;
	case 0x17:
		instructionString += "AFI";
		AFI();
		break;
	case 0x18:
		instructionString += "LFI";
		LFI();
		break;
	case 0x19:
		instructionString += "CPR";
		CPR();
		break;
	case 0xFF:
		instructionString += "NOP";
		NOP();
		break;
	default:
		instructionString += "ERR";
		throw (uint8_t)200;
		break;
	}
}

int8_t& Interpreter::loadArgAdrOrReg() {
	uint8_t error = memory->getMemoryContent(PID, PC).first;
	if (error != 0) throw error;

	int8_t adr = memory->getMemoryContent(PID, PC).second;
	PC++;

	instructionHex.push_back(adr);

	switch (adr) {
	case -0x01:
		instructionString += " AX";
		return AX;
		break;
	case -0x02:
		instructionString += " BX";
		return BX;
		break;
	case -0x03:
		instructionString += " CX";
		return CX;
		break;
	case -0x04:
		instructionString += " DX";
		return DX;
		break;
	default:
		instructionString += " [" + std::to_string(adr) + "]";
		error = memory->getMemoryContent(PID, adr).first;
		if (error != 0) throw error;
		return memory->getMemoryContent(PID, adr).second;
		break;
	}
}

int8_t Interpreter::loadArgNum() {
	uint8_t error = memory->getMemoryContent(PID, PC).first;
	if (error != 0) throw error;

	int8_t num = memory->getMemoryContent(PID, PC).second;
	PC++;

	instructionHex.push_back(num);
	instructionString += " " + std::to_string(num);

	return num;
}

std::string Interpreter::loadArgText(int n) {
	uint8_t error = memory->getMemoryContent(PID, PC).first;
	if (error != 0) throw error;

	std::string text = "";
	char t;

	instructionString += " ";

	for (int i = 0; i < n; i++) {
		t = memory->getMemoryContent(PID, PC).second;
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
	if ((a + b) > maxValue) throw (uint8_t)201;
	if ((a + b) < minValue) throw (uint8_t)202;
	a = a + b;
}

void Interpreter::SUB() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	if ((a - b) > maxValue) throw (uint8_t)201;
	if ((a - b) < minValue) throw (uint8_t)202;
	a = a - b;
}

void Interpreter::MUL() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	if ((a * b) > maxValue) throw (uint8_t)201;
	if ((a * b) < minValue) throw (uint8_t)202;
	a = a * b;
}

void Interpreter::DIV() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	if (b == 0) throw (uint8_t)203;
	a = a / b;
}

void Interpreter::MOD() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	if (b == 0) throw (uint8_t)203;
	a = a % b;
}

void Interpreter::INC() {
	int8_t& a = loadArgAdrOrReg();
	if ((a + 1) > maxValue) throw (uint8_t)201;
	if ((a + 1) < minValue) throw (uint8_t)202;
	a = a + 1;
}

void Interpreter::DEC() {
	int8_t& a = loadArgAdrOrReg();
	if ((a - 1) > maxValue) throw (uint8_t)201;
	if ((a - 1) < maxValue) throw (uint8_t)202;
	a = a - 1;
}

void Interpreter::JUM() {
	int8_t a = loadArgNum();
	PC = a;
}

void Interpreter::IFS() {
	int8_t& a = loadArgAdrOrReg();
	int8_t b = loadArgNum();
	if (a < 0) PC = b;
}

void Interpreter::IFE() {
	int8_t& a = loadArgAdrOrReg();
	int8_t b = loadArgNum();
	if (a == 0) PC = b;
}

void Interpreter::IFB() {
	int8_t& a = loadArgAdrOrReg();
	int8_t b = loadArgNum();
	if (a > 0) PC = b;
}

void Interpreter::IFN() {
	int8_t& a = loadArgAdrOrReg();
	int8_t b = loadArgNum();
	if (a != 0) PC = b;
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
	if (error == 67) PC -= 3;
	else if (error != 0) throw error;
}

void Interpreter::SFI() {
	fileSystem->closeProcessFiles(PID);
}

void Interpreter::EFI() {
	int8_t& a = loadArgAdrOrReg();
	//uint8_t error = fileSystem->writeToEndFile(a, PID);
	//if (error != 0) throw error;
}

void Interpreter::WFI() {
	int8_t& a = loadArgAdrOrReg();
	int8_t b = loadArgNum();
	//uint8_t error = fileSystem->writeToFile(a, b, PID);
	//if (error != 0) throw error;
}

void Interpreter::PFI() {
	int8_t& a = loadArgAdrOrReg();
	int8_t& b = loadArgAdrOrReg();
	//uint8_t error = fileSystem->writeToFile(a, b, PID);
	//if (error != 0) throw error;
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

void Interpreter::LFI() {
	int8_t& a = loadArgAdrOrReg();
	uint8_t error = fileSystem->wc(PID).first;
	if (error != 0) throw error;
	a = fileSystem->wc(PID).second;
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

std::pair<uint8_t,std::string> Interpreter::go() {
	try {
		loadPCB();
		loadCode();
		interpret();
		returnToPCB();
		if (changeToTerminated) PCB->setStateTerminated();
	}
	catch (uint8_t e) {
		PCB->setStateTerminated();
		return std::make_pair(e,"");
	}
	return  std::make_pair(0, instructionString);
}

// *******************************************
// **************** KONWERSJA ****************
// *******************************************

std::vector<int8_t> Interpreter::convertToMachine(std::string m) {
	std::vector<int8_t> machine;
	std::vector<std::string> arg;

	if (m[0] >= 48 && m[0] <= 57) {
		machine.push_back(std::stoi(m));
		return machine;
	}

	std::string code = m.substr(0, 3);

	if (m.length() > 3) {

		for (int i = 3; i < m.length(); i++) {
			if (m[i] >= 48 && m[i] <= 57) {
				arg.push_back("");
				for (int j = i; m[j] != ' ' && j < m.size(); j++) {
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
	if (code == "IFS") machine.push_back(0x0B);
	if (code == "IFE") machine.push_back(0x0C);
	if (code == "IFB") machine.push_back(0x0D);
	if (code == "IFN") machine.push_back(0x0E);
	if (code == "CFI") machine.push_back(0x0F);
	if (code == "DFI") machine.push_back(0x10);
	if (code == "OFI") machine.push_back(0x11);
	if (code == "SFI") machine.push_back(0x12);
	if (code == "EFI") machine.push_back(0x13);
	if (code == "WFI") machine.push_back(0x14);
	if (code == "PFI") machine.push_back(0x15);
	if (code == "RFI") machine.push_back(0x16);
	if (code == "AFI") machine.push_back(0x17);
	if (code == "LFI") machine.push_back(0x18);
	if (code == "CPR") machine.push_back(0x19);
	if (code == "NOP") machine.push_back(0xFF);

	if (arg.size() > 0) {
		for (int i = 0; i < arg.size(); i++) {
			if (arg[i] == "AX") machine.push_back(-0x01);
			else if (arg[i] == "BX") machine.push_back(-0x02);
			else if (arg[i] == "CX") machine.push_back(-0x03);
			else if (arg[i] == "DX") machine.push_back(-0x04);
			else if (arg[i][0] >= 65 && arg[i][0] <= 90) {
				machine.push_back(arg[i][0]);
				machine.push_back(arg[i][1]);
			}
			else machine.push_back(std::stoi(arg[i]));
		}
	}
	return machine;
}