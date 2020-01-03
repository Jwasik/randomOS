#pragma once
#include "Includes.h"
#include <conio.h>
#include <stdio.h>
#include <thread>
#include "FileMenager.h"
#include "Scheduler.h"
#include "ProcessManager.h"
#include "MemoryManager.h"
#include "VirtualMemory.h"
#include "Page.h"

class Shell
{
public:
	Shell();
	Shell(std::shared_ptr<FileMenager>, std::shared_ptr<Memory>, std::shared_ptr<VirtualMemory>, std::shared_ptr<ProcessManager>, std::shared_ptr<Scheduler>);
	~Shell();
	void run();

private:
	std::shared_ptr<FileMenager> fileManager;
	std::shared_ptr<Memory> memoryManager;
	std::shared_ptr<VirtualMemory> virtualMemory;
	std::shared_ptr<ProcessManager> processManager;
	std::shared_ptr<Scheduler> scheduler;

	template <typename T>	void print(T, unsigned int);
	template<typename I> std::string toHexString(I w);
	template <typename T>	void printLine(T, unsigned int);
	template<typename T>    void printWithPadding(T text, unsigned int color, unsigned int spaces);
	void printProcessInformation(std::shared_ptr<PCB>);
	void changeConsoleColor(unsigned int);
	void restoreDefaultColor();
	void toLower(std::string&);
	void printCode(uint8_t);



	std::string osName;

	HANDLE hConsole;
	unsigned int defaultColor;
};