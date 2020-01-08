#pragma once
#include "Includes.h"
#include "FileMenager.h"
#include "MemoryManager.h"
#include "ProcessManager.h"
#include "PCB.h"
#include "RUNNING.h"

class Interpreter
{
private:

	std::shared_ptr<Memory> memory;
	std::shared_ptr<FileMenager> fileSystem;
	std::shared_ptr<ProcessManager> processManager;

	std::shared_ptr<PCB> PCB;
	int8_t AX, BX, CX, DX;
	uint8_t PC;
	unsigned int PID;
	int8_t code;
	bool changeToTerminated;
	std::vector<int8_t> instructionHex;
	std::string instructionString;

	const int8_t maxValue = 127;
	const int8_t minValue = -128;

	void loadPCB();
	uint8_t loadCode();
	uint8_t interpret();
	int8_t& loadArgAdrOrReg();
	int8_t loadArgNum();
	std::string loadArgText(int n);
	void returnToPCB();

	void RET(); //Koniec                                    (0   | 0x00)
	void MOV(); //Przenieœ                                  (1   | 0x01)
	void WRI(); //Wpisz                                     (2   | 0x02)
	uint8_t ADD(); //Dodaj                                     (3   | 0x03)
	uint8_t SUB(); //Odejmij                                   (4   | 0x04)
	uint8_t MUL(); //Pomnó¿                                    (5   | 0x05)
	uint8_t DIV(); //Podziel                                   (6   | 0x06)
	uint8_t MOD(); //Reszta z dzielenia                        (7   | 0x07)
	uint8_t INC(); //Dodaj 1                                   (8   | 0x08)
	uint8_t DEC(); //Odejmnij 1                                (9   | 0x09)
	void JUM(); //Skok bezwarunkowy                         (10  | 0x0A)
	void IFS(); //Skok gdy <0                               (11  | 0x0B)
	void IFE(); //Skok gdy ==0                              (12  | 0x0C)
	void IFB(); //Skok gdy >0                               (13  | 0x0D)
	void IFN(); //Skok gdy !=0                              (14  | 0x0E)
	uint8_t CFI(); //Stwórz plik                               (15  | 0x0F)
	uint8_t DFI(); //Usuñ plik                                 (16  | 0x10)
	uint8_t OFI(); //Otwórz plik                               (17  | 0x11)
	void SFI(); //Zamknij plik                              (18  | 0x12)
	uint8_t EFI(); //Wpisz do pliku (na koniec)                (19  | 0x13)
	uint8_t WFI(); //Wpisz do pliku (liczba)                   (20  | 0x14)
	uint8_t PFI(); //Wpisz do pliku (adres lub rejestr)        (21  | 0x15)
	uint8_t RFI(); //Odczytaj z pliku (liczba)                 (22  | 0x16)
	uint8_t AFI(); //Odczytaj z pliku (adres lub rejestr)      (23  | 0x17)
	uint8_t LFI(); //Odczytaj d³ugoœæ pliku					(24  | 0x18)
	uint8_t CPR(); //Stwórz proces                             (25  | 0x19)

	void NOP(); //Nic nie rób                               (255 | 0xFF)

public:

	Interpreter(std::shared_ptr<Memory> memory, std::shared_ptr<FileMenager> filesystem, std::shared_ptr<ProcessManager> processManager);
	//returns pair <errorCode and executed command string>
	std::pair<uint8_t, std::string> go();
	static std::vector<int8_t> convertToMachine(std::string m);





};

