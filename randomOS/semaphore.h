#pragma once
#include <mutex>
#include <memory>
#include <queue>

/*DELETE once ProcessManagement.h is ready*/
class PCB {

};
std::shared_ptr<PCB> pcbPtr; 
/*DELETE once ProcessManagement.h is ready*/

class Semaphore //TODO: output for step-by-step execution
{
private:
	std::mutex mtx; //mutex locks provide protection for the constructor, wait() and signal() methods without negative impacts for multithreaded systems
	int value;
	std::queue<std::shared_ptr<PCB>> list; //TODO: possible changes due to PCB implementation

	void sleep(); //TODO: redundant once PCB is implemented
	void wakeup(std::shared_ptr<PCB> pcbPtr); //redundant once PCB is implemented

public:
	Semaphore(int new_value);
	void wait();
	void signal();

	const int& getValue();
	const std::queue<std::shared_ptr<PCB>>& getList();
};