// Memory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "MemoryManager.h"

/*int Memory::findFreeFrame()
{
	for (int i = 0; i < 8; i++)
	{
		if (FrameTable[i].taken == false)
		{
			return i;
		}
		else
		{
			std::cout << " no free frame " << std::endl;
			return 8;
		}
	}
}*/

Memory::Memory()
{
}

Memory::Memory(std::shared_ptr<VirtualMemory> vm) : virtualMemory(vm){}

void Memory::writeInMem(int8_t data, int pid, int logical)
{
	//wpisuje bajt do odpowiedniej ramki w RAMie

	for (int i = 0; i < 16; i++)
	{
		if (FrameTable[i].pid == pid && FrameTable[i].ProcessPageNr == logical / 16)
		{
			this->ram[16 * i + logical % 16] = data;
			this->FrameTable[i].dirtyflag = true;
		}
	}

}

void Memory::deleteFromMem(int frame)
{
	for (int index = 16 * frame; index < frame + 16; index++)
	{
		this->ram[index] = 0;

	}
	this->FrameTable[frame].dirtyflag = false;
	this->FrameTable[frame].lastUse = 0;
}

void Memory::printMemory()
{
	for (int i = 0; i < 128; i++)
	{
		std::cout << this->ram[i];
		if (i % 16 == 15) std::cout << std::endl;
	}
}

std::pair<uint8_t,int8_t&> Memory::getMemoryContent(int logical, int pid)
{
	//przeglada wszystkie ramki, jesli znajdzie to zwraca jej adres.
	//jesli nie ma jej w RAMie to
	//probuje sprowadzic ja z pliku wymiany
	//jesli jest to ja laduje, ale jesli jej nie ma rzucam wyjatek o zlym adresie logicznym
	
	//ramka znajduje się w ramie
	for (int i = 0; i < 8; i++)
	{
		if (FrameTable[i].pid == pid && logical / 16 == FrameTable[i].ProcessPageNr)
		{
			return {0, ram[i * 16 + logical % 16] };
		}
	}

	int frame = this->virtualMemory->getVictimFrameNumber();//numer ramki ofiary, czyli miejsca gdzie wpadnie żądana strona
	this->FrameTable[frame];//?! co to ma niby robić, nie przypisue wartości do niczego

	//zapisuje zmiany w ramce ofierze
	if (FrameTable[frame].dirtyflag == true)
	{
		VirtualMemory::Page page;
		for (int i = 0; i < 16; i++)
		{
			page.data[i] = ram[frame * 16 + i % 16];
		}
		virtualMemory->updateSwapFilePage(pid, logical / 16, page);//jesteś pewnien że nie powinieneś zapisać ramki ofiary, tylko zapisać ją na miejsce żądanej strony?
	}


	for (int i = 0; i < 16; i++)
	{//czy ty tutaj cały czas zapisujesz dane do jednej komórki, jaki to ma sens?
		int8_t data = virtualMemory->getPage(pid, logical / 16).data[logical % 16];
		this->ram[(16 * frame)+ (logical % 16)] = data;//jebane nawiasy rób, bo inaczej się liczy od lewej do prawej
	}

	this->FrameTable[frame].dirtyflag = true;//inicjacja pobranej strony
	virtualMemory->updateQueue(frame);
	this->FrameTable[frame].lastUse++;//co to jest last use i do czego jest wykorzystywane?

	this->FrameTable[frame].ProcessPageNr = logical / 16;
	this->FrameTable[frame].pid = pid;
	return { 0,ram[(frame * 16) + (logical % 16)] };//znowu jebane nawiasy
}

void Memory::test()
{
	int8_t data[16];
	std::vector<VirtualMemory::Page> pages;
	for (int8_t i = 0; i < 8; i++)
	{
		for (int j = 0; j < 16;j++)
		{
			data[i]=i;
		}
		pages.emplace_back(VirtualMemory::Page(data));
	}

	
	
	virtualMemory->insertProgram(std::make_pair(1, pages));
	virtualMemory->insertProgram(std::make_pair(2, pages));
	virtualMemory->insertProgram(std::make_pair(3, pages));
	virtualMemory->insertProgram(std::make_pair(4, pages));
	virtualMemory->insertProgram(std::make_pair(5, pages));
	virtualMemory->insertProgram(std::make_pair(6, pages));
	virtualMemory->insertProgram(std::make_pair(7, pages));
	virtualMemory->insertProgram(std::make_pair(8, pages));
		
	virtualMemory->printSwapFile();
	VirtualMemory::Page dane = virtualMemory->getPage(1, 7);
	dane.print();
	
	std::cout << std::endl << std::endl;
	this->getMemoryContent(1, 2);
	this->printMemory();
	this->getMemoryContent(2, 14);
	this->printMemory();
}