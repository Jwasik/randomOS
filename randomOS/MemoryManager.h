#pragma once


#include "Includes.h"
#include "VirtualMemory.h"
class Memory
{
	struct Frame
	{
		int pid;
		bool dirtyflag = 0;
	};
	
	//pamiec wlasciwa
	int8_t ram[128]{ 0 };
	//czyscze ramke
	void deleteFromMem(int);
	// informacja czy ramka jest zajeta i jaka strona jest w niej wpisana
	Frame FrameTable[8]{

		
			
	};
	std::vector<std::pair<int, bool>> ProcessPages
	{
		{-1, 0},
		{-1, 0},
		{-1, 0},
		{-1, 0},
		{-1, 0},
		{-1, 0},
		{-1, 0},
		{-1, 0}
	};

public:
	Memory();
	std::map<int, std::vector<std::pair<int, bool>>> PageTable;
	void printPageTable(int);
	
	
	
	//wpisuje bajt do ramki, czeka na zmiany jak bede dostawal page do wpisania
	void writeInMem(int pid, int logical,int8_t data);
	void printMemory();
	//przeglada wszystkie ramki, jesli znajdzie to zwraca jej adres.
	//jesli nie ma jej w RAMie to
	//    probuje sprowadzic ja z pliku wymiany
	//     jesli jest to ja laduje, ale jesli jej nie ma rzucam wyjatek o zlym adresie logicznym
	int8_t& getMemoryContent(int pid, int logical);
	void test();

};