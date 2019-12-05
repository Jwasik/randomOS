#pragma once
#include "Includes.h"
#include <conio.h>
#include <stdio.h>
#include "FileMenager.h"

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
	void changeConsoleColor(unsigned int);
	void restoreDefaultColor();
	void toLower(std::string&);

	std::string osName;

	HANDLE  hConsole;
	unsigned int defaultColor;
};