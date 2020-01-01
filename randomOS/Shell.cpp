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

Shell::Shell(std::shared_ptr<FileMenager> fm, std::shared_ptr<Memory> mm, std::shared_ptr<VirtualMemory> vm)
:defaultColor(10), fileManager(fm), memoryManager(mm), virtualMemory(vm)
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
	//Sleep(3000);

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
		else if (std::regex_match(command, match, std::regex("(.*)( )(--help)$")))
		{
			std::string helpFor = match[1];

			if (helpFor == "ls") 
			{

			}
			else if(helpFor =="fork")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  creates a new processes from the level of the console.\n", 14);

				//USAGE
				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >fork", 14);
				this->print(" [pName]",12);
				this->print(" [pSource]",11);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  pName", 12);
				this->print("     <string>", 3);
				this->print("   the name of the process.", 14);
				///usefull information, exceptions, what cannot be used
				this->print(" (cannot be longer then " + std::to_string(MAX_PROCESS_NAME_LENGHT) + " characters|cannot contain spaces)", 6);
		
				//var 2
				///variable name type and description
				this->print("\n  pSource", 11);
				this->print("   <string>", 3);
				this->print("   name of the .txt file containing the source code.", 14);
				///usefull information, exceptions, what cannot be used
				this->print(" (no file extension neccesarry|must be located in program directory)", 6);


				//example
				this->printLine("\n\n  Example", 5);
				this->print("  >fork", 14);
				this->print(" foo", 12);
				this->print(" test", 11);
				this->printLine("  (creates a new process \"foo\" from source code in test.txt)", 14);
			}
			else if (helpFor == "kill") 
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  removes a specified process and all of its children.", 14);

				//USAGE
				this->printLine("\nUSAGE: ", 13);
				//general form of the command
				this->print("  >kill", 14);
				this->print(" [pName]", 12);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  pName", 12);
				this->print("     <string>", 3);
				this->print("   the name of the process that is to be killed.", 14);

				//example
				this->printLine("\n\n  Example", 5);
				this->print("  >kill", 14);
				this->print(" foo", 12);
				this->printLine("  (kills a process called \"foo\")", 14);
			}
			else if (helpFor == "ps")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  prints currently exisitng processes. Can be modified with a parameter to show only ones of certain state.", 14);

				//USAGE
				this->printLine("\nUSAGE: ", 13);
				//general form of the command
				this->print("  >ps", 14);
				this->print(" <pState>", 12);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  pState", 12);
				this->print("    <param>", 3);
				this->print("    specifies the state the processes must have in order to be printed.", 14);

				this->print("\n    -r", 12);
				this->print(" running state.", 14);
				this->print("\n    -w", 12);
				this->print(" waiting state.", 14);
				this->print("\n    -a", 12);
				this->print(" ready state.", 14);

				//example
				this->printLine("\n\n  Examples", 5);
				this->print("  >ps", 14);
				this->printLine("  (prints a tree of all the processes)", 14);
				this->print("  >ps", 14);
				this->print(" -r", 12);
				this->printLine("  (prints only the running process)", 14);
			}
			//unrecognized help command
			else 
			{
				this->printLine("Help for this command does not exist, you can't be helped :(", 14);
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
			this->print("PROCESSES TREE", 13);
			this->printLine("\n"+processManager->displayTree(), 14);
			//stare kolorki nie wiem co to xD
		/*	std::cout << "ps" << std::endl;
			for (unsigned int i = 0; i < 255; i++)
			{
				std::cout << i;
				this->printLine("------------", i);
			}*/
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^ps[ ]-[wra]$")))
		{
			std::string temp = "";
			if(command.at(command.length()-1)=='w')
			{ 
				this->print("WAITING PROCESSES", 13);
				temp = this->processManager->displayWithState(PCB::ProcessState::WAITING);
				this->printLine(temp, 14);
				if (temp == "") { this->printLine("<No processes found>", 4); }
			}
			else if (command.at(command.length() - 1) == 'r') 
			{ 
				this->print("RUNNING PROCESS", 13);
				temp = this->processManager->displayWithState(PCB::ProcessState::RUNNING);
				this->printLine(temp, 14);
				if(temp==""){ this->printLine("<No processes found>", 4); }
			}
			else if (command.at(command.length() - 1) == 'a') 
			{
				this->print("READY PROCESSES", 13);
				temp = this->processManager->displayWithState(PCB::ProcessState::READY);
				this->printLine(temp, 14);
				if (temp == "") { this->printLine("<No processes found>", 4); }
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
							for (auto & x : Containers::Colors)
							{
								if (x.first == owner)
								{
									color = x.second + 1;
									break;
								}
							}
							
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
		else if (std::regex_match(command.begin(), command.end(), std::regex("^p vm$")))
		{
			this->printLine("DC QUEUE",14);
			this->print("FRAME NUMBER    ",13);
			this->printLine("REFERENCE BIT",13);
			for (auto & pair : virtualMemory->queue)
			{
				this->print("     ",14);
				this->print(int(pair.first),14);
				this->print("               ",14);
				this->printLine(int(pair.second),14);
			}
			std::cout << std::endl;

			this->printLine("VIRTUAL MEMORZ CONTENT", 14);
			this->print("PID   ", 13);
			this->printLine("PAGE CONTENT", 13);

			for (auto & pair : virtualMemory->swapFile)
			{
				for (auto & page : pair.second)
				{
					std::cout << " ";
					this->print(pair.first, 9);

					std::cout << "    ";
					for (unsigned int i = 0; i < 16; i++)
					{
						this->print(page.data[i],14);
						this->print(" ", 14);
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^p ram$")))
		{
			this->printLine("RAM CONTENT", 14);
			this->print("FRAME NUMBER    ", 13);
			this->printLine("CONTENT", 13);
			for (unsigned int i = 0; i < 8; i++)
			{
				this->print("     ",9);
				this->print(int(i),9);
				std::cout << "          ";
				for (unsigned int j = 0; j < 16; j++)
				{
					this->print(memoryManager->ram[i+j],14);
					this->print(" ", 14);
				}
				std::cout << std::endl;
			}
			
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^test ram$")))
		{
			std::pair<uint8_t,int8_t&> t = memoryManager->getMemoryContent(0,0);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^test vm$")))
		{
			//insert test program
			VirtualMemory::Page testPage;
			int8_t data[16]{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
			std::vector<VirtualMemory::Page> testVector;
			testVector.push_back(VirtualMemory::Page(data));
			data[11] = 127;
			testVector.push_back(VirtualMemory::Page(data));
			std::pair<int, std::vector<VirtualMemory::Page>> testPair;
			testPair.first = 0;
			testPair.second = testVector;
			virtualMemory->insertProgram(testPair);
			testPair.first = 1;
			virtualMemory->insertProgram(testPair);
			//end test program
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

void Shell::print(int text, unsigned int color = 10)
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
	SetConsoleTextAttribute(hConsole, 12);

	switch (code)
	{
	case 0:
		std::cout << "DONE" << std::endl;
	break;
	case 32:
		std::cout << "CODE 32 : ERROR_PM_PROCESS_NAME_TAKEN" << std::endl;
	break;
	case 33:
		std::cout << "CODE 33 : ERROR_PM_PROCESS_NAME_TOO_LONG" << std::endl;
	break;
	case 34:
		std::cout << "CODE 34 : ERROR_PM_PROCESS_NAME_CANNOT_BE_EMPTY" << std::endl;
	break;
	case 35:
		std::cout << "CODE 35 : ERROR_PM_PROCESS_NAME_CONTAINS_UNALLOWED_CHARACTERS" << std::endl;
	break;
	case 36:
		std::cout << "CODE 36 : ERROR_PM_PARENT_COULD_NOT_BE_FOUND" << std::endl;
	break;
	case 37:
		std::cout << "CODE 37 : ERROR_PM_INIT_CANNOT_BE_DELETED" << std::endl;
	break;
	case 38:
		std::cout << "CODE 38 : ERROR_PM_PROCESS_COULD_NOT_BE_FOUND" << std::endl;
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
	case 81:
		std::cout << "CODE 81 : ERROR_PAGE_DOESNT_EXIST" << std::endl;
		break;
	case 200:
		std::cout << "CODE 200 : ERROR_UNKNOWN_INSTRUCTION" << std::endl;
		break;
	case 201:
		std::cout << "CODE 201 : ERROR_NUMBER_TO_LARGE" << std::endl;
		break;
	case 202:
		std::cout << "CODE 202 : ERROR_NUMBER_TO_SMALL" << std::endl;
		break;
	case 203:
		std::cout << "CODE 203 : ERROR_DIVIDING_BY_ZERO" << std::endl;
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