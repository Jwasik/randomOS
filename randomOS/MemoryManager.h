#pragma once
#include "Includes.h"
#include "Page.h"
#include "VirtualMemory.h"

class Memory
{
	struct Frame
	{
		int pid;
		int page;
		bool dirtyflag = 0;
	};

	//pamiec wlasciwa
	int8_t ram[128];
	//czyscze ramke
	void deleteFromMem(int);
	// informacja czy ramka jest zajeta i jaka strona jest w niej wpisana
	Frame FrameTable[8];

	std::shared_ptr<VirtualMemory> vm;
public:
	Memory();
	Memory(std::shared_ptr<VirtualMemory>);
	void creatProcess(int pid);



	std::vector<std::pair<int, bool>> ProcessPages
	{
		{ -1, 0 },
	{ -1, 0 },
	{ -1, 0 },
	{ -1, 0 },
	{ -1, 0 },
	{ -1, 0 },
	{ -1, 0 },
	{ -1, 0 }
	};
	std::map<int, std::vector<std::pair<int, bool>>> PageTable;
	void printPageTable(int);



	//wpisuje bajt do ramki, czeka na zmiany jak bede dostawal page do wpisania
	uint8_t writeInMem(int pid, int logical, int8_t data);
	void printMemory();
	//przeglada wszystkie ramki, jesli znajdzie to zwraca jej adres.
	//jesli nie ma jej w RAMie to
	//    probuje sprowadzic ja z pliku wymiany
	//     jesli jest to ja laduje, ale jesli jej nie ma rzucam wyjatek o zlym adresie logicznym
	std::pair<uint8_t, int8_t&> getMemoryContent(int pid, int logical);
	void test();

	friend class Shell;
};