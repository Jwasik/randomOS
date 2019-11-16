#include "Memory.h"

Memory::Memory()
{
	for (unsigned int i = 0; i < memorySize / 16; i++)
	{
		frames[i].memory = &(this->memory[16*i]);
		frames[i].owner = 0 - 1;
	}

	for (unsigned int i = 0; i < 128; i++)
	{
		memory[i] = i;
	}

	for (unsigned int i = 0; i < 8; i++)
	{
		frames[i].printPage();
	}
}

uint8_t& Page::at(int index)
{
	if (index > 15 || index <0)
	{
		//ubija proces
		//ustawia flag� procesu wykonuj�cego operacje
		return memory[0];
	}
	else return memory[index];
}

void Page::printPage()
{
	std::cout << "Page: ";
	uint8_t *pom = this->memory;//pomocnicza
	for (unsigned int i = 0; i < 16; i++)
	{
		std::cout << int(*pom) << ' ';
		pom++;
	}
	std::cout << std::endl;
}

void Memory::exchange(std::pair<uint16_t, Page> ramPage, std::pair<uint16_t, Page> exchangePage)
{
	//zamienia podan� stron� ze stron� z pliku stronnicowania
}

void Memory::receiveFreePage(std::pair<uint16_t, Page> ramPage, std::pair<uint16_t, Page> exchangePage)
{
	//zamienia podan� stron� na pust� stron� z pliku stronnicowania
}

Page& Memory::at(int index)
{
	if (/*proces jest w�a�cicielem strony*/1)
	{
		//wczytaj stron�
		//return strona
	}
	else
	{
		//ustaw b��d w PCB
	}

	Page a;//usun�� potem
	return a;//usun�� potem
}

void Memory::printMemory()
{
	for (unsigned int i = 0; i < memorySize; i++)
	{
		if (i % 16 == 0 && i!= 0)std::cout << std::endl;
		std::cout << int(memory[i])<<' ';
	}
}
