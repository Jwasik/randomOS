#include "semaphore.h"

Semaphore::Semaphore(int new_value)
{
	value = new_value;
}

Semaphore::Semaphore()
{
	value = 0;
}

bool Semaphore::wait()
{
	value--;
	if (value < 0)
	{
		list.push(RUNNING);
		RUNNING->setStateWaiting();
		return 1;
	}
	return 0;
}

void Semaphore::signal() 
{
	value++;
	if (value <= 0)
	{
		list.front()->setStateRunning();
		list.pop();
	}
}

const int & Semaphore::getValue()
{
	return value;
}

const std::queue<std::shared_ptr<PCB>>& Semaphore::getList()
{
	return list;
}
