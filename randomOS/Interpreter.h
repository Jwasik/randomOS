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

	void HLT(); //Zatrzymaj siê
	void MOV(); //Przenieœ
	void WRI(); //Wpisz
	void ADD(); //Dodaj 
	void SUB(); //Odejmij
	void MUL(); //Pomnó¿
	void DIV(); //Podziel
	void MOD(); //Reszta z dzielenia
	void INC(); //Dodaj 1
	void DEC(); //Odejmnij 1
	void JUM(); //Skok bezwarunkowy (liczba)
	void JUA(); //Skok bezwarunkowy (adres lub rejestr)
	void JIF(); //Skok warunkowy (liczba)
	void JIA(); //Skok warunkowy (adres lub rejestr)
	void CFI(); //Stwórz plik
	void WFI(); //Wpisz do pliku
	void CPR(); //Stwórz proces

	std::string converToMnem(char code, std::vector<char> arg);

public:
	Interpreter();
	std::vector<char> convertToMachine(std::string m);
	int exec();
	


};

