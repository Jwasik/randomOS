
#pragma once
#include "Includes.h"
//#include "VirtualMemory.h
/*
class Memory
{
	struct Frame
	{

		int pid;
		int ProcessPageNr;
		bool dirtyflag = 0;
		int lastUse = 0;
	};

	int8_t ram[128]{ NULL };//pamiec wlasciwa
	void deleteFromMem(int);//czyscze ramke
	Frame FrameTable[8];// informacja czy ramka jest zajêta i jaka strona jest w niej wpisana


public:
	
	Memory();
	//int findFreeFrame();//znajduje wolna ramke, juz niepotrzebne
	void writeInMem(int8_t data, int pid, int logical);//wpisuje bajt do ramki, czeka na zmiany jak bede dostawal page do wpisania
	void printMemory();
	int8_t& getMemoryContent(int pid, int logical);//zwracam bajt po adresie logicznym
	static VirtualMemory getVM();
	void test();
	
};*/