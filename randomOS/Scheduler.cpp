#include "Scheduler.h"

Scheduler::Scheduler()
{ 
	this->active = std::make_shared<std::vector<std::shared_ptr<PCB>>>();
	this->expired = std::make_shared<std::vector<std::shared_ptr<PCB>>>();
	counter = 0;
}

Scheduler::~Scheduler()
{
}

uint8_t Scheduler::schedule()
{

	//INCREMENT THE COUNTER
	counter++;
	if (counter > 1000000) { counter = 0; } //to avoid overflow

	if  (RUNNING == nullptr || RUNNING->counter <= this->counter )
	{
		return nextProcess();
	}

	return 0;
}

uint8_t Scheduler::nextProcess()
{
	//move the currently running process into the expired queue and remove it from active
	if (RUNNING != nullptr){ 
		RUNNING->setStateReady();
		//if it was dummy don't put it back into expired (wastes processor time)
		if(RUNNING != DUMMY){ this->addProcess(RUNNING, this->expired); }
		this->active->erase(this->active->begin());
	}
	
	//check if the queueues should be swapped
	if (this->active->size() == 0)
	{
		if (this->expired->size() == 0)
		{
			RUNNING = DUMMY;
			RUNNING->setInstructionCounter(0);
			RUNNING->setStateRunning();
			this->addProcess(RUNNING,NULL);
			return 0;
		}
		for (int i = 0; i < expired->size(); i++) { active->push_back(expired->at(i)); }
		this->expired->clear();
	}

	//set the process on top of teh active queue as running
	RUNNING = (*this->active)[0];
	if (RUNNING == nullptr) nextProcess();



	//if the now running porocess is under a semaphore, switch to next
	if (RUNNING->getStateAsEnum() == PCB::ProcessState::WAITING)
	{
		return this->nextProcess();
	}
	//else set its state as running
	RUNNING->setStateRunning();
	return normalProcessPriorityAndTimerChange(RUNNING);
}

uint8_t Scheduler::addProcess(std::shared_ptr<PCB> process, std::shared_ptr<std::vector<std::shared_ptr<PCB>>> queue)
{
	//if the queue is not specified (passed as null) the active queue is assumed
	if (queue == NULL) { queue = this->active; }
	
	//error checks
	if (process->priority > 139 || process->priority < 100) { return ERROR_SH_PRIORITY_OUT_OF_RANGE; } //priority out of range <100, 139>
	if (process == NULL) { return ERROR_SH_ADDED_PROCESS_DOES_NOT_EXIST; } //process does not exist

	//position the process at a appropriate place in the queue
	/// iterate through the queue and put it just before an element with higher priority
	for (int i = 0; i < queue->size(); i++)
	{
		if ((*queue)[i]->priority > process->priority && !(*queue)[i]->getIsRunning())
		{
			queue->insert(queue->begin() + i, process);
			normalProcessPriorityAndTimerChange(process);
			return 0;
		}
	}
	///if the palce hasn't been found during iteration, either the queue is empty or it should be the last element, so just push it back
	queue->push_back(process);

	normalProcessPriorityAndTimerChange(process);
	return 0; 
}

uint8_t Scheduler::normalProcessPriorityAndTimerChange(std::shared_ptr<PCB> process)
{
	int waitingTime = this->counter - process->counter;
	if (waitingTime < 0)
		waitingTime += 1000000;

	
	int bonus = 0.1 * waitingTime;
	int previousPriority = process->priority;

	process->priority = process->basePriority + 5 - bonus;

	//handling priority over and under flow
	if (process->priority > 139){ process->priority = 139;}
	if (process->priority < 100){ process->priority = 100;}

	//ASSIGN COUNTER
	//if it's dummy just assign 2 processor tics to it
	if (process->getHasPID(0)) { process->counter = this->counter + DUMMY_TICS; return 0; }

	if (previousPriority < 120)
	{
		process->counter = this->counter + ((140 - previousPriority) * 2);
		return 0;
	}
	//else
	process->counter = this->counter + ((140 - previousPriority) * 0.5);
	return 0;
	

}