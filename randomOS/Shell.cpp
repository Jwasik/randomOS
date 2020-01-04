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

Shell::Shell(std::shared_ptr<FileMenager> fm, std::shared_ptr<Memory> mm, std::shared_ptr<VirtualMemory> vm, std::shared_ptr<ProcessManager> pm, std::shared_ptr<Scheduler> sch,std::shared_ptr<Interpreter> inte)
	:defaultColor(10), fileManager(fm), memoryManager(mm), virtualMemory(vm), processManager(pm), scheduler(sch), interpreter(inte)
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
	while (1)
	{
		std::cout << this->osName + "\\home>";
		std::getline(std::cin, command);
		this->toLower(command);

		//initialize variable used to store regex matches for further parsing
		std::smatch match;

		//GENERAL COMMANDS
		if (std::regex_match(command.begin(), command.end(), std::regex("^man$")))
		{
			printLine("\nFOR MORE INFO TYPE: COMMAND --HELP", 14);
			printLine("GENERAL COMMANDS", 13);
			std::cout << "POWEROFF   " << "- shutdown" << std::endl;
			std::cout << "CLEAN      " << "- clear console window" << std::endl;
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
			std::cout << "PROC       " << "- print process' information" << std::endl;
			printLine("INTERPRETER COMMANDS", 13);
			std::cout << "GO         " << "- execute one ASSEMBLER command" << std::endl;
			printLine("MODULE COMMANDS", 13);
			std::cout << "P RAM " << "- print RAM content" << std::endl;
			std::cout << "P VM " << "- print vMemory content" << std::endl;
			std::cout << "P SCH " << "- print Scheduler state" << std::endl;
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
				this->printLine("\n  Name      Type       Description                                         Details", 5);

				//var 1
				///variable name and what it is
				this->print("  pName", 12);
				this->print("     <string>", 3);
				this->print("   the name of the process.", 14);
				///usefull information, exceptions, what cannot be used
				this->print("                            no longer then " + std::to_string(MAX_PROCESS_NAME_LENGHT) + " chars, no spaces, at least one letter.", 6);
		
				//var 2
				///variable name type and description
				this->print("\n  pSource", 11);
				this->print("   <string>", 3);
				this->print("   name of the .txt file containing the source code.", 14);
				this->print("   no file extension neccesarry, always assumed to be .txt.", 6);
				///usefull information, exceptions, what cannot be used
				this->print("\n    \"filename\"", 11);
				this->print("   for file in project directory.", 14);
				this->print("\n    \"C:\\dir\\\"filename\"", 11);
				this->print("   for specifing different path", 14);
			


				//example
				this->printLine("\n\n  Examples", 5);
				this->print("  >fork", 14);
				this->print(" foo", 12);
				this->print(" test", 11);
				this->printLine("  (creates a new process \"foo\" from source code in test.txt in default dir)", 14);
				this->print("  >fork", 14);
				this->print(" foo", 12);
				this->print(" C:\\Bar\\test", 11);
				this->printLine("  (creates a new process \"foo\" from source code in test.txt located in specified directory)", 14);
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
				this->print("\n            <int>", 3);
				this->print("      the PID of the process that is to be killed.", 14);

				//example
				this->printLine("\n\n  Example", 5);
				this->print("  >kill", 14);
				this->print(" foo", 12);
				this->printLine("  (kills a process called \"foo\")", 14);
			}
			else if (helpFor == "proc")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  get detailed information about a process.", 14);

				//USAGE
				this->printLine("\nUSAGE: ", 13);
				//general form of the command
				this->print("  >proc", 14);
				this->print(" [pName]", 12);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  pName", 12);
				this->print("     <string>", 3);
				this->print("   the name of the process that is to be displayed.", 14);
				this->print("\n            <int>", 3);
				this->print("      the PID of the process that is to be displayed.", 14);

				//example
				this->printLine("\n\n  Examples", 5);
				this->print("  >proc", 14);
				this->print(" foo", 12);
				this->printLine("  (displays a process called \"foo\")", 14);
				this->print("  >proc", 14);
				this->print(" 10", 12);
				this->printLine("  (displays a process with PID 10)", 14);
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
				this->printLine("Help for this command does not exist.", 4);
			}

		}
		else if (std::regex_match(command, match, std::regex("^(proc)([ ])([0-9]+)$")))
		{
			//print info with pid
			std::shared_ptr<PCB> temp= processManager->getPCBByPID(std::stoi(match[3]));
			if (temp == nullptr) { this->printLine("AN ERROR OCCURED!", 4); this->printCode(38); }
			else
			{
				this->print("PROCESS INFORMATION ", 13);
				printProcessInformation(temp);
			}
			}
		else if (std::regex_match(command, match, std::regex("^(proc)([ ])([a-z0-9]+)$")))
		{
			//print info with name
			std::shared_ptr<PCB> temp = processManager->getPCBByName(match[3]);
			if (temp == nullptr) { this->printLine("AN ERROR OCCURED!", 4); this->printCode(38); }
			else
			{
				this->print("PROCESS INFORMATION ", 13);
				printProcessInformation(temp);
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^ls$")))
		{
			auto files = fileManager->ls();

			this->printLine(" DIRECTORY: \\HOME>\n", 14);

			this->printLine(" TYPE: FILENAME:             SIZE:", 14);
			for (const auto& filename : files)
			{
				this->print(" <TXT> ", 14);
				this->print(char(175), 14);
				this->print(" " + filename, 14);

				for (unsigned int i = filename.length(); i < 20; i++)std::cout << " ";
				this->printLine(fileManager->wc(filename).second, 14);
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^rm[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 3);
			fileManager->closeFile(command, 0);
			uint8_t code = fileManager->deleteFile(command);
			this->printCode(code);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^touch[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 6);
			uint8_t code = fileManager->createFile(command);
			this->printCode(code);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^cat[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 4);
			std::pair<uint8_t, std::string> code = fileManager->cat(command);
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

			std::pair<uint8_t, unsigned int> code = fileManager->wc(filename);
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

			uint8_t code = fileManager->rename(filename, argument);
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
				uint8_t code = fileManager->append(filename, letter);
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
				fileManager->append("b", 'a');
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

			uint8_t code = fileManager->clearFile(filename);
			this->printCode(code);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^fork[ ]+[a-z0-9]+[ ]+.+$")))
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
		else if (std::regex_match(command,match, std::regex("^(kill)([ ])([0-9]+)$")))
		{
			//delete with pid
			uint8_t errorCode= processManager->deleteProcess(std::stoi(match[3]));
			if(errorCode!=0) { this->printLine("AN ERROR OCCURED!", 4); this->printCode(errorCode); }
			else
			{
				this->print("Process ", 14);
				this->print("[PID: "+match[3].str()+"]", 11);
				this->printLine(" has been deleted,", 14);
			}
		}
		else if (std::regex_match(command, match, std::regex("^(kill)([ ])([a-z0-9]+)$")))
		{
			//delete with name
			uint8_t errorCode = processManager->deleteProcess(match[3]);
			if (errorCode != 0) { this->printLine("AN ERROR OCCURED!", 4); this->printCode(errorCode); }
			else
			{
				this->print("Process ", 14);
				this->print("[" + match[3].str() + "]", 11);
				this->printLine(" has been deleted.", 14);
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("go")))
		{
			uint8_t errorCode = this->scheduler->schedule();
			if (errorCode != 0) { this->printLine("AN ERROR OCCURED!", 4); this->printCode(errorCode); }
			
			errorCode = this->interpreter->go();
			if (errorCode != 0) { this->printLine("AN ERROR OCCURED!", 4); this->printCode(errorCode); }
			else
			{
			this->printLine("One instruction completed.", 14);
			}

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^ps$")))
		{
			this->print("PROCESSES TREE", 13);
			this->printWithPadding("\n"+processManager->displayTree(), 14,2);
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
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^p fs$")))
		{
			auto names = fileManager->ls();

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
					std::string temp = std::to_string(memoryManager->ram[(PAGE_SIZE*i)+j]);
					this->print(temp, 14);
					int spaceNumber = 4 - temp.length();
					for (int z = 0; z < spaceNumber; z++) { print(" ", 1);}
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
			this->print("SCHEDULER COUNTER: ", 13);
			this->print(std::to_string(scheduler->counter), 11);
			std::cout << std::endl;

			this->print("CURRENTLY RUNNING: ", 13);
			if(RUNNING != NULL)
			{
				this->print(RUNNING->getName(), 14);
				this->print(" [PID= ", 14);
				this->print(RUNNING->getPID(), 11);
				this->print("]",14);
				this->print(" (Until Counter= ", 14);
				this->print(RUNNING->counter, 11);
				this->print(")", 14);
			}
			else { this->print("<none>", 4); }
			std::cout << std::endl;
			std::cout << std::endl;
			this->printLine("QUEUES ", 13);

			this->print("No.  ", 11);
			this->print("ACTIVE [PRIORITY]", 10);
			this->print("    ", 10);
			this->print("EXPIRED", 12);
			std::cout << std::endl;

			//check which has more elements
			int maxSize = scheduler->active->size();
			if (scheduler->expired->size() > maxSize) { maxSize = scheduler->expired->size(); }

			for (int i = 0; i < maxSize; i++)
			{

				//print Number first
				this->print(" " + std::to_string(i+1), 3);

				unsigned int spaceDelay = 20;

				//position the elements in second row
				this->print("   ", 1);

				//PRINT ACTIVE
				if (i < scheduler->active->size())
				{
					this->print((*scheduler->active)[i]->getName(), 2);
					this->print(" [", 2);
					this->print(std::to_string((*scheduler->active)[i]->priority),10 );
					this->print( "]", 2);
					//prepare spaces between the seconds and third row
					spaceDelay -= ((*scheduler->active)[i]->getName().length() + 5);
				}
				//if there are no elements in this collumn at all
				if (i ==0 && scheduler->active->size() == 0) { this->print("<none>", 2); spaceDelay -= 6; }

				//position the elements in third row
				for (unsigned int j = 0; j < spaceDelay; j++)this->print(" ", 6);

				//PRINT EXPIRED
				if (i < scheduler->expired->size())
				{
					this->print((*scheduler->expired)[i]->getName(), 4);	
				}
				//if there are no elements in this collumn at all
				if (i ==0 && scheduler->expired->size() == 0) { this->print("<none>", 4);}
	
				//go to next line
				std::cout << std::endl;
			}

			if(maxSize ==0)
			{ 
				this->print("     <none>", 2); 
				for (unsigned int j = 0; j < 14; j++)this->print(" ", 6);
				this->print("<none>", 4);
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
void Shell::printWithPadding(T str, unsigned int color, unsigned int spaces)
{
	SetConsoleTextAttribute(hConsole, color);

	//split string by newline 
	std::vector<std::string> strings;
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = str.find('\n', prev)) != std::string::npos)
	{
		strings.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}
	strings.push_back(str.substr(prev));

	//generate the padding spaces
	std::string padding = "";
	for (int i = 0; i < spaces; i++) { padding.append(" "); }

	//output with padding added to each line
	for (auto s : strings) 
	{
		std::cout << padding + s << std::endl;
	}

	SetConsoleTextAttribute(hConsole, this->defaultColor);
}

template <typename T>
void Shell::print(T text, unsigned int color)
{
	SetConsoleTextAttribute(hConsole, color);
	std::cout << text;
	SetConsoleTextAttribute(hConsole, this->defaultColor);
}

void Shell::printProcessInformation(std::shared_ptr<PCB> PCB)
{
	print("\n  Name: ", 14);
	printLine(PCB->getName(), 3);

	print("  PID: ", 14);
	printLine(PCB->getPID(), 3);

	print("  State: ", 14);
	printLine(PCB->getStateAsString(), 3);

	print("  Parent: ", 14);
	printLine(PCB->getParentAsString(), 3);

	print("  Children: ", 14);
	printLine(PCB->getChildrenAsString(), 11);

	print("  IC: ", 14);
	printLine(PCB->getInstructionCounter(), 3);

	print("  Registers: ", 14); 
	print("[",14);
	print(std::to_string(PCB->getRegisterA()), 3);
	print("]", 14);
	print("[", 14);
	print(std::to_string(PCB->getRegisterB()), 3);
	print("]", 14);
	print("[", 14);
	print(std::to_string(PCB->getRegisterC()), 3);
	print("]", 14);
	print("[", 14);
	print(std::to_string(PCB->getRegisterD()), 3);
	print("]", 14);
	std::cout << std::endl;
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
	case 16:
		std::cout << "CODE 16 : ERROR_SH_PRIORITY_OUT_OF_RANGE" << std::endl;
		break;
	case 17:
		std::cout << "CODE 17 : ERROR_SH_ADDED_PROCESS_DOES_NOT_EXIST" << std::endl;
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
	case 42:
		std::cout << "CODE 41 : ERROR_PM_PROCESS_NAME_HAS_TO_CONTAIN_AT_LEAST_ONE_LETTER" << std::endl;
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

	I temp = abs(w);
	size_t hex_len = sizeof(I) << 1;
	static const char* digits = "0123456789ABCDEF";
	std::string rc(hex_len, '0');
	for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
		rc[i] = digits[(temp >> j) & 0x0f];

	if (w < 0) { return "-" + rc; }
	return "0"+rc;
}
