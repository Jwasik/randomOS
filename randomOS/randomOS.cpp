﻿
#include "Includes.h"

#include "FileMenager.h"
#include "MemoryManager.h"

#include "ProcessManager.h"
#include "MemoryManager.h"
#include "VirtualMemory.h"
#include "Interpreter.h"
#include "Scheduler.h"
#include "Shell.h"
#include "PCB.h"

int main()
{
	//Shell shell;
	//m.test();

	//shell.run();


	//TEST RAMU 
	std::shared_ptr<VirtualMemory> vm = std::make_shared<VirtualMemory>();
	Memory mem(vm);
	Memory* memory = &mem;
	FileMenager f(memory);
	f.createFile("ja");
	f.openFile("ja", 1);
	mem.creatProcess(1);
	for (int i = 0; i < 120; i++)
	{
		f.writeToEndFile(70, 1);
	}
	f.writeToEndFile(75,1);
	f.readFile(0,0,121,1);

	//mem.printPageTable(1);
	mem.printMemory();
	


	//vm->test();
	//mem.test();
	////TEST MENADERA PLIKOW
	
	FileMenager f;
	f.createFile("jak");
	//f.openFile("ja",1);
	



	Shell shell(fileManager, memoryManager, virtualMemory);
	shell.run();

	////TEST MENADERA PLIKOW
	
	
	////TEST PAMIĘCI RAM
	///*Memory mem;
	//mem.printMemory();*/

	////TEST MANAGERA PROCESOW
	//ProcessManager tes;
	//std::pair<int8_t,unsigned int> processReturn= std::make_pair(0,0);
	//
	//std::cout << "\n\n---PROCESS CREATION----\n\n";

	//processReturn =tes.fork("dumm", 0, "xd");
	//if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	//else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	//processReturn =tes.fork("", 1, "xd");
	//if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	//else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	//processReturn = tes.fork("KarolWojtyla", 1, "xd");
	//if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	//else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }


	//processReturn = tes.fork("konstantynopolitanczykieniczowiecznekeow", 1, "xd");
	//if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	//else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	//processReturn = tes.fork("okay", 1, "xd");
	//if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	//else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	//processReturn = tes.fork("not okay", 1, "xd");
	//if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	//else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	//processReturn = tes.fork("janPawel", 2, "xd");
	//if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	//else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	//processReturn = tes.fork("janPawel", 5, "xd");
	//if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	//else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }
	//tes.getPCBByPID(3)->setStateRunning();

	//std::cout<<"\n\n---PROCESS TREE----\n"<<tes.displayTree() << "\n\n-------------------";
	//std::cout << "\n\n---STATE READY----" << tes.displayWithState(PCB::ProcessState::READY);
	//std::cout << "\n\n---STATE RUNNING----" << tes.displayWithState(PCB::ProcessState::RUNNING);

	//int8_t errorHandling=0;

	//std::cout << "\n\n---PROCESS DELETION----\n\n";
	//for (int i = 0; i < 12; i++) {
	//	std::cout << "\nTrying to delete process with PID=" << std::to_string(i) <<"...";
	//	errorHandling = tes.deleteProcess(i);
	//	if(errorHandling!=0){ std::cout << "\nERROR!! number: " << std::to_string(errorHandling);}
	//	else {
	//		std::cout << "\n---------NEW TREE-----\n" << tes.displayTree() <<"\n\n-------------------";
	//	}
	//
	//}

	//VM TEST

	/*int8_t data[16] = { 0 };
	std::vector<VirtualMemory::Page> pages;
	for (int8_t i = 0; i < 8; i++) {
		for (int8_t &j : data) {
			j = i;
		}
		pages.emplace_back(VirtualMemory::Page(data));
	}
	for (int i = 0; i<8; i++) virtualMemory->insertProgram(std::make_pair(i, pages));
	virtualMemory->printSwapFile();
	uint8_t byte = virtualMemory->getPage(7, 3).data[10];
	std::cout << std::to_string(byte) << std::endl;*/
}

