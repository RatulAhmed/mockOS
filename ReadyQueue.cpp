/* 
 * Ratul Ahmed 
 * The Ready Queue
 * 
 * Accepts PCB objects 
*/ 

#include "PCB.h"
#include "ReadyQueue.h"


ReadyQueue::ReadyQueue()
{
	level0_size = 0;
	level1_size = 0;
	level2_size = 0;

	running_process_level = 0;

}



void ReadyQueue::enqueueNewPCB(PCB & new_PCB)
{
	level0.push_back(new_PCB);

	// if there is nothing in any of the deques
	// this is the first process
	if(getL0Size() == 0 && getL1Size() == 0 && getL2Size() == 0)
	{
	
	// Since first process, it immediately uses cpu

	level0.front().setCPUStatus(true);
	incrSizeL0();
	running_process_level = 0;
	}

	// A Process is already using the CPU from level0
	// bc we signify the front of the deque as running
	// This Process simply enters the deque and waits 
	else if(getL0Size() > 0)
	{
		incrSizeL0();
		running_process_level = 0;
	}

	// else L0 is empty and L1 is nonempty then
	// front of l2 is curretly using process
	// regardl
	else if(getL0Size() == 0 && getL1Size() > 0)
	{
		level0.front().setCPUStatus(true);
		incrSizeL0();

		//we must preempt the level1 process using cpu
		// the process stays at front if preempted		
		level1.front().setPreemptedFlag(true);
		level1.front().setCPUStatus(false);

		running_process_level = 0;
	}
	//else new process comes while l2 process using queue
	else if(getL0Size() == 0 && getL1Size() == 0 && getL2Size() > 0)
	{
		level0.front().setCPUStatus(true);
		incrSizeL0();

		// don't change where the l2 pcb is
		// it got preempted so stays at front of l2 queue
		level2.front().setPreemptedFlag(true);
		level2.front().setCPUStatus(false);

		running_process_level = 0;
	}
}

void ReadyQueue::enqueueFromDisk(PCB & disk_PCB)
{
	if(disk_PCB.getPriority() == 0)
	{
		// this will check if we preempt for lower processes
		enqueueNewPCB(disk_PCB);

	}

	// the process returning from disk has level 1 priority
	if(disk_PCB.getPriority() == 1)
	{
		// The level 0 queue has items
		// so we just enqueue and leave it
		if(getL0Size() > 0)
		{
			level1.push_back(disk_PCB);
			incrSizeL1();
			level1.back().setDiskStatus(false);
		}

		//all the levels are empty and no preemption needed
		if(getL0Size() == 0 && getL1Size() == 0  && getL2Size() == 0)
		{
			level1.push_back(disk_PCB);
			incrSizeL1();
			level1.front().setDiskStatus(false);   //front == back here
			level1.front().setCPUStatus(true); // this process is now running
			running_process_level = 1;
		}

		// L0 is empty and there's a process running for L1
		if(getL0Size() == 0 && getL1Size() > 0)
		{
			level1.push_back(disk_PCB);
			incrSizeL1();
			level1.back().setDiskStatus(false);
			level1.back().setCPUStatus(false);
		}

		// l0 l1 empty, l2 process running and we must preempt it
		if(getL0Size() == 0 && getL1Size() == 0 && getL2Size() > 0)
		{
			level2.front().setCPUStatus(false);
			level2.front().setPreemptedFlag(true);
			level1.push_back(disk_PCB);
			level1.front().setCPUStatus(true);
			level1.front().setDiskStatus(false);
			incrSizeL1();
		}

	}

	if(disk_PCB.getPriority() == 2)
	{
		// if there are elements in L1 or L0 then we just enqueue
		if(getL1Size() > 0 || getL0Size() > 0)
		{
			level2.push_back(disk_PCB);
			level2.back().setDiskStatus(false);
			incrSizeL2();
		}

		if(getL0Size() == 0 && getL1Size() == 0 && getL2Size() == 0)
		{
			level2.push_back(disk_PCB);
			incrSizeL2();
			level2.front().setCPUStatus(true);
			level2.front().setDiskStatus(false);
		}

		if(getL0Size() == 0 && getL1Size() == 0 && getL2Size() > 0)
		{
			level2.push_back(disk_PCB);
			incrSizeL2();
		}
		
	}
}

