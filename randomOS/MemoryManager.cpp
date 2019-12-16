// Memory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "MemoryManager.h"


VirtualMemory *vm = new VirtualMemory();

Memory::Memory()
{
	
}

void Memory::writeInMem(int pid, int logical,int8_t data)
{
	//wpisuje bajt do odpowiedniej ramki w RAMie
	if (PageTable.find(pid) == PageTable.end())
	{
		this->PageTable.insert(std::make_pair(pid, this->ProcessPages));
	}
	if (this->PageTable.find(pid)->first >= 0 && this->PageTable.find(pid)->second[logical/16].second==1)
	{
		
		
		this->ram[16 * this->PageTable.find(pid)->second[logical / 16].first + logical % 16] = data;
		
		this->FrameTable[PageTable.find(pid)->second[logical / 16].first].pid=pid;
		this->FrameTable[PageTable.find(pid)->second[logical / 16].first].dirtyflag = 1;
		this->PageTable.find(pid)->second[logical / 16].second = 1;
		vm->updateQueue(PageTable.find(pid)->second[logical / 16].first);
	}
	if (this->PageTable.find(pid)->first >= 0 && this->PageTable.find(pid)->second[logical / 16].second == 0)
	{
		int frame = vm->getVictimFrameNumber();
		if (this->FrameTable->dirtyflag == 1)
		{
			VirtualMemory::Page page;
			for (int i = 0; i < 16; i++)
			{
				page.data[i] = ram[frame * 16 + i % 16];
			}

			vm->updateSwapFilePage(FrameTable[frame].pid, logical / 16, page);
		}
		int8_t data = vm->getPage(pid, logical / 16).data[logical % 16];
		
		this->ram[logical % 16 + 16 * frame] = data;
		
		this->PageTable.find(pid)->second[logical / 16].first = frame;
		this->PageTable.find(pid)->second[logical / 16].second = 1;
		vm->updateQueue(frame);
	}
}

void Memory::deleteFromMem(int frame)
{
	for (int index = 16 * frame; index < frame + 16; index++)
	{
		this->ram[index] = 0;

	}
	this->FrameTable[frame].dirtyflag = false;
	
}

void Memory::printMemory()
{
	for (int i = 0; i < 128; i++)
	{
		std::cout << this->ram[i];
		if (i % 16 == 15) std::cout << std::endl;
	}
}

int8_t& Memory::getMemoryContent(int pid, int logical )
{
	
	if (this->PageTable.find(pid) == this->PageTable.end())
	{
		this->PageTable.insert(std::make_pair(pid,this->ProcessPages));
	}
	if (this->PageTable.find(pid)->first >= 0 && this->PageTable.find(pid)->second[logical / 16].first == -1)
	{

		int frame = vm->getVictimFrameNumber();
		if (this->FrameTable->dirtyflag = 1)
		{
			VirtualMemory::Page page;
			for (int i = 0; i < 16; i++)
			{
				page.data[i] = this->ram[frame * 16 + i % 16];
			}

			vm->updateSwapFilePage(this->FrameTable[frame].pid, logical / 16, page);
		}
		for (int i = 0; i < 16; i++)
		{
			int8_t data = vm->getPage(pid, logical / 16).data[logical % 16];
			this->ram[logical % 16 + 16 * frame] = data;
		}
		
		vm->updateQueue(frame);
		this->PageTable.find(pid)->second[logical / 16].first = frame;
		this->PageTable.find(pid)->second[logical / 16].second = 1;

		return this->ram[frame * 16 + logical % 16];
	}
	
	if (this->PageTable.find(pid)->first >= 0 && this->PageTable.find(pid)->second[logical / 16].second == 1)
	{
		return this->ram[this->PageTable.find(pid)->second[logical/16].first * 16 + logical % 16];
	}
	if (this->PageTable.find(pid)->first >= 0 && this->PageTable.find(pid)->second[logical / 16].second == 0)
	{
		int frame = vm->getVictimFrameNumber();

		VirtualMemory::Page page;
		for (int i = 0; i < 16; i++)
		{
			page.data[i] = this->ram[frame * 16 + i % 16];
		}
		vm->updateSwapFilePage(pid, logical / 16, page);

		this->PageTable.find(pid)->second[logical / 16].first = frame;
		this->PageTable.find(pid)->second[logical / 16].second = 1;

		for (int i = 0; i < 16; i++)
		{
			int8_t data = vm->getPage(pid, logical / 16).data[logical % 16];
			this->ram[logical % 16 + 16 * frame] = data;
		}
		
		vm->updateQueue(frame);
		return this->ram[frame * 16 + logical % 16];

	}
	

	//przeglada wszystkie ramki, jesli znajdzie to zwraca jej adres.
	//jesli nie ma jej w RAMie to
	//    probuje sprowadzic ja z pliku wymiany
	//     jesli jest to ja laduje, ale jesli jej nie ma rzucam wyjatek o zlym adresie logicznym
	/*for (int i = 0; i < 8; i++)
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
	return ram[frame * 16 + logical % 16];*/
}

void Memory::printPageTable(int pid)
{
	
	std::cout << "PageTable of "<< pid << std::endl;
	for (int i =0; i<8 ;i++)
	{
		std::cout << "{" << this->PageTable.find(pid)->second[i].first << ", " << this->PageTable.find(pid)->second[i].second << "} "<< std::endl;
	}
	std::cout << std::endl;
	
}

void Memory::test()
{
	int8_t data[16] = { 0 };
	std::vector<VirtualMemory::Page> pages;
	for (int8_t i = 0; i < 8; i++)
	{
		for (int j = 0; j < 16;j++)
		{
			data[j]=i;
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
	VirtualMemory::Page strona = vm->getPage(1, 0);
	strona.print();
	
	
	this->writeInMem(1, 1,strona.data[1]);
	
	this->printMemory();
	this->printPageTable(1);
	
	std::cout<< this->getMemoryContent(1, 0);
	
	
}

int main()
{
	Memory mem;
	//vm->test();
	mem.test();
}