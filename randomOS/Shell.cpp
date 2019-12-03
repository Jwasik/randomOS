#include "Shell.h"



Shell::Shell():defaultColor(10)
{
	srand(time(NULL));
	system("color 0A");
	unsigned char systemName[7];
	for (unsigned int i = 0; i < 7; i++)
	{
		systemName[i]=(rand()%26) + 97;
		std::cout << systemName[i];
	}
	std::cout << "OS" << std::endl;
}


Shell::~Shell()
{
}

void Shell::run()
{
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::string command="";
	while (1)
	{
		std::cout << "home> ";
		std::cin >> command;
		if (std::regex_match(command.begin(), command.end(), std::regex("^poweroff$")))
		{
			return;
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^help$")))
		{
			
			print("commands", 14);
			std::cout << "poweroff" << std::endl;
			std::cout << "ls" << std::endl;
			std::cout << "fork" << std::endl;
			std::cout << "ps" << std::endl;
		}


	}
}

void Shell::print(std::string text, unsigned int color)
{
	SetConsoleTextAttribute(hConsole, color);
	std::cout << text << std::endl;
	SetConsoleTextAttribute(hConsole, this->defaultColor);
}