// we are updating the current running processes TQ and making the
// correct movements of the queue
void ReadyQueue::updateRunningTQ()
{
	// The current running process is in level0
	// it only gets 1 TQ and it moves down to back of 
	// L1 queue
	if(running_process_level == 0)
	{
		level0.front().setCPUStatus(false);
		level0.front().setPreemptedFlag(true);
		level0.front().setPriority(1);
		level0.front().decrementTQ();        //2 remaining TQ

		// update the running process level
		running_process_level = 1;
		level1.push_back(level0.front());
		level0.pop_front();
		level1.back().setCPUStatus(false);
		level1.back().setPriority(1);
		decrSizeL0();
		incrSizeL1();

		// check which level process should be running now
		// if L1 is now empty this level1 process runs
		if(getL0Size() == 0)
		{
			level1.front().setCPUStatus(true);
			running_process_level = 1;
		}

		//if we still have PCBS in level 0 we run the front
		if(getL0Size() > 0)
		{
			level0.front().setCPUStatus(true);
			running_process_level = 0;
		}
	}

	// current running process is in level 1
	if(running_process_level == 1) 
	{
		level1.front().decrementTQ();

		if(level1.front().getTimeQuantum() == 1)
		{
			//we don't have to do anything
			// the process has 1 remaining TQ in level 1
		}
		if(level1.front().getTimeQuantum() == 0)
		{
			level2.push_back(level1.front());
			level1.pop_front();
			level2.back().setCPUStatus(false);
			level2.back().setPriority(2);
			decrSizeL1();

			incrSizeL2();

			//now level 1 could be empty
			if(getL1Size() == 0)
			{
				level2.front().setCPUStatus(true);
				running_process_level = 2;
			}
			// Level 1 is not empty
			else if(getL1Size() > 0 && getL0Size() == 0)
			{
				level1.front().setCPUStatus(true);
				running_process_level = 1;
			}
		}
	}
}


void ReadyQueue::terminate() 
{


	// Current running process is in level 0
	// We need to check if terminatng makes level 0 empty
	// or just run next level 0 process
	if(running_process_level == 0)
	{

		if(getL0Size() == 0)
		{
			std::cout << "No process to terminate";
			return;
		}

		// First we "terminate the process"
		level0.front().setCPUStatus(false);
		level0.front().setPID(-1);
		decrSizeL0();
		level0.pop_front();

		// L0 is empty
		if( getL0Size() == 0 )
		{
			//check if L1 is empty
			if(getL1Size() > 0)
			{
				level1.front().setCPUStatus(true);
				running_process_level = 1;
			}
			else if(getL2Size() > 0 && getL1Size() == 0)
			{
				level2.front().setCPUStatus(true);
				running_process_level = 2;
			}
		}
		else if (getL0Size() > 0)
		{
			level0.front().setCPUStatus(true); 
			running_process_level = 0;
		}
	}

	if(running_process_level == 1)
	{
		level1.front().setCPUStatus(false);
		level1.front().setPID(-1);
		decrSizeL1();
		level1.pop_front();

		if(getL1Size() == 0)
			level2.front().setCPUStatus(true);
			running_process_level = 2;
		if(getL1Size() > 0)
		{
			level1.front().setCPUStatus(true);
			running_process_level = 1;
		}
	}

	if(running_process_level == 2) 	{
		level2.front().setCPUStatus(false);
		level2.front().setPID(-1);
		decrSizeL2();
		level2.pop_front();

		if(getL2Size() > 0)
		{
			level2.front().setCPUStatus(true);
			running_process_level = 2;
		}

	}
}


// Dequeue the currently running process
// and return it so the OS can move it do disk
// make the adjustments to multi level queue
PCB& ReadyQueue::dequeueToDisk() 

