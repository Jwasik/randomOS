
#include "Includes.h"
#include "FileMenager.h"
#include "Memory.h"
#include "ProcessManager.h"
#include "Interpreter.h"


int main()
{
	FileMenager f;
	f.createFile("plik");
	f.deleteFile("plik");
	////TEST MENADERA PLIKOW
	/*
	FileMenager f;
	f.createFile("ja");
	f.openFile("ja",1);
	
	for (int i = 0; i < 90; i++)
	{
		f.writeToEndFile(67,1);
	}
	f.writeToFile(70,70,1);

	f.createFile("jak");
	f.openFile("jak", 2);
	for (int i = 0; i < 80; i++)
	{
		f.writeToEndFile(70, 2);
	}
	for (int i = 0; i < 80; i++)
	{
		f.writeToEndFile(67, 2);
	}
	f.closeFile("ja",1);
	//f.deleteFile("ja");
	//std::string t = f.cat("ja");
	//std::cout << t;
	//f.readFile(0,70,20,2);
	*/


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

}

