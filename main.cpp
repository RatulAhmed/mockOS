/* 
 * Ratul Ahmed 
 * Main driver for Operating System  
 * CSCI 340 Section 3? T-F 3:45 - 5:00
 *
*/ 
 
#include <iostream> 
#include <string>

#include "OperatingSystem.h"
#include "ReadyQueue.h"
#include "PCB.h"


int main() 
{
	long RAM;
	int page_size; // also frame size
	int num_of_disks;


	// Get the needed inputs from the user here
	// Ram Size up to 4 Billion (Bytes), num of disks, page size = frame size
	std::cout << "Enter the amount of RAM in the system" << std::endl;
	std::cin >> RAM;

	std::cout << "Enter the amount of Hard Disks in the system" << std::endl;
	std::cin >> num_of_disks;

	std::cout << "Enter the page/frame size in the system" << std::endl;
	std::cin >> page_size;

	// now create an "OperatingSystem" with these specifications

	OperatingSystem OS(RAM,num_of_disks,page_size);

	OS.runOS();

//	OS.newProcess();
//	OS.newProcess();
//	OS.newProcess();

//	OS.snapshotRQ();

//	OS.snapshotRQ();

}


