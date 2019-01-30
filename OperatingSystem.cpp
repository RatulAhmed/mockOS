/* 
 * Ratul Ahmed 
 * OperatingSystem.cpp
 * This object will contain the SysGen
 * and will use that to create the device queues
 *
*/ 



#include "OperatingSystem.h"
#include "ReadyQueue.h"
#include "PCB.h"
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <sstream>

OperatingSystem::OperatingSystem(long ram, int disk, int page) : disk_queue(disk), multi_level_queue()
{
	total_RAM = ram;
	available_pid = 1;
	num_of_hard_disk = disk;
	page_size = page;
	frame_size = page;
}



void OperatingSystem::newProcess()
{	
	PCB temp(available_pid);
	multi_level_queue.enqueueNewPCB(temp);
	available_pid++;
}


void OperatingSystem::timeQuantumFinished()
{
	multi_level_queue.updateRunningTQ();

}

void OperatingSystem::terminateProcess()
{
	multi_level_queue.terminate();
}


void OperatingSystem::requestDisk(int num, std::string file_name) 
{
	PCB temp = multi_level_queue.dequeueToDisk();
	temp.setFileName(file_name);
	disk_queue[num].push_back(temp);
}

void OperatingSystem::diskFinished(int disknumber) 
{	
	PCB temp = disk_queue[disknumber].front();
	disk_queue[disknumber].pop_front();
	multi_level_queue.enqueueFromDisk(temp);
}

void OperatingSystem::snapshotRQ() const
{
	multi_level_queue.print();
}



void OperatingSystem::snapshotDisk() const
{

}

// Possible strings; A, Q, t, d num file, D number, m address, S r, S i, S m
void OperatingSystem::runOS()
{
	std::string input_string;
	int disk_num;
	std::string file_name;
	char throwaway;
	bool flag = true;
	std::stringstream ss;

	while(flag)
	{
		std::getline(std::cin, input_string);
		if(input_string == "A")
		{
			newProcess();
		}
		if (input_string == "Q")
		{
			timeQuantumFinished();
		}
		if(input_string == "t")
		{
			terminateProcess();
		}
		if(input_string == "S r")
		{
			snapshotRQ();
		}
		if(input_string == "S i")
		{
			snapshotDisk();
		}
		if(input_string == "S m")
		{

		}
	}
	
}


























