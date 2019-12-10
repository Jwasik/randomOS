#pragma once
#include "Includes.h"
#include <conio.h>
#include <stdio.h>
#include <thread>
#include "FileMenager.h"

void voice1();

class Shell
{
public:
	Shell();
	~Shell();
	void run();

private:
	void print(std::string, unsigned int);
	void print(char, unsigned int);
	void printLine(std::string, unsigned int);
	void printLine(int, unsigned int);
	void changeConsoleColor(unsigned int);
	void restoreDefaultColor();
	void toLower(std::string&);
	void printCode(uint8_t);



	std::string osName;

	HANDLE  hConsole;
	unsigned int defaultColor;
};