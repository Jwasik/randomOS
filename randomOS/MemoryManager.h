#pragma once
#include "Includes.h"
#include "VirtualMemory.h"
class Memory  
{
	struct Frame
	{
		
		int pid;
		int ProcessPageNr;
		bool taken=0;
		int lastUse=0;
	};

public:
	uint8_t ram[128];//pamiec wlasciwa
	Memory();
	int findFreeFrame();//znajduje wolna ramke
	void writeInMem(uint8_t data,int pid, int logical);//wpisuje strone do ramki, czeka na zmiany jak bede dostawal page do wpisania
	void deleteFromMem(int);//czyscze ramke
	void printMemory();
	uint8_t& getMemoryContent(int logical, int pid);//zwracam bajt po adresie logicznym
	Frame FrameTable[8];// informacja czy ramka jest zajêta i jaka strona jest w niej wpisana
	

};