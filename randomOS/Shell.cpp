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

	srand(time(NULL));


	for (unsigned int i = 0; i < 5; i++)
	{
		this->osName += (rand() % 26) + 97;
	}
	this->osName += "OS";
	this->printLine(this->osName, 11);
}

Shell::Shell(std::shared_ptr<FileMenager> fm, std::shared_ptr<Memory> mm, std::shared_ptr<VirtualMemory> vm, std::shared_ptr<ProcessManager> pm, std::shared_ptr<Scheduler> sch)
	:defaultColor(10), fileManager(fm), memoryManager(mm), virtualMemory(vm), processManager(pm), scheduler(sch)
{
	system("color 0A");
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	srand(time(NULL));


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
			for (const auto& filename : files)
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

			for (auto& letter : argument)
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
		else if (std::regex_match(command.begin(), command.end(), std::regex("^fork[ ]+[a-z0-9]+[ ]+[a-z0-9]+$")))
		{
			command.erase(0, 5);
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
			argument = command+".txt";

			std::pair<uint8_t, unsigned int> errorCode = this->processManager->fork(filename, 0, argument);

			if (errorCode.first != 0){ this->printLine("AN ERROR OCCURED!",4); this->printCode(errorCode.first);}
			else
			{
				this->print("New process created with PID = ", 14);
				this->printLine(errorCode.second, 3);
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^kill[ ][0-9]+$")))
		{
			std::cout << "kill pid" << std::endl;
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^kill[ ][a-z0-9]+$")))
		{
			std::cout << "kill name" << std::endl;
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("go")))
		{
			this->scheduler->schedule();
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^ps$")))
		{
		
			this->printLine(processManager->displayTree(), 14);
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
			if(command.at(command.length()-1)=='w')
			{ 
				this->print("WAITING PROCESSES", 6);
				this->printLine(this->processManager->displayWithState(PCB::ProcessState::WAITING), 14);
			}
			else if (command.at(command.length() - 1) == 'r') 
			{ 
				this->print("RUNNING PROCESSES", 6);
				this->printLine(this->processManager->displayWithState(PCB::ProcessState::RUNNING), 14);
			}
			else if (command.at(command.length() - 1) == 'a') 
			{
				this->print("READY PROCESSES", 6);
				this->printLine(this->processManager->displayWithState(PCB::ProcessState::READY), 14);
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^p fs$")))
		{
			auto names = fmanager.ls();

			std::string str;
			for (unsigned int i = 0; i < Containers::bit_vector.size(); i++)
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
							for (auto& x : Containers::Colors)
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
			this->printLine("DC QUEUE", 14);
			this->print("FRAME NUMBER    ", 13);
			this->printLine("REFERENCE BIT", 13);
			for (auto& pair : virtualMemory->queue)
			{
				this->print("     ", 14);
				this->print(int(pair.first), 14);
				this->print("               ", 14);
				this->printLine(int(pair.second), 14);
			}
			std::cout << std::endl;

			this->printLine("VIRTUAL MEMORY CONTENT", 14);
			this->print("PID   ", 13);
			this->printLine("PAGE CONTENT", 13);

			for (auto& pair : virtualMemory->swapFile)
			{
				for (auto& page : pair.second)
				{
					std::cout << " ";
					this->print(pair.first, 9);

					std::cout << "    ";
					for (unsigned int i = 0; i < 16; i++)
					{
						//print as hexa
						this->print(toHexString(page.data[i]), 14);

						////print as decimal
						//this->print(std::to_string(page.data[i]), 14);
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
				this->print("     ", 9);
				this->print(int(i), 9);
				std::cout << "          ";
				for (unsigned int j = 0; j < 16; j++)
				{
					this->print(memoryManager->ram[i + j], 14);
					this->print(" ", 14);
				}
				std::cout << std::endl;
			}

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^p proc$")))
		{
			this->printLine(processManager->displayTree(), 14);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^p sch")))
		{
		this->print("ACTIVE",6);
		this->print("              ",6);
		this->print("EXPIRED",12);
			for (unsigned int i = 0; i < 1000; i++)//color 6 i 12
			{
				
				if (scheduler->active->size() < i && scheduler->expired->size() < i)break;
				
				unsigned int spaceDelay = 20;
				if (scheduler->active->size() > i)
				{
					this->print((*scheduler->active)[i]->getName(), 6);
					this->print(" ", 6);
					this->print((*scheduler->active)[i]->getPID(), 6);
					spaceDelay -= (*scheduler->active)[i]->getName().length();
				}

				for (unsigned int j = 0; j < spaceDelay; j++)this->print(" ", 6);

				if (scheduler->expired->size() > i)
				{
					this->print((*scheduler->expired)[i]->getName(), 6);
					this->print(" ", 6);
					this->print((*scheduler->expired)[i]->getPID(), 6);
				}
				std::cout << std::endl;
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^test ram$")))
		{
			std::pair<uint8_t, int8_t&> t = memoryManager->getMemoryContent(0, 0);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^test vm$")))
		{
			//insert test program
			Page testPage;
			int8_t data[16]{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
			std::vector<Page> testVector;
			testVector.push_back(Page(data));
			data[11] = 127;
			testVector.push_back(Page(data));
			std::pair<int, std::vector<Page>> testPair;
			testPair.first = 0;
			testPair.second = testVector;
			virtualMemory->insertProgram(testPair);
			testPair.first = 1;
			virtualMemory->insertProgram(testPair);
			//end test program
		}
		else
		{
		this->printLine("UNRECOGNISED COMMAND",4);
		}
	}
}

template <typename T>
void Shell::printLine(T text, unsigned int color)
{
	SetConsoleTextAttribute(hConsole, color);
	std::cout << text << std::endl;
	SetConsoleTextAttribute(hConsole, this->defaultColor);
}

template <typename T>
void Shell::print(T text, unsigned int color)
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

void Shell::toLower(std::string& str)
{
	for (auto& letter : str)
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
		std::cout << "ALLES GING BESSER ALS ERWARTET" << std::endl;
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
	case 40:
		std::cout << "CODE 40 : ERROR_PM_CANNOT_OPEN_SOURCE_CODE_FILE" << std::endl;
		break;
	case 41:
		std::cout<< "CODE 41 : ERROR_PM_CODE_DOESNT_FIT_INTO_NUMBER_OF_DECLARED_PAGES" << std::endl;
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

template <typename I> std::string Shell::toHexString(I w) {
	size_t hex_len = sizeof(I) << 1;
	static const char* digits = "0123456789ABCDEF";
	std::string rc(hex_len, '0');
	for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
		rc[i] = digits[(w >> j) & 0x0f];
	return rc;
}