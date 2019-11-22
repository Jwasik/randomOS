#pragma once
#include "Includes.h"
class Memory 
{


public:
	char ram[128];//pamiec wlasciwa
	Memory(int);
	void writeInMem(int,std::string);
	void deleteFromMem(int);
	void printMemory();
	int LogicaltoPhysical(int logical);
	char& getMemoryContent(int logical, int PC);//dopóki nie wiem jakie adresy dostane od wirtualnej traktuje PID jako adres logiczny
	bool FrameTable[8]{0};
	

};