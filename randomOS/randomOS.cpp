// randomOS.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "Memory.h"
#include "Includes.h"
#include "Interpreter.h"
#include "ProcessManager.h"

#include "PCB.h"

int main()
{
	Memory mem;
	//TEST MANAGERA PROCESOW

	ProcessManager tes;
	std::pair<int8_t,unsigned int> processReturn= std::make_pair(0,0);
	unsigned int PIDOFTHECREATED=0;

	std::cout << "\n\n---PROCESS CREATION----\n\n";

	processReturn =tes.fork("dumm", 0, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn =tes.fork("", 1, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("KarolWojtyla", 1, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }


	processReturn = tes.fork("konstantynopolitanczykieniczowiecznekeow", 1, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("okay", 1, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("not okay", 1, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("janPawel", 2, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("janPawel", 5, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	std::cout<<"\n\n---PROCESS TREE----\n"<<tes.displayTree() << "\n\n-------------------";

	int8_t errorHandling=0;

	std::cout << "\n\n---PROCESS DELETION----\n\n";
	for (int i = 0; i < 12; i++) {
		std::cout << "\nTrying to delete process with PID=" << std::to_string(i) <<"...";
		errorHandling = tes.deleteProcess(i);
		if(errorHandling!=0){ std::cout << "\nERROR!! number: " << std::to_string(errorHandling);}
		else {
			std::cout << "\n---------NEW TREE-----\n" << tes.displayTree() <<"\n\n-------------------";
		}
	
	}
	
	mem.printMemory();
}

