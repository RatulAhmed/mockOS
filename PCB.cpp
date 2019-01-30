/*
 * Ratul Ahmed
 * PCB.cpp 
 * Process Control BLock Implementation File
 *
 *
*/

#include <string>
#include "PCB.h"



PCB::PCB()
{
	pid = 1;
	cpu_status = false;
	disk_status = false;
	preemption_flag = false;
	disk_number = -1;
	priority_level = -1;
	time_quantum_left = -1;
	file_name = "";
}



/*
 * The OS keeps track of available pids, created new 
 * PCB objects and pushes immediately onto RQ
 * we check where is really belongs later
 */
PCB::PCB(int assigned_pid)
{
	pid = assigned_pid;
	cpu_status = false;      // default not in cpu
	disk_status = false;     // default not in disk
	preemption_flag = false; // default it's not preempted obv
	disk_number = -1;          // -1 is a marker to indicate not in disk
	priority_level = 0;      // new processes are default highest priority
	time_quantum_left = 3;   // default there are 3 time quantums 
							 // in level 0, 1 TQ allowed,  in level 1 - 2 TQs
	file_name = "";
}  


int PCB::getPriority() const { return priority_level; }

bool PCB::getDiskStatus() const { return disk_status; }

bool PCB::getCPUStatus() const { return cpu_status; }

int PCB::getTimeQuantum() const { return time_quantum_left; }

int PCB::getPID() const { return pid; }

std::string PCB::getFileName() const { return file_name; }

void PCB::setPID(int i)
{
	pid = i;
}

void PCB::setPreemptedFlag(bool status)
{
	preemption_flag = status;
} 

void PCB::setDiskStatus(bool status) 
{
	disk_status = status;
}
void PCB::setCPUStatus(bool status)
{
	cpu_status = status;
}

void PCB::setDiskNum(int disk_num)
{
	disk_number = disk_num;
}

void PCB::setFileName(std::string _file_name) 
{
	file_name = _file_name;
}

void PCB::setPriority(int priority)
{
	priority_level = priority;
}

void PCB::decrementTQ() 
{
	time_quantum_left--;
}



