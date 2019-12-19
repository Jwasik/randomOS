// Memory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "MemoryManager.h"

Memory::Memory()
{

}

Memory::Memory(std::shared_ptr<VirtualMemory> vm) : vm(vm)
{

}

uint8_t Memory::writeInMem(int pid, int logical, int8_t data)
{
	//if process has no PageTable it assigns new one to it
	if (PageTable.find(pid) == PageTable.end())
	{
		this->PageTable.insert(std::make_pair(pid, this->ProcessPages));
	}
	//process has PageTable and is written in RAM
	if (this->PageTable.find(pid)->first >= 0 && this->PageTable.find(pid)->second[logical / 16].second == 1)
	{
		this->ram[(16 * this->PageTable.find(pid)->second[logical / 16].first) + (logical % 16)] = data;
		vm->updateQueue(PageTable.find(pid)->second[logical / 16].first);

	}
	//process has PageTable but is not written in RAM
	if (this->PageTable.find(pid)->first >= 0 && this->PageTable.find(pid)->second[logical / 16].second == 0)
	{
		int frame = vm->getVictimFrameNumber();
		//if Page in this Frame has changed it is uploaded back to swapFile
		if (this->FrameTable[frame].dirtyflag == 1)
		{
			VirtualMemory::Page page;
			for (int i = 0; i < 16; i++)
			{
				page.data[i] = this->ram[(frame * 16) + i];
			}
			vm->updateSwapFilePage(FrameTable[frame].pid, logical / 16, page);
		}
		//Page that we want to change is being downloaded from swapFile
		VirtualMemory::Page dane = vm->getPage(pid, logical / 16);
		for (int i = 0; i < 16; i++)
		{
			//ta linijka nie dziala
			this->ram[(logical % 16) + (16 * frame)] = dane.data[i];
		}
		//the byte that we wanted to write is being written at the logical adress
		this->ram[(logical % 16) + (16 * frame)] = data;
		//update PageTable
		this->PageTable.find(pid)->second[logical / 16].first = frame;
		vm->updateQueue(frame);

	}
	//update information about frame
	this->FrameTable[PageTable.find(pid)->second[logical / 16].first].pid = pid;
	this->FrameTable[PageTable.find(pid)->second[logical / 16].first].dirtyflag = 1;
	//set byte of correctness
	this->PageTable.find(pid)->second[logical / 16].second = 1;
	return 0;

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

std::pair<uint8_t, int8_t&> Memory::getMemoryContent(int pid, int logical)
{
	//if process has no PageTable it assigns new one to it
	if (this->PageTable.find(pid) == this->PageTable.end())
	{
		this->PageTable.insert(std::make_pair(pid, this->ProcessPages));
	}
	//process has PageTable and it is not written in RAM
	if (this->PageTable.find(pid)->first >= 0 && this->PageTable.find(pid)->second[logical / 16].first == -1)
	{

		int frame = vm->getVictimFrameNumber();
		//if Page in this Frame has changed it is uploaded back to swapFile
		if (this->FrameTable->dirtyflag = 1)
		{
			VirtualMemory::Page page;
			for (int i = 0; i < 16; i++)
			{
				page.data[i] = this->ram[(frame * 16) + i];
			}
			vm->updateSwapFilePage(this->FrameTable[frame].pid, logical / 16, page);
		}
		//Page that wanted logical adress is at is being downloaded from swapFile
		VirtualMemory::Page dane = vm->getPage(pid, logical / 16);
		for (int i = 0; i < 16; i++)
		{
			this->ram[(logical % 16) + (16 * frame)] = dane.data[i];
		}
		vm->updateQueue(frame);
		this->PageTable.find(pid)->second[logical / 16].first = frame;
		return{ 0, this->ram[frame * 16 + logical % 16] };
	}
	//process has PageTable and it is written in RAM
	if (this->PageTable.find(pid)->first >= 0 && this->PageTable.find(pid)->second[logical / 16].second == 1)
	{
		return { 0,this->ram[this->PageTable.find(pid)->second[logical / 16].first * 16 + (logical % 16)] };

		vm->updateQueue(this->PageTable.find(pid)->second[logical / 16].first);
	}
	this->PageTable.find(pid)->second[logical / 16].second = 1;
	this->FrameTable[PageTable.find(pid)->second[logical / 16].first].pid = pid;

}

void Memory::printPageTable(int pid)
{

	std::cout << "PageTable of " << pid << std::endl;
	for (int i = 0; i < 8; i++)
	{
		std::cout << "{" << this->PageTable.find(pid)->second[i].first << ", " << this->PageTable.find(pid)->second[i].second << "} " << std::endl;
	}
	std::cout << std::endl;

}

void Memory::test()
{
	int8_t data[16];
	std::vector<VirtualMemory::Page> pages;
	for (int8_t i = 0; i < 8; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			data[j] = i;
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

	this->writeInMem(1, 20, 33);
	this->printMemory();

	this->printPageTable(1);

	std::pair<uint8_t, int8_t&> bajt = this->getMemoryContent(1, 20);
	std::cout << "| " << bajt.second << " |" << std::endl;
	this->getMemoryContent(2, 1);
	this->printMemory();



}