#pragma once
#include <atomic>
#include <mutex>
#include <memory>
#include <queue>

/*DELETE once ProcessManagement.h is ready*/
class PCB {

};
PCB pcb; //TODO: 
/*DELETE once ProcessManagement.h is ready*/

class Semaphore 
{
private:
	std::mutex mtx;
	std::atomic<int> value;
	std::queue<PCB*> list; //TODO: possible changes due to PCB implementation; change to std::shared_ptr

	void sleep(); //TODO: update to reflect the PCB implementation, might be REDUNDANT
	void wakeup(std::shared_ptr<PCB> w_pcb); //update to reflect the PCB implementation, might be REDUNDANT
public:
	Semaphore(int new_value);
	void wait(); //TODO: uninterrupted execution
	void signal();
};