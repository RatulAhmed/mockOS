/* 
 * Ratul Ahmed 
 * Operating_System.h
 * The OperatingSystem call encapsulates all of the 
 * data structures used to execute the commands, process movements
 * and memory management
*/ 

#ifndef Operating_System_H
#define Operating_System_H

#include "PCB.h"
#include "ReadyQueue.h"
#include <vector>
#include <deque>
class OperatingSystem
{

private: 
	// Avaiable PID for next new PCB incrememt w new pid
	int available_pid;

	ReadyQueue multi_level_queue;
	std::vector<std::deque<PCB> > disk_queue; //this should be it's own class.. but more difficult for me
	long total_RAM;
	int num_of_hard_disk;
	int page_size;
	int frame_size;
//  CPUObject


public:
	OperatingSystem(long ram, int disks, int page);

	int getAvailablePid();

	void runOS();

// Commands for OS

/* 1) Create New process and do the appropriate stuff [A]
	  // create a new PCB, push it into RQ usuing new enqueue
	  // if it's the only one in the RQ and it's at level 0
	  // turn the cpu flag on
*/
	void newProcess();	


/*
 * 2) The Current Running Process has used a time quantum [Q]
 */
	void timeQuantumFinished();




/*	
 * 3) The Process has terminated [t] // current running process in cpu
*/
	void terminateProcess();



/*
 * 4) The Process in the CPU requests a harddisk #n/ wants to read write file_name [d num file_name]
 */
		void requestDisk(int disknumber, std::string file_name);
/*

 * 5) The hard disk has finished the work for a process [D num]
*/
		void diskFinished(int disknumber); 
/*
 * 6) Current process in CPU requests a mem operation for logical address [m address]
 */
 		void requestMem(int address);

 /*
 * 7) Snapshot of the CPU and Ready Queue (print statement) [S r]
 */
 		void snapshotRQ() const;
 /*
 * 8) Snpashot of the hard disks and it's queue [S i]
 */
 		void snapshotDisk() const;

 /*		
 * 9) Snapshot of memory, show used frames, process number occupying it, and the page num stored [S m]
 *
 */
 		void snapshotMem() const;


};



#endif Operating_System_H