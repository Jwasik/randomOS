#include "MemoryManager.h"


VirtualMemory *vm = new VirtualMemory();

VirtualMemory  Memory::getVM()
{
	return *vm;
}


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
	
		int frame = getVM().getVictimFrameNumber();
		this->FrameTable[frame];

		if (FrameTable[frame].dirtyflag == true)
		{
			VirtualMemory::Page page;
			for (int i = 0; i < 16; i++)
			{
				page.data[i] = ram[frame*16+i%16];
			}
			getVM().updateSwapFilePage(pid, logical /16, page);
		}


		for (int i = 0; i < 16; i++)
		{
			int8_t data = getVM().getPage(pid, logical / 16).data[logical % 16];
			this->ram[logical % 16 + 16 * frame] = data;
		}
		this->FrameTable[frame].dirtyflag = true;
		getVM().updateQueue(frame);
		this->FrameTable[frame].lastUse++;
		this->FrameTable[frame].ProcessPageNr = logical / 16;
		this->FrameTable[frame].pid = pid;
		return ram[frame * 16 + logical % 16];
}

void Memory::test()
{
	int8_t data[16] = { 0 };
	std::vector<VirtualMemory::Page> pages;
	for (int8_t i = 0; i < 3; i++) {
		for (int8_t &j : data) {
			j = i;
		}
		pages.emplace_back(VirtualMemory::Page(data));
	}

	this->getVM().insertProgram(std::make_pair(1, pages));
	this->getVM().insertProgram(std::make_pair(2, pages));
	this->getVM().printSwapFile();
	std::cout << std::endl << std::endl;
	this->getVM().getPage(1, 0);
	this->getMemoryContent(1, 0);
	this->printMemory();
	this->getVM().getPage(2, 0);
	this->getMemoryContent(2, 0);
	this->printMemory();
}


