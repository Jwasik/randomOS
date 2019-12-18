#pragma once
#include "PCB.h"
#include <mutex>
#include <memory>
#include <queue>

class Semaphore //TODO: output for step-by-step execution
{
private:
	std::mutex mtx; //mutex locks provide protection for the constructor, wait() and signal() methods without negative impacts for multithreaded systems
	int value;
	std::queue<std::shared_ptr<PCB>> list;

public:
	Semaphore(int new_value);
	bool wait(); //returns 1 if a semaphore is down, 0 if it's not
	void signal();

	const int& getValue();
	const std::queue<std::shared_ptr<PCB>>& getList();
};