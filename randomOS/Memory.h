#pragma once
#include "Includes.h"

//RAM jest podzielony na ramki, a procesz ��da strony
//Adres fizyczny to adres w ramie
//Proces ��da stron� po adresie logicznym, a pami�� zwraca mu ramk�

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
	void exchange(std::pair<uint16_t,Page>, std::pair<uint16_t, Page>);//wywo�uje zamian� stron z plikiem wymiany, za argument bierze adresy fizyczne stron
	void receiveFreePage(std::pair<uint16_t, Page>, std::pair<uint16_t, Page>);//prosi plik stronnicowania o przydzielenie pustej strony i oddaje zape�nion�

	unsigned int owner[8];/*wskazuje na PID w�a�ciciela ramki
	0-1 znaczy �e ramka jest wolna*/

	uint8_t addresses[8];//zawiera adresy fizyczne poszczeg�lnych stron

	static constexpr unsigned short memorySize = 128; //B
	uint8_t memory[memorySize];//w�a�ciwa pami��
	Page frames[memorySize/16];
public:
	Memory();
	Page& at(int index);
	void printMemory();
};

