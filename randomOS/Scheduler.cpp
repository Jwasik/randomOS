#include "Scheduler.h"

Scheduler::Scheduler()
{ 
	caunter = 0;
}

Scheduler::~Scheduler()
{
}

uint8_t Scheduler::schedule()
{
	caunter++;
	if()
	return uint8_t();
}

uint8_t Scheduler::nextProcess()
{
	if (RUNNING == NULL)
		return  ; // where is dummy?

	if (this->ready.size() == 0)
	{
		if (this->waiting.size())
		{
			RUNNING = //dodaje dummy;
				return 0;
		}

		this->ready = this->waiting;
		this->waiting.clear();
	}

	this->addProcess(RUNNING, &this->waiting);
	RUNNING = this->ready[0];
	this->ready.erase(this->ready.begin());
	return 0;

	return 27; // b³¹d: WTF?
}

uint8_t Scheduler::addProcess(std::shared_ptr<PCB> process, std::vector<std::shared_ptr<PCB>> *queue)
{
	if (process.priority > 139 || process.priority < 100)
		return ; //priority out of range <100, 139>
	if (process == NULL)
		return ; //process does not exist

	for (int i = 0; i < queue->size(); i++)
	{
		if (waiting[i].priority > process.priority)
		{
			queue->insert(queue->begin() + i, process);
			return 0;
		}
	}
	if (queue->size() == 0)
	{
		queue->push_back(process);
		return 0;
	}
	return 27; // b³¹d: WTF?
}
