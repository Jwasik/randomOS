// Memory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "MemoryManager.h"


VirtualMemory *vm = new VirtualMemory();



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

int8_t& Memory::getMemoryContent(int logical, int pid)
{
	//przeglada wszystkie ramki, jesli znajdzie to zwraca jej adres.
	//jesli nie ma jej w RAMie to
	//    probuje sprowadzic ja z pliku wymiany
	//     jesli jest to ja laduje, ale jesli jej nie ma rzucam wyjatek o zlym adresie logicznym
	for (int i = 0; i < 8; i++)
	{
		if (FrameTable[i].pid == pid && logical / 16 == FrameTable[i].ProcessPageNr)
		{
			return ram[i * 16 + logical % 16];
		}
	}

	int frame = vm->getVictimFrameNumber();
	this->FrameTable[frame];

	if (FrameTable[frame].dirtyflag == true)
	{
		VirtualMemory::Page page;
		for (int i = 0; i < 16; i++)
		{
			page.data[i] = ram[frame * 16 + i % 16];
		}
		vm->updateSwapFilePage(pid, logical / 16, page);
	}


	for (int i = 0; i < 16; i++)
	{
		int8_t data = vm->getPage(pid, logical / 16).data[logical % 16];
		this->ram[logical % 16 + 16 * frame] = data;
	}
	this->FrameTable[frame].dirtyflag = true;
	vm->updateQueue(frame);
	this->FrameTable[frame].lastUse++;
	this->FrameTable[frame].ProcessPageNr = logical / 16;
	this->FrameTable[frame].pid = pid;
	return ram[frame * 16 + logical % 16];
}

void Memory::test()
{
	int8_t data[16] = { 0 };
	std::vector<VirtualMemory::Page> pages;
	for (int8_t i = 0; i < 8; i++)
	{
		for (int i = 0; i < 16;i++)
		{
			data[i]=i;
		}
		pages.emplace_back(VirtualMemory::Page(data));
	}

	
	
	vm->insertProgram(std::make_pair(1, pages));
	vm->insertProgram(std::make_pair(2, pages));
	vm->insertProgram(std::make_pair(3, pages));
	vm->insertProgram(std::make_pair(4, pages));
	vm->insertProgram(std::make_pair(5, pages));
	vm->insertProgram(std::make_pair(6, pages));
	vm->insertProgram(std::make_pair(7, pages));
	vm->insertProgram(std::make_pair(8, pages));
		
	vm->printSwapFile();
	VirtualMemory::Page dane = vm->getPage(1, 7);
	dane.print();
	
	std::cout << std::endl << std::endl;
	/*this->getMemoryContent(1, 2);
	this->printMemory();
	this->getMemoryContent(2, 14);
	this->printMemory();*/
}

int main()
{
	Memory mem;
	mem.test();
}