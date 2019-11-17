
#include "Includes.h"

#include "Memory.h"
#include "ProcessManager.h"
#include "Interpreter.h"


int main()
{
	////TEST PAMIĘCI RAM
	///*Memory mem;
	//mem.printMemory();*/

	//TEST MANAGERA PROCESOW
	ProcessManager tes;
	std::pair<int8_t,unsigned int> processReturn= std::make_pair(0,0);
	
	std::cout << "\n\n---PROCESS CREATION----\n\n";

	processReturn = tes.fork("eyo", 0, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn =tes.fork("dumm", 0, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn =tes.fork("", 2, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("KarolWojtyla", 2, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }


	processReturn = tes.fork("konstantynopolitanczykieniczowiecznekeow", 1, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("okay", 2, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("not okay", 2, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("janPawel", 3, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("janek", 4, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }


	processReturn = tes.fork("tomek", 5, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }
	
	processReturn = tes.fork("franek", 4, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }

	processReturn = tes.fork("janPawel", 6, "xd");
	if (processReturn.first != 0) { std::cout << "\nERROR!! number: " << std::to_string(processReturn.first); }
	else { std::cout << "\nNew Process created with PID= " << std::to_string(processReturn.second); }
	tes.getPCBByPID(3)->setStateRunning();

	std::cout<<"\n\n---PROCESS TREE----\n"<<tes.displayTree() << "\n\n-------------------";
	std::cout << "\n\n---STATE READY----" << tes.displayWithState(PCB::ProcessState::READY);
	std::cout << "\n\n---STATE RUNNING----" << tes.displayWithState(PCB::ProcessState::RUNNING);

	std::cout << "\n\n---DETAILS of process with PID 3---" << tes.displayDetailsOfAProcess(3).second;

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

}

