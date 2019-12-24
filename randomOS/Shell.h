#pragma once
#include "Includes.h"
#include <conio.h>
#include <stdio.h>
#include <thread>
#include "FileMenager.h"
#include "MemoryManager.h"
#include "VirtualMemory.h"
#include "Page.h"

void voice1();

class Shell
{
public:
	Shell();
	Shell(std::shared_ptr<FileMenager> fileMenager,std::shared_ptr<Memory> memoryManager,std::shared_ptr<VirtualMemory> virtualMemory);
	~Shell();
	void run();

private:
	std::shared_ptr<FileMenager> fileManager;
	std::shared_ptr<Memory> memoryManager;
	std::shared_ptr<VirtualMemory> virtualMemory;

	void print(std::string, unsigned int);
	void print(char, unsigned int);
	void print(int, unsigned int);
	void printLine(std::string, unsigned int);
	void printLine(int, unsigned int);
	void changeConsoleColor(unsigned int);
	void restoreDefaultColor();
	void toLower(std::string&);
	void printCode(uint8_t);



	std::string osName;

	HANDLE hConsole;
	unsigned int defaultColor;
};