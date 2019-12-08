#include "semaphore.h"
std::shared_ptr<PCB> pcb;//TODO: Delete once there's a global PCB available

Semaphore::Semaphore(int new_value)
{
	mtx.lock();
	value = new_value;
	mtx.unlock();
}

bool Semaphore::wait()
{
	mtx.lock();
	value--;
	if (value < 0)
	{
		list.push(pcb);
		pcb->setStateWaiting();
		return 1;
	}
	mtx.unlock();
	return 0;
}

void Semaphore::signal() 
{
	mtx.lock();
	value++;
	if (value <= 0)
	{
		list.front()->setStateRunning();
		list.pop();
	}
	mtx.unlock();
}

const int & Semaphore::getValue()
{
	return value;
}

const std::queue<std::shared_ptr<PCB>>& Semaphore::getList()
{
	return list;
}
