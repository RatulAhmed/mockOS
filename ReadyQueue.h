/* 
 * Ratul Ahmed 
 * The Ready Queue
 * 
 * Accepts PCB objects 
 * internally implemented with deques
*/ 

#ifndef ReadyQueue_H
#define ReadyQueue_H


#include <deque>
#include <iostream>
#include "PCB.h"


//The First item in the queue is in the CPU
//CPU is empty only if queue is empty
class ReadyQueue
{


public:

	ReadyQueue(); 
	
	void enqueueNewPCB(PCB & new_PCB);

	void enqueueFromDisk(PCB & disk_PCB);

	void updateRunningTQ();

	void terminate(); 

	PCB& dequeueToDisk();



// helpers 
	int getL0Size() const;
	int getL1Size() const;
	int getL2Size() const;

	void incrSizeL0();
	void incrSizeL1();
	void incrSizeL2();

	void decrSizeL0();
	void decrSizeL1();
	void decrSizeL2();
	
	void print() const;



private:

	// If an item is running with CPU it's just at the front of the queue
	// and we update bool values
	// I used a dequeue instead of a list because it gives access to front and back like list
	// and the [] operator which list does not have, makes life easier

	// front is 0 back it l[size-1]
	std::deque<PCB> level0; // Round Robin Scheduler
	std::deque<PCB> level1;  // Round Robin Scheduler w/ 2 time quantums 
	std::deque<PCB> level2;  //FCFS

	int level0_size;
	int level1_size;
	int level2_size;
	
	int running_process_level;

	/*
	 * level[size-1] represents the item in the CPU 
	 * depending on the contents of the levels
	 */
};






#endif ReadyQueue_H