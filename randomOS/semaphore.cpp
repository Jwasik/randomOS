#include "semaphore.h"

Semaphore::Semaphore(int new_value)
{
	value = 1;
}

Semaphore::Semaphore()
{
	value = 1;
}

bool Semaphore::wait()
{
	if(value >= 0)value--;
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
