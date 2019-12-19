#include "Scheduler.h"

Scheduler::Scheduler()
{ 
	counter = 0;
}

Scheduler::~Scheduler()
{
}

uint8_t Scheduler::schedule() //
{
	counter++;
	if (counter > 1000000)
	{
		counter = 0;
	}

	if (RUNNING.counter == counter || RUNNING = nullptr)
	{
		this->result = nextProcess();
		if (this->result != 0)
			return result;
		

		return 0;
	}

	return 0;
	return 27;
}

uint8_t Scheduler::nextProcess()
{
	// missing 

	if (RUNNING != nullptr)
	{
		this->addProcess(RUNNING, &this->expired);
	}

	if (this->active.size() == 0)
	{
		if (this->expired.size() == 0)
		{
			RUNNING = //dodaje dummy;
				return 0;
		}

		this->active = this->expired;
		this->expired.clear();
	}

	RUNNING = this->active[0];
	this->active.erase(this->active.begin());
	if (RUNNING.state == "waiting")
	{
		result = this->nextProcess();
		return 0;
	}

	this->result = normalProcessPriorityAndTimerChange(RUNNING);
	if (this->result != 0)
		return result;

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
		if (expired[i].priority > process.priority)
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

uint8_t Scheduler::normalProcessPriorityAndTimerChange(std::shared_ptr<PCB> process)
{
	int waitingTime = this->counter - process->counter;
	if (waitingTime < 0)
		waitingTime += 1000000;

	
	int bonus = 0.1 * waitingTime;
	int previousPriority = process.priority;

	process->priority = process->basePriority + 5 - bonus;

	if (process->priority > 139)
	{
		process->priority = 139;
	}
	if (process->priority < 100)
	{
		process->priority = 100; //lol skjsvbjksvbjkskbjsv
	}
	if (previousPriority < 120)
	{
		process->counter = this->counter + ((140 - previousPriority) * 2);
	}
	else
	{
		process->counter = this->counter + ((140 - previousPriority) * 0.5);

	}

	return 0;
	return 27;
}


