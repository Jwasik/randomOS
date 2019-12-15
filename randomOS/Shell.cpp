#include "Shell.h"

//nie pytać
#define c	32.7
#define cis	34.6
#define d	36.7
#define dis	38.9
#define e	41.2
#define f	43.7
#define fis	46.2
#define g	49
#define gis	51.9
#define a	55.0
#define ais	58.3
#define h	61.7

Shell::Shell() :defaultColor(10)
{
	system("color 0A");
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	srand(time(time_t(NULL)));


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

	system("color 0C");
	while (1)
	{
		std::string str;
		std::cout << "TYPE START TO START SYSTEM" << std::endl;
		std::cin >> str;
		if (str == "START" || str == "start")break;
		std::cin.ignore();
	}
	system("cls");

	system("color CE");
	std::thread v1(voice1);

	this->printLine("\n\n\t	           !#########       #\n\t	        !########!          ##!\n\t	     !########!               ###\n\t	  !##########                  ####\n\t	######### #####                ######\n\t	 !###!      !####!              ######\n\t	   !           #####            ######!\n\t	                 !####!         #######\n\t	                  #####       #######\n\t	                    !####!   #######!\n\t	                     ####!########\n\t         ##                   ##########\n\t       ,######!          !#############\n\t     ,#### ########################!####!\n\t   ,####'     ##################!'    #####\n\t ,####'            #######              !####!\n\t####'                                      #####\n\t~##                                          ##~\n\t", 206);
	Sleep(3000);

	system("color 0A");
	system("cls");

	std::string command = "";
	FileMenager fmanager;
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
			auto files = fmanager.ls();

			this->printLine(" DIRECTORY: \\HOME>\n", 14);

			this->printLine(" TYPE: FILENAME:             SIZE:", 14);
			for (const auto & filename : files)
			{
				this->print(" <TXT> ", 14);
				this->print(char(175), 14);
				this->print(" " + filename, 14);

				for (unsigned int i = filename.length(); i < 20; i++)std::cout << " ";
				this->printLine(fmanager.wc(filename).second, 14);
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^rm[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 3);
			fmanager.closeFile(command, 0);
			uint8_t code = fmanager.deleteFile(command);
			this->printCode(code);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^touch[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 6);
			uint8_t code = fmanager.createFile(command);
			this->printCode(code);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^cat[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 4);
			std::pair<uint8_t, std::string> code = fmanager.cat(command);
			this->printCode(code.first);
			if (code.first == 0)this->printLine(code.second, 14);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^wc[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 3);
			std::string filename = "";

			while (1)
			{
				if (command[0] == ' ')command.erase(0, 1);
				else break;
			}

			filename = command;

			std::pair<uint8_t, unsigned int> code = fmanager.wc(filename);
			if (code.first == 0)this->printLine(code.second, 14);
			else this->printCode(code.first);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^mv[ ]+[0-9a-zA-z]+[ ]+[0-9a-zA-Z]+$")))
		{
			command.erase(0, 3);
			std::string filename = "";
			std::string argument = "";

			while (1)
			{
				if (command[0] == ' ')command.erase(0, 1);
				else break;
			}
			filename = command;
			for (auto it = filename.begin(); it != filename.end(); it++)
			{
				if (*it == ' ')
				{
					filename.erase(it, filename.end());
					break;
				}
			}
			command.erase(0, filename.length());
			while (1)
			{
				if (command[0] == ' ')command.erase(0, 1);
				else break;
			}
			argument = command;

			uint8_t code = fmanager.rename(filename, argument);
			this->printCode(code);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^append[ ]+[0-9a-zA-z]+[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 7);
			std::string filename = "";
			std::string argument = "";

			while (1)
			{
				if (command[0] == ' ')command.erase(0, 1);
				else break;
			}
			filename = command;
			for (auto it = filename.begin(); it != filename.end(); it++)
			{
				if (*it == ' ')
				{
					filename.erase(it, filename.end());
					break;
				}
			}
			command.erase(0, filename.length());
			while (1)
			{
				if (command[0] == ' ')command.erase(0, 1);
				else break;
			}
			argument = command;

			for (auto & letter : argument)
			{
				uint8_t code = fmanager.append(filename, letter);
				if (code != 0)
				{
					this->printCode(code);
					break;
				}
			}
			this->printCode(0);

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^test$")))
		{
			for (int i = 0; i < 128; i++)
			{
				fmanager.append("b", 'a');
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^clear[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 6);
			std::string filename = "";

			while (1)
			{
				if (command[0] == ' ')command.erase(0, 1);
				else break;
			}
			filename = command;

			uint8_t code = fmanager.clearFile(filename);
			this->printCode(code);
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
			for(unsigned int i = 0; i < 255; i++)
			{
				std::cout << i;
				this->printLine("------------", i);
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^p fs$")))
		{


			auto names = fmanager.ls();

			std::string str;
			for (unsigned int i = 0; i < Containers::bit_vector.size();i++)
			{
				//this->print(i + '0', 14);
				if (Containers::bit_vector[i] == 0) 
				{ 
					

					std::string owner = Containers::BitVectorWithFiles[i];
					unsigned int color = 1;
					for (unsigned int j = 0; j < names.size(); j++)
					{
						if (names[j] == owner)
						{
							color = j+10;
							break;
						}
					}					
					this->print(char(178), color);
				}
				else if (Containers::bit_vector[i] == 1)
				{
					this->print(char(176), 14);
				}
			}
			std::cout << std::endl;

		}
		else
		{
			printLine("UNRECOGNISED COMMAND \"" + command + "\"\nTYPE \"MAN\" TO GET COMMAND LIST", 12);
		}
	}
	v1.join();
}

void Shell::printLine(std::string text, unsigned int color = 10)
{
	SetConsoleTextAttribute(hConsole, color);
	std::cout << text << std::endl;
	SetConsoleTextAttribute(hConsole, this->defaultColor);
}

void Shell::printLine(int text, unsigned int color)
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

void Shell::printCode(uint8_t code)
{
	SetConsoleTextAttribute(hConsole, 14);

	switch (code)
	{
	case 0:
		std::cout << "DONE" << std::endl;
		break;
	case 64:
		std::cout << "CODE 64 : ERROR_ALREADY_EXISTING_FILE" << std::endl;
		break;
	case 65:
		std::cout << "CODE 65 : ERROR_NO_SPACE_ON_DISK" << std::endl;
		break;
	case 66:
		std::cout << "CODE 66 : ERROR_NO_FILE_WITH_THAT_NAME" << std::endl;
		break;
	case 67:
		std::cout << "CODE 67 : ERROR_FILE_OPENED_BY_OTHER_PROCESS" << std::endl;
		break;
	case 68:
		std::cout << "CODE 68 : ERROR_FILE_IS_NOT_OPENED" << std::endl;
		break;
	case 69:
		std::cout << "CODE 69 : ERROR_UOT_OF_FILE_RANGE" << std::endl;
		break;
	case 70:
		std::cout << "CODE 70 : ERROR_FILE_IS_OPENED_CANNOT_DELETE" << std::endl;
		break;
	default:
		std::cout << "CODE " << int(code) << ": ERROR_UNRECOGNIZED_ERROR" << std::endl;
		break;
	}


	SetConsoleTextAttribute(hConsole, this->defaultColor);
}


void voice1()
{
	int tempo = 80;
	double quarter = 60000.0 / tempo;

	int zwrotka = 1;

	//1
	Beep(g * 8, quarter * 3);
	Beep(0, quarter / 2);
	Beep(g * 8, quarter / 2);
	while (1)
	{
		//2
		Beep(c * 16, quarter);
		Beep(g * 8, quarter*0.75);
		Beep(a * 8, quarter / 4);
		Beep(h * 8, quarter);
		Beep(e * 8, quarter / 2);
		Beep(e * 8, quarter / 2);
		//3
		Beep(a * 8, quarter);
		Beep(g * 8, quarter*0.75);
		Beep(f * 8, quarter / 4);
		Beep(g * 8, quarter);
		Beep(c * 8, quarter / 2);
		Beep(c * 8, quarter / 2);
		//4
		Beep(d * 8, quarter);
		Beep(d * 8, quarter*0.75);
		Beep(e * 8, quarter / 4);
		Beep(f * 8, quarter);
		Beep(f * 8, quarter*0.75);
		Beep(g * 8, quarter / 4);
		//5
		Beep(a * 8, quarter);
		Beep(h * 8, quarter / 2);
		Beep(c * 16, quarter / 2);
		Beep(d * 16, quarter*1.5);
		Beep(g * 8, quarter / 2);
		//6
		Beep(e * 16, quarter);
		Beep(d * 16, quarter*0.75);
		Beep(c * 16, quarter / 4);
		Beep(d * 16, quarter);
		Beep(h * 8, quarter / 2);
		Beep(g * 8, quarter / 2);
		//7
		Beep(c * 16, quarter);
		Beep(h * 8, quarter*0.75);
		Beep(a * 8, quarter / 4);
		Beep(h * 8, quarter);
		Beep(e * 8, quarter / 2);
		Beep(e * 8, quarter / 2);
		//8
		Beep(a * 8, quarter);
		Beep(g * 8, quarter*0.75);
		Beep(f * 8, quarter / 4);
		Beep(g * 8, quarter);
		Beep(c * 8, quarter*0.75);
		Beep(c * 8, quarter / 4);
		//9
		Beep(c * 16, quarter);
		Beep(h * 8, quarter*0.75);
		Beep(a * 8, quarter / 4);
		Beep(g * 8, quarter / 2);
		Beep(h * 8, quarter / 2);
		Beep(c * 16, quarter / 2);
		Beep(d * 16, quarter / 2);
		//10
		Beep(e * 16, quarter * 2);
		Beep(d * 16, quarter / 2);
		Beep(c * 16, quarter / 2);
		Beep(h * 8, quarter / 2);
		Beep(c * 16, quarter / 2);
		//11
		Beep(d * 16, quarter*1.5);
		Beep(g * 8, quarter / 2);
		Beep(g * 8, quarter / 2);
		Beep(h * 8, quarter / 2);
		Beep(c * 16, quarter / 2);
		Beep(d * 16, quarter / 2);
		//12
		Beep(c * 16, quarter * 2);
		Beep(h * 8, quarter / 2);
		Beep(a * 8, quarter / 2);
		Beep(g * 8, quarter / 2);
		Beep(a * 8, quarter / 2);
		//13
		Beep(h * 8, quarter *1.5);
		Beep(e * 8, quarter / 2);
		Beep(e * 8, quarter / 2);
		Beep(g * 8, quarter / 2);
		Beep(a * 8, quarter / 2);
		Beep(h * 8, quarter / 2);
		//14
		Beep(c * 16, quarter);
		Beep(a * 8, quarter*0.75);
		Beep(h * 8, quarter / 4);
		Beep(c * 16, quarter);
		Beep(a * 8, quarter*0.75);
		Beep(h * 8, quarter / 4);
		//15
		Beep(c * 16, quarter);
		Beep(a * 8, quarter / 2);
		Beep(c * 16, quarter / 2);
		Beep(f * 16, quarter*1.5);
		Beep(0, quarter*1.5 / 2);
		//16
		Beep(f * 16, quarter * 2);
		Beep(e * 16, quarter / 2);
		Beep(d * 16, quarter / 2);
		Beep(c * 16, quarter / 2);
		Beep(d * 16, quarter / 2);
		//17
		Beep(e * 16, quarter *1.5);
		Beep(c * 16, quarter / 2);
		Beep(c * 16, quarter * 2);
		//18
		Beep(d * 16, quarter * 2);
		Beep(c * 16, quarter / 2);
		Beep(h * 8, quarter / 2);
		Beep(a * 8, quarter / 2);
		Beep(h * 8, quarter / 2);
		//19
		Beep(c * 16, quarter *1.5);
		Beep(a * 8, quarter / 2);
		Beep(a * 8, quarter * 2);
		if (zwrotka < 2)
		{
			//20 1st volta
			Beep(c * 16, quarter);
			Beep(h * 8, quarter*0.75);
			Beep(a * 8, quarter / 4);
			Beep(g * 8, quarter);
			Beep(c * 8, quarter*1.5);
			Beep(c * 8, quarter / 4);
			//21
			Beep(c * 16, quarter);
			Beep(h * 8, quarter*0.75);
			Beep(a * 8, quarter / 4);
			Beep(g * 8, quarter);
			Beep(g * 8, quarter / 2);
			Beep(g * 8, quarter / 2);
			zwrotka++;
		}
		else
		{
			//20 2nd volta
			Beep(c * 16, quarter);
			Beep(h * 8, quarter*0.75);
			Beep(a * 8, quarter / 4);
			Beep(g * 8, quarter);
			Beep(c * 8, quarter*1.5);
			Beep(c * 8, quarter / 4);
			//21
			Beep(g * 8, quarter * 2);
			Beep(a * 8, quarter);
			Beep(h * 8, quarter);
			Beep(c * 16, quarter*2.5);
			break;
		}
	}
}