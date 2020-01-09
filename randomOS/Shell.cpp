#include "Shell.h"

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
}

Shell::Shell(std::shared_ptr<FileMenager> fm, std::shared_ptr<Memory> mm, std::shared_ptr<VirtualMemory> vm, std::shared_ptr<ProcessManager> pm, std::shared_ptr<Scheduler> sch, std::shared_ptr<Interpreter> inte)
	:defaultColor(10), fileManager(fm), memoryManager(mm), virtualMemory(vm), processManager(pm), scheduler(sch), interpreter(inte)
{
	goCounter = 0;
	system("color 0A");
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	srand(time(NULL));


	for (unsigned int i = 0; i < 5; i++)
	{
		this->osName += (rand() % 26) + 97;
	}
	this->osName += "OS";
}


Shell::~Shell()
{
}

void Shell::run()
{
	system("color 0A");
	Sleep(1000);

	PlaySound(L"sound.wav", NULL, SND_ASYNC);

	Sleep(200);
	this->printLogo();

	this->printLine("\n\n\t\t\t\t\t\t\tLOADING",10);
	std::cout << "\t\t";
	for (unsigned int i = 0; i < 84;i++)this->print(char(176), 8);
	std::cout << "\r\t\t";
	Sleep(200);
	for (unsigned int i = 0; i < 84; i++)
	{
		this->print(char(178), 10);
		Sleep(2500/84);
	}

	Sleep(500 / 84);
	system("cls");
	this->printLogo();
	std::cout << std::endl;
	std::cout << std::endl;
		

	std::string command = "";
	while (1)
	{
		std::cout << this->osName + "\\home>";
		std::getline(std::cin, command);
		this->toLower(command);

		//remove trailing spaces
		command = std::regex_replace(command, std::regex(" +$"), "");

		//initialize variable used to store regex matches for further parsing
		std::smatch match;

		//GENERAL COMMANDS
		if (std::regex_match(command.begin(), command.end(), std::regex("^man$")))
		{
			print("FOR MORE INFO TYPE: \"", 14);
			print("<COMMAND>", 9);
			printLine(" --HELP\"", 14);
			printLine("GENERAL COMMANDS", 13);
			print("  POWEROFF    ", 9); printLine("- shutdown", 14);
			print("  CLEAN       ", 9); printLine("- clear console window", 14);
			print("  MAN         ", 9); printLine("- print a list of commands", 14);
			printLine("FILE SYSTEM COMMANDS", 13);
			print("  LS          ", 9); printLine("- print directory content", 14);
			print("  RM          ", 9); printLine("- delete a file", 14);
			print("  TOUCH       ", 9); printLine("- create a file", 14);
			print("  CAT ", 9); print("-h      ", 11); printLine("- print a file to console", 14);
			print("  MV          ", 9); printLine("- rename a file", 14);
			print("  WC          ", 9); printLine("- print a file character count", 14);
			print("  APPEND      ", 9); printLine("- add text to the end of a file", 14);
			print("  CLEAR       ", 9); printLine("- delete file's content", 14);
			printLine("PROCESSES COMMANDS", 13);
			print("  FORK        ", 9); printLine("- create a new process", 14);
			print("  KILL        ", 9); printLine("- delete a process", 14);
			print("  PS", 9); print(" -w -r -a ", 11); printLine("- print a list of processes", 14);
			print("  PROC        ", 9); printLine("- print process' details", 14);
			printLine("INTERPRETER COMMANDS", 13);
			print("  GO          ", 9); printLine("- execute one ASSEMBLER command", 14);
			printLine("MODULE COMMANDS", 13);
			print("  P RAM", 9); print(" -d -h ", 11); printLine("- print RAM content", 14);
			print("  P VM", 9); print("  -d -h ", 11); printLine("- print vMemory content", 14);
			print("  P FS", 9); print("  -d -h ", 11); printLine("- print File System content", 14);
			print("  P SCH       ", 9); printLine("- print Scheduler state", 14);
			print("  P SEM       ", 9); printLine("- print Semaphores states", 14);
			print("  P IN        ", 9); printLine("- print INode state", 14);
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
				this->printLine("DESCRIPTION: ", 13);
				this->print("  lists all files in main catalog.\n", 14);

				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >ls\n", 14);
			}
			else if (helpFor == "rm")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  deletes a specified file.\n", 14);

				//USAGE
				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >rm", 14);
				this->print(" [fName]", 12);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  fName", 12);
				this->print("     <string>", 3);
				this->print("   the name of the file.", 14);
				///usefull information, exceptions, what cannot be used


				//example
				this->printLine("\n\n  Examples", 5);
				this->print("  >rm", 14);
				this->print(" foo", 12);
				this->printLine("  (deletes file \"foo\")", 14);
			}
			else if (helpFor == "touch")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  creates an empty file.\n", 14);

				//USAGE
				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >touch", 14);
				this->print(" [fName]", 12);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description                                         Details", 5);

				//var 1
				///variable name and what it is
				this->print("  fName", 12);
				this->print("     <string>", 3);
				this->print("   the name of the file.", 14);
				///usefull information, exceptions, what cannot be used
				///usefull information, exceptions, what cannot be used
				this->print("                               must be " + std::to_string(FileNameLenght) + " chars long, case sensitive.", 6);

				//example
				this->printLine("\n\n  Examples", 5);
				this->print("  >touch", 14);
				this->print(" foo", 12);
				this->printLine("  (creates file \"foo\")", 14);
			}
			else if (helpFor == "cat")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  prints file to the console.\n", 14);

				//USAGE
				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >cat", 14);
				this->print(" [fName]", 12);
				this->print(" <fState>", 11);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  fName", 12);
				this->print("     <string>", 3);
				this->print("   the name of the file.\n", 14);
				//var 2
				///variable name and what it is
				this->print("  fState", 11);
				this->print("    <param>", 3);
				this->print("    specifies output type.", 14);
				this->print("\n    -h", 11);
				this->print(" printout character hex codes.", 14);
				///usefull information, exceptions, what cannot be used

				//example
				this->printLine("\n\n  Examples", 5);
				this->print("  >cat", 14);
				this->print(" foo", 12);
				this->printLine("  (prints file \"foo\")", 14);

				this->print("  >cat", 14);
				this->print(" foo", 12);
				this->print(" -h", 11);
				this->printLine("  (prints file \"foo\" as hex numbers)", 14);
			}
			else if (helpFor == "mv")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  renames file.\n", 14);

				//USAGE
				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >mv", 14);
				this->print(" [fName]", 12);
				this->print(" [fName2]", 11);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  fName", 12);
				this->print("     <string>", 3);
				this->print("   the name of the file before renaming.\n", 14);
				//var 2
				///variable name and what it is
				this->print("  fName2", 11);
				this->print("    <string>", 3);
				this->print("   the name of the file after renaming.\n", 14);
				///usefull information, exceptions, what cannot be used

				//example
				this->printLine("\n\n  Examples", 5);
				this->print("  >mv", 14);
				this->print(" foo", 12);
				this->print(" example", 11);
				this->printLine("  (renames file \"foo\" to \"example\")", 14);
			}
			else if (helpFor == "wc")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  counts characters in file.\n", 14);

				//USAGE
				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >wc", 14);
				this->print(" [fName]", 12);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  fName", 12);
				this->print("     <string>", 3);
				this->print("   the name of the file.", 14);
				///usefull information, exceptions, what cannot be used

				//example
				this->printLine("\n\n  Examples", 5);
				this->print("  >cat", 14);
				this->print(" foo", 12);
				this->printLine("  (counts characters in file \"foo\")", 14);
			}
			else if (helpFor == "append")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  appends text to end of file.\n", 14);

				//USAGE
				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >append", 14);
				this->print(" [fName]", 12);
				this->print(" [text]", 11);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description                                         Details", 5);

				//var 1
				///variable name and what it is
				this->print("  fName", 12);
				this->print("    <string>", 3);
				this->print("   the name of the file.\n", 14);
				//var 2
				///variable name and what it is
				this->print("  text", 11);
				this->print("     <string>", 3);
				this->print("    the name of the file after renaming.", 14);
				this->print("                can contain only letters and digits", 6);

				///usefull information, exceptions, what cannot be used

				//example
				this->printLine("\n\n  Examples", 5);
				this->print("  >append", 14);
				this->print(" foo", 12);
				this->print(" abcd", 11);
				this->printLine("  (appends \"abcd\" to file \"foo\")", 14);
			}
			else if (helpFor == "clear")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  deletes content of file.\n", 14);

				//USAGE
				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >clear", 14);
				this->print(" [fName]", 12);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  fName", 12);
				this->print("     <string>", 3);
				this->print("   the name of the file.", 14);
				///usefull information, exceptions, what cannot be used

				//example
				this->printLine("\n\n  Examples", 5);
				this->print("  >clear", 14);
				this->print(" foo", 12);
				this->printLine("  (deletes content of file \"foo\")", 14);
			}
			else if (helpFor == "fork")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  creates a new processes from the level of the console.\n", 14);

				//USAGE
				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >fork", 14);
				this->print(" [pName]", 12);
				this->print(" [pSource]", 11);
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
				this->print(" [pQual]", 12);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  pQual", 12);
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
			else if (helpFor == "p ram")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  prints the contents of RAM memory. In HEXa by default, but can be modified using the parameter.", 14);

				//USAGE
				this->printLine("\nUSAGE: ", 13);
				//general form of the command
				this->print("  >p ram", 14);
				this->print(" <oType>", 12);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  oType", 12);
				this->print("     <param>", 3);
				this->print("    determines the type of output.", 14);
				this->print("\n    -d", 12);
				this->print(" output in decimal.", 14);
				this->print("\n    -h", 12);
				this->print(" output in hexa.", 14);

				//example
				this->printLine("\n\n  Example", 5);
				this->print("  >p ram", 14);
				this->printLine("  (prints RAM in default (HEX) format)", 14);
				this->print("  >p ram", 14);
				this->print(" -d", 12);
				this->printLine("  (prints RAM in decimal format)", 14);
			}
			else if (helpFor == "p vm")
			{
				//DESCRIPTION
				this->printLine("DESCRIPTION: ", 13);
				this->print("  prints the contents of virtual memory. In HEXa by default, but can be modified using the parameter.", 14);

				//USAGE
				this->printLine("\nUSAGE: ", 13);
				//general form of the command
				this->print("  >p vm", 14);
				this->print(" <oType>", 12);
				this->print("\n", 14);
				//variables explained
				//table header
				this->printLine("\n  Name      Type       Description", 5);

				//var 1
				///variable name and what it is
				this->print("  oType", 12);
				this->print("     <param>", 3);
				this->print("    determines the type of output.", 14);
				this->print("\n    -d", 12);
				this->print(" output in decimal.", 14);
				this->print("\n    -h", 12);
				this->print(" output in hexa.", 14);

				//example
				this->printLine("\n\n  Example", 5);
				this->print("  >p vm", 14);
				this->printLine("  (prints vMemory in default (HEX) format)", 14);
				this->print("  >p ram", 14);
				this->print(" -d", 12);
				this->printLine("  (prints vMemory in decimal format)", 14);
			}
			else if (helpFor == "p sch")
			{
				this->printLine("DESCRIPTION: ", 13);
				this->print("  print scheduler contents. Shows the currently running process and both of the scheduler queues.\n", 14);

				this->printLine("USAGE: ", 13);
				//general form of the command
				this->print("  >p sch\n", 14);
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
			std::shared_ptr<PCB> temp = processManager->getPCBByPID(std::stoi(match[3]));
			if (temp == nullptr) { this->printLine("AN ERROR OCCURED!", 12); this->printCode(38); }
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
			if (temp == nullptr) { this->printLine("AN ERROR OCCURED!", 12); this->printCode(38); }
			else
			{
				this->print("PROCESS INFORMATION ", 13);
				printProcessInformation(temp);
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^ls$")))
		{
			auto files = fileManager->ls();

			this->printLine(" DIRECTORY: \\HOME>", 14);
			if (files.size() == 0) { this->printLine("  <the directory is empty>", 4); }
			else
			{
				this->printLine("", 0);
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
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^rm[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 3);
			fileManager->closeFile(command, 0);
			uint8_t code = fileManager->deleteFile(command);
			if (code == 0)
			{
				this->print("File ", 14);
				this->print(command, 12);
				this->printLine(" has been removed.", 14);
			}
			else { this->printLine("AN ERROR OCCURED!", 12); this->printCode(code); }
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^touch[ ]+[0-9a-zA-z]+$")))
		{
			command.erase(0, 6);
			uint8_t code = fileManager->createFile(command);
			if (code == 0)
			{
				this->print("File ", 14);
				this->print(command, 12);
				this->printLine(" has been created.", 14);
			}
			else {
				this->printLine("AN ERROR OCCURED!", 12); this->printCode(code);
				//print tip for innaproriate name lenght
				if (code == 71)
				{
					this->print("[", 14);
					this->print("Tip: ", 13);
					this->print("The fileName must be ", 14);
					this->print(std::to_string(FileNameLenght), 11);
					this->printLine(" chars long.]", 14);
				}
			}
		}
		else if (std::regex_match(command, match, std::regex("^(cat[ ]+)([0-9a-zA-z]+)( -[ah])?$")))
		{
			std::pair<uint8_t, std::string> code = fileManager->cat(match[2]);

			if (code.first == 0)
			{
				this->print("FILENAME: ", 13);
				this->printLine(match[2], 14);
				this->printLine("CONTENTS ", 13);
				//check if it's an empty file
				if (code.second == "") { this->printLine("<the file is empty>", 4); }
				else
				{
					//print in hexa
					if (match[3] == " -h")
					{
						for (auto cha : code.second)
						{
							if (cha != '\n') { this->print(toHexString(cha), 14); }
							else { std::cout << std::endl; }
						}
						std::cout << std::endl;
					}
					//print as string
					else { this->printLine(code.second, 14); }
				}

			}
			else { this->printLine("AN ERROR OCCURED!", 12); this->printCode(code.first); }
		}
		else if (std::regex_match(command, match, std::regex("^(p in )([0-9a-zA-z]+)$")))
		{
			std::pair<std::vector<int>, uint8_t> code = fileManager->printInode(match[2]);

			if (code.second == 0)
			{
				this->print("FILENAME: ", 13);
				this->printLine(match[2], 14);
				this->printLine("I NODE CONTENTS ", 13);
				this->print("  ", 13);
				this->printLine("[0] [1] [2]", 5);
				this->print("   ", 13);
				for (auto iNode : code.first)
				{
					print(std::to_string(iNode) + "   ", 14);
				}
				std::cout << std::endl;
			}
			else { this->printLine("AN ERROR OCCURED!", 12); this->printCode(code.second); }
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
			if (code.first == 0)
			{
				this->print("File ", 14);
				this->print(filename, 12);
				this->print(" has ", 14);
				this->print(code.second, 11);
				this->printLine(" characters. ", 14);
			}
			else { this->printLine("AN ERROR OCCURED!", 12); this->printCode(code.first); }
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
			if (code == 0)
			{
				this->print("The file ", 14);
				this->print(filename, 12);
				this->print(" has been renamed to ", 14);
				this->print(argument, 11);
				this->printLine(".", 14);
			}
			else { this->printLine("AN ERROR OCCURED!", 12); this->printCode(code); }
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

			uint8_t code = 0;
			for (auto& letter : argument)
			{
				code = fileManager->append(filename, letter);
				if (code != 0)
				{
					if (code == 65)
					{
						this->printLine("AN ERROR OCCURED!", 12);
						this->print("The string \"", 14);
						this->print(argument, 11);
						this->print("\" has been", 14);
						this->print(" PARTIALLY ", 12);
						this->print("added to ", 14);
						this->print(filename, 12);
						this->printLine(".", 14);
					}
					this->printCode(code);
					break;
				}
			}
			if (code == 0)
			{
				this->print("The string \"", 14);
				this->print(argument, 11);
				this->print("\" has been added to ", 14);
				this->print(filename, 12);
				this->printLine(".", 14);
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
			if (code == 0)
			{
				this->print("File ", 14);
				this->print(filename, 12);
				this->printLine(" has been cleared.", 14);
			}
			else { this->printLine("AN ERROR OCCURED!", 12); this->printCode(code); };
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
			argument = command + ".txt";

			std::pair<uint8_t, unsigned int> errorCode = this->processManager->fork(filename, 0, argument);

			if (errorCode.first != 0) { this->printLine("AN ERROR OCCURED!", 12); this->printCode(errorCode.first); }
			else
			{
				this->print("New process created with PID = ", 14);
				this->printLine(errorCode.second, 3);
			}
		}
		else if (std::regex_match(command, match, std::regex("^(kill)([ ])([0-9]+)$")))
		{
			//delete with pid
			uint8_t errorCode = processManager->deleteProcess(std::stoi(match[3]));
			if (errorCode != 0) { this->printLine("AN ERROR OCCURED!", 12); this->printCode(errorCode); }
			else
			{
				this->print("Process ", 14);
				this->print("[PID: " + match[3].str() + "]", 11);
				this->printLine(" has been deleted,", 14);
			}
		}
		else if (std::regex_match(command, match, std::regex("^(kill)([ ])([a-z0-9]+)$")))
		{
			//delete with name
			uint8_t errorCode = processManager->deleteProcess(match[3]);
			if (errorCode != 0) { this->printLine("AN ERROR OCCURED!", 12); this->printCode(errorCode); }
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
			if (errorCode != 0) { this->printLine("AN ERROR OCCURED IN SCHEDULER!", 12); this->printCode(errorCode); }

			std::pair < uint8_t, std::string> errorC = this->interpreter->go();
			if (errorC.first != 0) {
				this->printLine("AN ERROR OCCURED IN INTERPRETER!", 12);
				if (errorC.second != "" || errorC.second != "ERR")
				{
					this->print("(When trying to execute ", 14);
					this->print(errorC.second, 11);
					this->print(" in  ", 14);
					this->print(RUNNING->getName(), 12);
					this->printLine(")", 14);
				}
				this->printCode(errorC.first);
			}
			else
			{
				this->print("Currently running: ", 14);
				this->printLine(RUNNING->getName(), 12);
				this->print("Completed instruction: ", 14);
				this->printLine(errorC.second, 11);
			}
			goCounter++;
			if (goCounter == 7)
			{
				this->print("[", 14);
				this->print("Tip: ", 13);
				this->print("You can use \"go ", 14);
				this->print("[NUMBER OF STEPS]", 11);
				this->printLine("\" instead.]", 14);
				goCounter = 0;
			}

			//uint8_t errorCode = this->scheduler->schedule();
			if (errorCode != 0) { this->printLine("AN ERROR OCCURED IN SCHEDULER!", 12); this->printCode(errorCode); }
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("go[ ][0-9]+")))
		{
			unsigned int count = 0;
			command.erase(0, 3);

			for (unsigned int i = 0; i < command.length(); i++)
			{
				count *= 10;
				count += (command[i] - '0');
			}
			if (count <= 999)
			{
				this->print("EXECUTING ", 13);
				this->print(count, 9);
				this->printLine(" INSTRUCTIONS ", 13);
				this->printLine("  No. Details", 5);
				for (unsigned int i = 0; i < count; i++)
				{
					uint8_t errorCode = this->scheduler->schedule();
					if (errorCode != 0)
					{
						this->print("  " + std::to_string(i + 1), 12);
						//position the string appropriately
						std::string spaces = "    ";
						for (int z = 0; z < std::to_string(i + 1).length(); z++) { if (spaces.length() > 0) { spaces.pop_back(); } }
						this->printLine(spaces + "AN ERROR OCCURED IN SCHEDULER!", 12);
						this->print("     ", 0);
						this->printCode(errorCode);
					}

					std::pair < uint8_t, std::string> errorC = this->interpreter->go();
					if (errorC.first != 0)
					{
						this->print("  " + std::to_string(i + 1), 12);
						//position the string appropriately
						std::string spaces = "    ";
						for (int z = 0; z < std::to_string(i + 1).length(); z++) { if (spaces.length() > 0) { spaces.pop_back(); } }
						this->printLine(spaces + "AN ERROR OCCURED IN INTERPRETER!", 12);
						if (errorC.second != "" || errorC.second != "ERR")
						{
							this->print("      (When trying to execute ", 14);
							this->print(errorC.second, 11);
							this->print(" in  ", 14);
							this->print(RUNNING->getName(), 12);
							this->printLine(")", 14);
						}
						this->print("      ", 0);
						this->printCode(errorC.first);
					}
					else
					{
						this->print("  " + std::to_string(i + 1), 9);
						//position the string appropriately
						std::string spaces = "    ";
						for (int z = 0; z < std::to_string(i + 1).length(); z++) { if (spaces.length() > 0) { spaces.pop_back(); } }

						this->print(spaces + "Currently running: ", 14);
						this->printLine(RUNNING->getName(), 12);
						this->print("      Completed instruction: ", 14);
						this->printLine(errorC.second, 11);
					}
				}
			}
			else { this->printLine("AN ERROR OCCURED!", 12); this->printCode(1); }

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^ps$")))
		{
			this->print("PROCESSES TREE", 13);

			this->printWithPadding("\n" + processManager->displayTree(), 14, 2);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^format c$")))
		{
			while (Containers::MainFileCatalog.size() > 0)
			{
				uint8_t err = this->fileManager->deleteFile(Containers::MainFileCatalog.begin()->name);
				if (err != 0) { printCode(err); }
			}
			this->printLine("FORMAT PERFORMED SUCCESFULLY", 12);
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^ps[ ]-[wra]$")))
		{
			std::vector<std::shared_ptr<PCB>> temp;
			if (command.at(command.length() - 1) == 'w')
			{
				this->printLine("WAITING PROCESSES", 13);
				temp = this->processManager->getAllWithState(PCB::ProcessState::WAITING);
				for (auto p : temp)
				{
					this->print("  -" + p->getName() + " [PID ", 14);
					this->print(p->getPID(), 11);
					this->printLine("]", 14);
				}
				if (temp.size() == 0) { this->printLine("<No processes found>", 4); }
			}
			else if (command.at(command.length() - 1) == 'r')
			{
				this->print("RUNNING PROCESS", 13);
				temp.push_back(processManager->getPCBByPID(RUNNING->getPID()));
				for (auto p : temp) { this->printProcessInformation(p); }
				if (temp.size() == 0) { this->printLine("<No processes found>", 4); }
			}
			else if (command.at(command.length() - 1) == 'a')
			{
				this->printLine("READY PROCESSES", 13);
				temp = this->processManager->getAllWithState(PCB::ProcessState::READY);
				for (auto p : temp)
				{
					this->print("  -" + p->getName() + " [PID ", 14);
					this->print(p->getPID(), 11);
					this->printLine("]", 14);
				}
				if (temp.size() == 0) { this->printLine("<No processes found>", 4); }
			}
		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^p fs$")))
		{
			this->printLine("DRIVE CONTENTS", 13);
			auto names = fileManager->ls();

			this->print("  ", 0);
			std::string str;
			for (unsigned int i = 0; i < Containers::bit_vector.size() / 2; i++)
			{
				this->print(i, 14);
				this->print("  ", 14);
				if (i < 10)this->print(" ", 14);
			}

			std::cout << std::endl;
			this->print("  ", 0);
			for (unsigned int i = 0; i < Containers::bit_vector.size() / 2; i++)
			{
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
					this->print(char(178), color);
					std::cout << " ";
				}
				else if (Containers::bit_vector[i] == 1)
				{
					this->print(char(176), 14);
					this->print(char(176), 14);
					std::cout << " ";
				}
				std::cout << " ";
			}
			std::cout << std::endl;

			this->print("  ", 0);
			for (unsigned int i = Containers::bit_vector.size() / 2; i < Containers::bit_vector.size(); i++)
			{
				this->print(i, 14);
				this->print("  ", 14);
				if (i < 10)this->print(" ", 14);
			}
			std::cout << std::endl;

			this->print("  ", 0);
			for (unsigned int i = Containers::bit_vector.size() / 2; i < Containers::bit_vector.size(); i++)
			{
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
					this->print(char(178), color);
					std::cout << " ";
				}
				else if (Containers::bit_vector[i] == 1)
				{
					this->print(char(176), 14);
					this->print(char(176), 14);
					std::cout << " ";
				}
				std::cout << " ";
			}
			std::cout << std::endl;

			//if there are any files, print out the color legend
			if (Containers::Colors.size() > 0) {
				this->printLine("LEGEND", 13);
				this->printLine("  Color  FileName  Size  Preview", 5);

				for (int z = 0; z < Containers::Colors.size(); z++)
				{
					auto x = Containers::Colors[z];
					this->print("   ", 0);
					this->print(char(178), x.second + 1);
					this->print(char(178), x.second + 1);
					this->print("      " + x.first, 14);
					std::string wcTemp = std::to_string(fileManager->wc(x.first).second);
					this->print("      " + wcTemp, 11);
					std::string catTemp = fileManager->cat(x.first).second;
					//if cat too long elapse it (max 20 chars)
					if (catTemp.size() > 25) { catTemp = catTemp.substr(0, 22) + "{...}"; }

					//calculate spaces
					std::string spaces = "    ";
					for (int z = 0; z < wcTemp.length(); z++) { if (spaces.length() > 0) { spaces.pop_back(); } }
					//if cat empty, print empty file tag
					if (catTemp.length() == 0)
					{
						this->print(spaces + "  ", 14);
						this->printLine("<emptyFile>", 12);
					}
					else { this->printLine(spaces + "  \"" + catTemp + "\"", 14); }
					if (z < Containers::Colors.size() - 1) { std::cout << std::endl; }
				}
			}


		}
		else if (std::regex_match(command, match, std::regex("^(p vm)( -[dh])?$")))
		{
			this->printLine("DC QUEUE", 13);
			this->print("  Frame Number    ", 5);
			this->printLine("Reference Bit", 5);
			for (auto& pair : virtualMemory->queue)
			{
				this->print("       ", 14);
				this->print(int(pair.first), 9);
				this->print("               ", 14);
				this->printLine(int(pair.second), 14);
			}
			std::cout << std::endl;

			this->printLine("VIRTUAL MEMORY CONTENT", 13);
			this->print("  PID   ", 5);
			//print as decimal if specified
			if (match[2] == " -d") { this->printLine("Page content [DEC]", 5); }
			//else print as hexa
			else { this->printLine("Page Content [HEX]", 5); }


			for (auto& pair : virtualMemory->swapFile)
			{
				for (auto& page : pair.second)
				{
					std::cout << "   ";
					this->print(pair.first, 9);

					std::cout << "    ";
					for (unsigned int i = 0; i < 16; i++)
					{
						//print as decimal if specified
						if (match[2] == " -d") { this->print(std::to_string(page.data[i]), 14); }
						//else print as hexa
						else { this->print(toHexString(page.data[i]), 14); }
						this->print(" ", 14);
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}


		}
		else if (std::regex_match(command, match, std::regex("^(p pt )([0-9]+)$")))
		{

			uint8_t errorCode = 0;
			auto results = this->memoryManager->printPageTable(std::stoi(match[2]), errorCode);
			//check for errors
			if (errorCode != 0) { this->printLine("AN ERROR OCCURED!", 12); printCode(errorCode); }
			else
			{
				this->print("PAGE TABLE for PID=", 13);
				this->printLine(match[2], 11);

				this->printLine("No. Frame Correctness Bit ", 5);
				for (int i = 0; i < results.size(); i++)
				{
					print(" " + std::to_string(i), 9);
					std::string frameTemp = std::to_string(results[i].first);
					if (results[i].first == -1) { frameTemp = "-"; }
					print("    " + frameTemp, 14);
					printLine("         " + std::to_string(results[i].second), 14);
				}
			}

		}
		else if (std::regex_match(command, match, std::regex("^(p ram)( -[dh])?$")))
		{
			this->printLine("RAM CONTENT", 13);
			this->print("  Frame Number    ", 5);
			if (match[2] == " -d") { this->printLine("Contents [DEC]", 5); }

			//print in hexa
			else { this->printLine("Contents [HEX]", 5); }

			for (unsigned int i = 0; i < 8; i++)
			{
				this->print("       ", 9);
				this->print(int(i), 9);
				std::cout << "          ";

				//COLORING RAM
				int color = 14;
				int result = memoryManager->FrameTable[i].pid;
				if (result != -1) { color = 14 + 16 * (result + 1); }

				for (unsigned int j = 0; j < 16; j++)
				{
					std::string temp = "";
					//print in decimal
					if (match[2] == " -d") { temp = std::to_string(memoryManager->ram[(PAGE_SIZE * i) + j]); }
					//print in hexa
					else { temp = toHexString(memoryManager->ram[(PAGE_SIZE * i) + j]); }

					this->print(temp, color);
					int spaceNumber = 4 - temp.length();
					//TO AVOID COLOR BEING ALL THE WAY THROUGH THE CONSOLE
					if (j == 15) { this->print("", 0); }

					for (int z = 0; z < spaceNumber; z++) { print(" ", color); }
				}
				std::cout << std::endl;
			}
			std::vector<int> addedPIDs;
			if (memoryManager->FrameTable[0].pid != -1) {
				this->printLine("LEGEND", 13);
				this->printLine("  Color  PID", 5);
				for (int z = 0; z < 8; z++)
				{
					int color = 14;
					int result = memoryManager->FrameTable[z].pid;
					if (result == -1)continue;

					bool printLegend = true;
					for (auto p : addedPIDs) { if (p == result) { printLegend = false; } }

					if (printLegend)
					{
						addedPIDs.push_back(result);
						color = (result + 1);

						this->print("   ", 0);
						this->print(char(178), color);
						this->print(char(178), color);
						this->print("     " + std::to_string(result), 14);
						if (z < 7) { std::cout << std::endl; }
					}

				}
			}


		}
		else if (std::regex_match(command, match, std::regex("^color$")))
		{
			for (int i = 0; i < 256; i++)
			{
				print(std::to_string(i) + "                                                                        ", i);
				this->restoreDefaultColor();
				std::cout << std::endl;
			}

		}
		else if (std::regex_match(command.begin(), command.end(), std::regex("^p sch")))
		{
			this->print("SCHEDULER COUNTER: ", 13);
			this->print(std::to_string(scheduler->counter), 11);
			std::cout << std::endl;

			this->print("CURRENTLY RUNNING: ", 13);
			if (RUNNING != NULL)
			{
				this->print(RUNNING->getName(), 14);
				this->print(" [PID= ", 14);
				this->print(RUNNING->getPID(), 11);
				this->print("]", 14);
				if (!RUNNING->getHasPID(0))
				{
					this->print(" (Runs until Counter= ", 14);
					this->print(RUNNING->counter, 11);
					this->print(")", 14);
				}
				else
				{
					this->print(" (Runs until any", 14);
					this->print(" OTHER PROCESS ", 11);
					this->print("is added)", 14);
				}

			}
			else { this->print("<none>", 4); }
			std::cout << std::endl;
			std::cout << std::endl;
			this->printLine("QUEUES ", 13);

			this->print("  No.  ", 11);
			this->print("ACTIVE [PRIORITY]", 10);
			this->print("    ", 10);
			this->print("EXPIRED [PRIORITY]", 12);
			std::cout << std::endl;

			//check which has more elements
			int maxSize = scheduler->active->size();
			if (scheduler->expired->size() > maxSize) { maxSize = scheduler->expired->size(); }

			for (int i = 0; i < maxSize; i++)
			{

				//print Number first
				this->print("   " + std::to_string(i + 1), 3);

				unsigned int spaceDelay = 20;

				//position the elements in second row
				this->print("   ", 1);

				//PRINT ACTIVE
				if (i < scheduler->active->size())
				{
					this->print((*scheduler->active)[i]->getName(), 2);
					this->print(" [", 2);
					this->print(std::to_string((*scheduler->active)[i]->priority), 10);
					this->print("]", 2);
					//prepare spaces between the seconds and third row
					spaceDelay -= ((*scheduler->active)[i]->getName().length() + 5);
				}
				//if there are no elements in this collumn at all
				if (i == 0 && scheduler->active->size() == 0) { this->print("<none>", 2); spaceDelay -= 6; }

				//position the elements in third row
				for (unsigned int j = 0; j < spaceDelay; j++)this->print(" ", 6);

				//PRINT EXPIRED
				if (i < scheduler->expired->size())
				{
					this->print((*scheduler->expired)[i]->getName(), 4);
					this->print(" [", 4);
					this->print(std::to_string((*scheduler->expired)[i]->priority), 12);
					this->print("]", 4);
				}
				//if there are no elements in this collumn at all
				if (i == 0 && scheduler->expired->size() == 0) { this->print("<none>", 4); }

				//go to next line
				std::cout << std::endl;
			}

			if (maxSize == 0)
			{
				this->print("     <none>", 2);
				for (unsigned int j = 0; j < 14; j++)this->print(" ", 6);
				this->print("<none>", 4);
				std::cout << std::endl;
			}

		}
		else if (std::regex_match(command, match, std::regex("^p sem$")))
		{
			this->print("FILENAME", 13);
			this->print("        ", 13);
			this->print("STATE", 13);
			this->print("        ", 13);
			this->print("VALUE", 13);
			this->print("        ", 13);
			this->print("QUEUE\n", 13);
			if (Containers::MainFileCatalog.size() != 0)
			{
				for (auto& file : Containers::MainFileCatalog)
				{
					this->print(file.name, 14);
					for (unsigned int i = file.name.length(); i < 16; i++) { this->print(" ", 14); }

					if (file.s.getValue() < 0)
					{
						for (unsigned int i = 0; i < 5; i++)this->print(char(178), 4);
					}
					else
					{
						for (unsigned int i = 0; i < 5; i++)this->print(char(178), 2);
					}
					this->print("         ", 13);
					this->print(file.s.getValue(), 11);
					this->print("          ", 13);
					auto procList = file.s.getList();;
					for (int i = 0; i < procList.size(); i++)
					{
						this->print(procList[i]->getName(), 14);
						if (i < procList.size() - 1) { this->print(", ", 14); }
					}
					std::cout << std::endl;
				}
			}
			else { printLine("<no files>", 4); }

		}
		else
		{
			this->printLine("UNRECOGNISED COMMAND", 4);
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
	print("[", 14);
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
		if (letter == ' ')break;
		if (letter >= 65 && letter <= 90)
		{
			letter += 32;
		}
	}
}

void Shell::printCode(uint8_t code)
{
	SetConsoleTextAttribute(hConsole, 4);

	switch (code)
	{
	case 0:
		std::cout << "ALLES GING BESSER ALS ERWARTET" << std::endl;
		break;
	case 1:
		std::cout << "CODE 1 : GO_CAN_EXECUTE_MAX_999_COMMANDS_AT_ONCE" << std::endl;
		break;
	case 16:
		std::cout << "CODE 16 : ERROR_SH_PRIORITY_OUT_OF_RANGE" << std::endl;
		break;
	case 17:
		std::cout << "CODE 17 : ERROR_SH_ADDED_PROCESS_DOES_NOT_EXIST" << std::endl;
		break;
	case 18:
		std::cout << "CODE 18 : ERROR_SH_PROCESS_NOT_FOUND" << std::endl;
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
		std::cout << "CODE 41 : ERROR_PM_CODE_DOESNT_FIT_INTO_NUMBER_OF_DECLARED_PAGES" << std::endl;
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
	case 71:
		std::cout << "CODE 71 : ERROR_INAPPROPRIATE_FILENAME_LENGHT" << std::endl;
		break;
	case 81:
		std::cout << "CODE 81 : ERROR_PAGE_DOESNT_EXIST" << std::endl;
		break;
	case 82:
		std::cout << "CODE 82 : ERROR_PROCESS_IS_NOT_RUNNING" << std::endl;
		break;
	case 96:
		std::cout << "CODE 96 : ERROR_VM_PROGRAM_ALREADY_EXISTS" << std::endl;
		break;
	case 97:
		std::cout << "CODE 97 : ERROR_VM_EMPTY_PROGRAM" << std::endl;
		break;
	case 98:
		std::cout << "CODE 98 : ERROR_VM_OUT_OF_QUEUE_RANGE" << std::endl;
		break; 
	case 99:
		std::cout << "CODE 99 : ERROR_VM_OUT_OF_PROGRAM_RANGE" << std::endl;
		break;
	case 100:
		std::cout << "CODE 100 : ERROR_VM_PROGRAM_DOES_NOT_EXIST" << std::endl;
		break;
	case 200:
		std::cout << "CODE 200 : ERROR_UNKNOWN_INSTRUCTION" << std::endl;
		break;
	case 201:
		std::cout << "CODE 201 : ERROR_NUMBER_TOO_LARGE" << std::endl;
		break;
	case 202:
		std::cout << "CODE 202 : ERROR_NUMBER_TOO_SMALL" << std::endl;
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

void Shell::printLogo()
{
	std::cout << "\n\n";
	this->printLine("\t\t########     ###    ##    ## ########   #######  ##     ##    #######   ######  ", 8);
	this->printLine("\t\t##     ##   ## ##   ###   ## ##     ## ##     ## ###   ###   ##     ## ##    ## ", 11);
	this->printLine("\t\t##     ##  ##   ##  ####  ## ##     ## ##     ## #### ####   ##     ## ##       ", 12);
	this->printLine("\t\t########  ##     ## ## ## ## ##     ## ##     ## ## ### ##   ##     ##  ######  ", 13);
	this->printLine("\t\t##   ##   ######### ##  #### ##     ## ##     ## ##     ##   ##     ##       ## ", 14);
	this->printLine("\t\t##    ##  ##     ## ##   ### ##     ## ##     ## ##     ##   ##     ## ##    ## ", 15);
	this->printLine("\t\t##     ## ##     ## ##    ## ########   #######  ##     ##    #######   ######  ", 9);
}

template <typename I> std::string Shell::toHexString(I w) {

	I temp = abs(w);
	size_t hex_len = sizeof(I) << 1;
	static const char* digits = "0123456789ABCDEF";
	std::string rc(hex_len, '0');
	for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
		rc[i] = digits[(temp >> j) & 0x0f];

	if (w < 0) { return "-" + rc; }
	return " " + rc;
}


