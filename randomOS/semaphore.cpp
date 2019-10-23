#include "semaphore.h"

void Semaphore::sleep()
{
}

void Semaphore::wakeup(std::shared_ptr<PCB> w_pcb)
{
}

Semaphore::Semaphore(int new_value)
{
	value = new_value; //equivalent to std::atomic.store()
}

void Semaphore::wait()
{
	mtx.lock; //mutex locks provide protection for critical sections of the semaphores
	value--;
	if (value < 0)
	{
		list.push(&pcb);
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
