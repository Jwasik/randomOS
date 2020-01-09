
#include "Includes.h"

#include "FileMenager.h"
#include "Memory.h"
#include "ProcessManager.h"
#include "MemoryManager.h"
#include "VirtualMemory.h"
#include "Interpreter.h"
#include "Scheduler.h"
#include "Shell.h"
#include "PCB.h"

int main()
{
	//Modules
	//Group 1
	std::shared_ptr<VirtualMemory> virtualMemory = std::make_shared<VirtualMemory>();
	std::shared_ptr<Memory> memoryManager = std::make_shared<Memory>(virtualMemory);
	std::shared_ptr<FileMenager> fileManager = std::make_shared<FileMenager>(memoryManager);

	//Group 2
	std::shared_ptr<Scheduler> scheduler = std::make_shared<Scheduler>(fileManager,virtualMemory,memoryManager);
	std::shared_ptr<ProcessManager> processManager = std::make_shared<ProcessManager>(scheduler, virtualMemory, fileManager, memoryManager);
	std::shared_ptr<Interpreter> interpreter = std::make_shared<Interpreter>(memoryManager,fileManager,processManager);

	Shell shell(fileManager, memoryManager, virtualMemory, processManager, scheduler, interpreter);
	shell.run();
}

