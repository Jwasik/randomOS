#pragma once

#include <iostream>
#include <vector>

class Interpreter
{ 
private:

	std::shared_ptr<PCB> PCB;
	char AX, BX, CX, DX;
	unsigned int PC;
	char code;
	std::vector<char> arg;

	void loadPCB();
	void loadCode();
	void loadArguments(int n);
	void interpret();
	void returnToPCB();
	void interruption();

	void HLT(); //Zatrzymaj si�
	void MOV(); //Przenie�
	void WRI(); //Wpisz
	void ADD(); //Dodaj 
	void SUB(); //Odejmij
	void MUL(); //Pomn�
	void DIV(); //Podziel
	void MOD(); //Reszta z dzielenia
	void INC(); //Dodaj 1
	void DEC(); //Odejmnij 1
	void JUM(); //Skok bezwarunkowy (liczba)
	void JUA(); //Skok bezwarunkowy (adres lub rejestr)
	void JIF(); //Skok warunkowy (liczba)
	void JIA(); //Skok warunkowy (adres lub rejestr)
	void CFI(); //Stw�rz plik
	void WFI(); //Wpisz do pliku
	void CPR(); //Stw�rz proces

	std::string converToMnem(char code, std::vector<char> arg);

public:
	Interpreter();
	std::vector<char> convertToMachine(std::string m);
	int exec();
	


};

