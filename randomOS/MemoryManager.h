#pragma once
#include "Includes.h"
#include "VirtualMemory.h"
class Memory
{
	struct Frame
	{

		int pid;
		int ProcessPageNr;
		bool dirtyflag = 0;
		int lastUse = 0;
	};
	//pamiec wlasciwa
	int8_t ram[128]{ 0 };
	//czyscze ramke
	void deleteFromMem(int);
	// informacja czy ramka jest zajeta i jaka strona jest w niej wpisana
	Frame FrameTable[8];


public:

	Memory();
	//wpisuje bajt do ramki, czeka na zmiany jak bede dostawal page do wpisania
	void writeInMem(int8_t data, int pid, int logical);
	void printMemory();
	//przeglada wszystkie ramki, jesli znajdzie to zwraca jej adres.
	//jesli nie ma jej w RAMie to
	//    probuje sprowadzic ja z pliku wymiany
	//     jesli jest to ja laduje, ale jesli jej nie ma rzucam wyjatek o zlym adresie logicznym
	int8_t& getMemoryContent(int pid, int logical);
	void test();

};