#pragma once
#include "Includes.h"
#include <conio.h>
#include <stdio.h>

class Shell
{
public:
	Shell();
	~Shell();
	void run();
	void print(std::string, unsigned int);
private:
	HANDLE  hConsole;
	unsigned int defaultColor;
};