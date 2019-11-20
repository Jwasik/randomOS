#include "semaphore.h"

void Semaphore::sleep()
{
}
void Semaphore::wakeup(std::shared_ptr<PCB> pcbPtr)
{
}

Semaphore::Semaphore(int new_value)
{
	mtx.lock;
	value = new_value;
	mtx.unlock;
}

void Semaphore::wait()
{
	mtx.lock;
	value--;
	if (value < 0)
	{
		list.push(pcbPtr);
		sleep();
	}
	mtx.unlock;
}

void Semaphore::signal() 
{
	mtx.lock;
	value++;
	if (value <= 0)
	{
		wakeup(list.front);
		list.pop;
	}
	mtx.unlock;
}

const int & Semaphore::getValue()
{
	return value;
}

const std::queue<std::shared_ptr<PCB>>& Semaphore::getList()
{
	return list;
}
