#include "Shell.h"



Shell::Shell() :defaultColor(10)
{
	srand(time(NULL));
	system("color 0A");

	for (unsigned int i = 0; i < 5; i++)
	{
		this->osName += (rand() % 26) + 97;
	}
	this->osName += "OS";
	this->printLine(this->osName, 11);
}


Shell::~Shell()
{
}

void Shell::run()
{
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::string command = "";
	FileMenager f(nullptr);
	f.createFile("c");
	f.openFile("c", 5);

	while (1)
	{
		std::cout << this->osName + "\\home>";
		std::getline(std::cin, command);
		this->toLower(command);

		//GENERAL COMMANDS
		if (std::regex_match(command.begin(), command.end(), std::regex("^man$")))
		{
			printLine("\nFOR MORE INFO TYPE: COMMAND --HELP", 14);
			printLine("GENERAL COMMANDS", 13);
			std::cout << "POWEROFF   " << "- shutdown" << std::endl;
			std::cout << "CLEAR      " << "- clear console window" << std::endl;
			std::cout << "MAN        " << "- print commands list" << std::endl;
			printLine("FILE SYSTEM COMMANDS", 13);
			std::cout << "LS         " << "- print directory content" << std::endl;
			std::cout << "RM         " << "- delete file" << std::endl;
			std::cout << "TOUCH      " << "- create file" << std::endl;
			std::cout << "CAT        " << "- print file to console" << std::endl;
			std::cout << "MV         " << "- rename file" << std::endl;
			std::cout << "WC         " << "- print file character count" << std::endl;
			std::cout << "APPEND     " << "- add text to the end of file" << std::endl;
			std::cout << "CLEAR      " << "- delete file`s content" << std::endl;
			printLine("PROCESSES COMMANDS", 13);
			std::cout << "FORK       " << "- create new process" << std::endl;
			std::cout << "KILL       " << "- kill process" << std::endl;
			std::cout << "PS -w -r -a" << "- print process list" << std::endl;
			printLine("INTERPRETER COMMANDS", 13);
			std::cout << "GO         " << "- execute one ASSEMBLER command" << std::endl;
			printLine("MODULE COMMANDS", 13);
			std::cout << "PRINT RAM  " << "- print RAM content" << std::endl;
			std::cout << std::endl;

			restoreDefaultColor();
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^poweroff$")))
		{
			printLine("SHUTTING DOWN", 12);
			system("pause");
			return;
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^clean")))
		{
			system("cls");
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex(".*--help$")))
		{
			std::cout << "HELP" << std::endl;
			if (std::regex_match(command.begin(), command.end(), std::regex("^ls --help$")))
			{

			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^ls$")))
		{
			std::string str;
			for (auto & i : Containers::bit_vector)
			{
				print(i+48,12);
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^rm[ ]+[0-9a-zA-z]+$")))
		{

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^touch[ ]+[0-9a-zA-z]+$")))
		{
			f.createFile("d");
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^cat[ ]+[0-9a-zA-z]+$")))
		{

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^wc[ ]+[0-9a-zA-z]+$")))
		{

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^mv[ ]+[0-9a-zA-z]+[ ]+[0-9a-zA-Z]+$")))
		{

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^delete[ ]+[0-9a-zA-z]+$")))
		{

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^append[ ]+[0-9a-zA-z]+[ ]+[0-9a-zA-z]+$")))
		{

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^clear[ ]+[0-9a-zA-z]+$")))
		{
			std::cout << "clear" << std::endl;
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^fork[ ][a-z0-9]+[ ][a-z0-9]+$")))
		{
			std::cout << "fork" << std::endl;
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^kill[ ][0-9]+$")))
		{
			std::cout << "kill pid" << std::endl;
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^kill[ ][a-z0-9]+$")))
		{
			std::cout << "kill name" << std::endl;
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^ps$")))
		{
			std::cout << "ps" << std::endl;
		}
		else
		{
			printLine("UNRECOGNISED COMMAND \"" + command + "\"\nTYPE \"MAN\" TO GET COMMAND LIST", 12);
		}
	}
}

void Shell::printLine(std::string text, unsigned int color = 10)
{
	SetConsoleTextAttribute(hConsole, color);
	std::cout << text << std::endl;
	SetConsoleTextAttribute(hConsole, this->defaultColor);
}

void Shell::print(std::string text, unsigned int color = 10)
{
	SetConsoleTextAttribute(hConsole, color);
	std::cout << text;
	SetConsoleTextAttribute(hConsole, this->defaultColor);
}

void Shell::print(char text, unsigned int color = 10)
{
	SetConsoleTextAttribute(hConsole, color);
	std::cout << text;
	SetConsoleTextAttribute(hConsole, this->defaultColor);
}

void Shell::changeConsoleColor(unsigned int color)
{
	SetConsoleTextAttribute(hConsole, color);
}

void Shell::restoreDefaultColor()
{
	SetConsoleTextAttribute(hConsole, this->defaultColor);
}

void Shell::toLower(std::string &str)
{
	for (auto & letter : str)
	{
		if (letter >= 65 && letter <= 90)
		{
			letter += 32;
		}
	}
}
