#include "semaphore.h"

Semaphore::Semaphore(int new_value)
{
	value = 0;
}

Semaphore::Semaphore()
{
	value = 0;
}

bool Semaphore::wait()
{
	bool exist = false;

	for (auto &i : list)
	{
		if (i == RUNNING) exist = true;
	}
	if(value < 0 && exist==false)
	{
		list.push_back(RUNNING);
		RUNNING->setStateWaiting();
		value--;
	}
	else if (exist)
	{
		RUNNING->setStateWaiting();
		return 1;
	}
	if (list.empty())
	{
		value--;
		list.push_back(RUNNING);
	}
	return 0;
}

void Semaphore::deleteFromList(int PID)
{
	for (int i=0;i<list.size();i++)
	{
		if (list[i]->getPID() == PID)
		{
			list.erase(list.begin() + i);
			value++;
			break;
		}
	}
}

int Semaphore::signal() 
{
	if (value < 0)
	{
			list.erase(list.begin());
			if (!list.empty()) 
			{
				list[0]->setStateReady();
				value++;
				return list[0]->getPID();
			}
			value++;
			if (list.size() == 1) value = 0;
	}
	return -1;
}

bool Semaphore::firstElement(unsigned int PID)
{
	
	if (list.empty() || list.front()->getPID() == PID)
	{
		return true;
	}
	return false;
}

const int & Semaphore::getValue()
{
	return value;
}

const std::vector<std::shared_ptr<PCB>>& Semaphore::getList()
{
	return list;
}