{
	PCB returnPCB;
	
	/// dequeue from level 0
	if(running_process_level == 0)
	{
		level0.front().setCPUStatus(false);
		returnPCB = level0.front();
		level0.pop_front();
		decrSizeL0();

		if(getL0Size() > 0)
		{
			level0.front().setCPUStatus(true);
			running_process_level = 0;
		}
		else if(getL2Size() > 0 && getL1Size() == 0)
		{
				level2.front().setCPUStatus(true);
				running_process_level = 2;
		}
		// the process in L1 should run
		else if(getL0Size() == 0 && getL1Size() > 0)
		{
			level1.front().setCPUStatus(true);
			running_process_level = 1;
		}
	}


	if(running_process_level == 1)
	{
		level1.front().setCPUStatus(false);
		decrSizeL1();
		returnPCB = level1.front();
		level1.pop_front();

		if(getL1Size() == 0)
			level2.front().setCPUStatus(true);
			running_process_level = 2;
		
		if(getL1Size() > 0)
		{
			level1.front().setCPUStatus(true);
			running_process_level = 1;
		}
	}

	if(running_process_level == 2) 	
	{
		level2.front().setCPUStatus(false);
		decrSizeL2();
		returnPCB = level2.front();
		level2.pop_front();

		if(getL2Size() > 0)
		{
			level2.front().setCPUStatus(true);
			running_process_level = 2;
		}

	}
	returnPCB.setDiskStatus(true);
	return returnPCB;
}




// moving an object from level0 queue to 1
// This means Q has been called prior and a tme quantum
// has passed
/*void ReadyQueue::moveDownToL1()
{

}

// Q has been called on the current process twice
// 2 time quantums has passed and moved down to L2
void ReadyQueue::moveDownToL2()
{

}

int ReadyQueue::level1_TQ()
{
	return level1.front().getTimeQuantum();
}


*/



int ReadyQueue::getL0Size() const { return level0_size; }
int ReadyQueue::getL1Size() const { return level1_size; }
int ReadyQueue::getL2Size() const { return level2_size; }

void ReadyQueue::incrSizeL0() { level0_size++; }
void ReadyQueue::incrSizeL1() { level1_size++; }
void ReadyQueue::incrSizeL2() { level2_size++; }


void ReadyQueue::decrSizeL0() { level0_size--; }
void ReadyQueue::decrSizeL1() { level1_size--; }
void ReadyQueue::decrSizeL2() { level2_size--; }

/*
void ReadyQueue::updateL0_TQ() 
{
	level0[level0_size - 1].decrementTQ();
}

void ReadyQueue::updateL1_TQ()
{
	level1_size[level1_size - 1].decrementTQ();
}

*/





// ********* PRINT FUNCTION ********
// ****** Use this for [S r] command  ******
void ReadyQueue::print() const
{


	// ******* PRINT LEVEL 0 QUEUE *********
	std::cout << "Level 0: ";
	for(int i = 0; i < getL0Size(); i++)
	{
		if(level0[i].getCPUStatus())
		{
			// don't print
		}
		else 
		{
		std::cout << "P" << level0[i].getPID() << " ";
		}
	}
	std::cout << "\n";

	// ******* PRINT LEVEL 1 QUEUE ********
	std::cout << "Level 1: ";
	for(int i = 0; i < getL1Size(); i++)
	{
		if(level1[i].getCPUStatus())
		{
			//don't print
		}
		else
		{
		std::cout << "P" << level1[i].getPID() << " ";
		}
	}
	std::cout << "\n";
	std::cout << "Level 2: ";


	// ******* PRINT LEVEL 2 QUEUE ***********
	for(int i = 0; i < getL2Size(); i++)
	{
		if(level2[i].getCPUStatus())
		{
			// dont print 
		}
		else 
		{
		std::cout << "P" << level2[i].getPID() << " ";
		}
	}

	std::cout << "\n";

	// *** PRINT OUT THE CURRENT RUNNING PROCESS & IT'S LEVEL *****
	if(running_process_level == 0)
	{
		
		std::cout << "CPU: P" << level0.front().getPID() << " from Level " << level0.front().getPriority() << std::endl;
		
	}	

	if(running_process_level == 1)
	{

		std::cout << "CPU: P" << level1.front().getPID() << " from Level " << level1.front().getPriority() << std::endl;
	}
	

	if(running_process_level == 2)
	{

		std::cout << "CPU: P" << level2.front().getPID() << " from Level " << level2.front().getPriority() << std::endl;
		
	}


//	std::cout << "CPU STATUS OF FRONT : " << level0.front().getCPUStatus() << std::endl;
//	std::cout << "THE PROCESS AT FRONT IS: PROCESS " << level0.front().getPID() << std::endl;
}

















