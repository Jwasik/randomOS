#pragma once
#include "Includes.h"

//RAM jest podzielony na ramki, a procesz ¿¹da strony
//Adres fizyczny to adres w ramie
//Proces ¿¹da stronê po adresie logicznym, a pamiêæ zwraca mu ramkê

class Page
{
private:
	friend class Memory;

	uint8_t *memory;
	unsigned int owner;
public:
	uint8_t& at(int index);//na pozycji
	void printPage();
};

class Memory
{
private:
	void exchange(std::pair<uint16_t,Page>, std::pair<uint16_t, Page>);//wywo³uje zamianê stron z plikiem wymiany, za argument bierze adresy fizyczne stron
	void receiveFreePage(std::pair<uint16_t, Page>, std::pair<uint16_t, Page>);//prosi plik stronnicowania o przydzielenie pustej strony i oddaje zape³nion¹

	unsigned int owner[8];/*wskazuje na PID w³aœciciela ramki
	0-1 znaczy ¿e ramka jest wolna*/

	uint8_t addresses[8];//zawiera adresy fizyczne poszczególnych stron

	static constexpr unsigned short memorySize = 128; //B
	uint8_t memory[memorySize];//w³aœciwa pamiêæ
	Page frames[memorySize/16];
public:
	Memory();
	Page& at(int index);
	void printMemory();
};

