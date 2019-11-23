#pragma once
#include "Includes.h"
class Memory 
{
	struct Frame
	{
		int PageNr;
		bool taken=0;
	};

public:
	char ram[128];//pamiec wlasciwa
	Memory(int);
	void writeInMem(int,std::string);//wpisuje max 16 bajtów do ramki, czeka na zmiany jak bede dostawal page do wpisania
	void deleteFromMem(int);//czyscze ramke
	void printMemory();
	int LogicaltoPhysical(int logical);//zamieniam nr na poczatkowy adres frame'a
	char& getMemoryContent(int logical, int PC);//dopóki nie wiem jakie adresy dostane traktuje PID jako adres logiczny
	Frame FrameTable[8];// informacja czy ramka jest zajêta i jaka strona jest w niej wpisana
	

};