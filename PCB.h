/*
 * Ratul Ahmed
 * PCB.h 
 * Header File for PCB object
 *
*/

#ifndef PCB_H
#define PCB_H

#include <string>

class PCB
{

private:
	int pid;  			   //Process ID 
	bool cpu_status;       // True if using CPU;
	bool disk_status;      // True if using disk;
	bool preemption_flag;
	int disk_number;      // -1 if not in a disk 
	int priority_level;
	int time_quantum_left; //default 3 time quantums? or maybe start at 0
	std::string file_name;

public:
	/*
	 * Default constructor
	 */
	PCB();

	PCB(int assigned_pid);
	//PCB(int _pid, bool _cpu_status, bool _disk_status, int disk_number, int _time_quantum);

	int getPriority() const;

	bool getDiskStatus() const;

	bool getCPUStatus() const;

	int getTimeQuantum() const; 

	std::string getFileName() const;

	// -1 indicated deleted pid (termianted)
	void setPID(int i);

	int getPID() const;

	void setFileName(std::string _file_name);

	void setPreemptedFlag(bool status); 

	void setDiskStatus(bool status);

	void setCPUStatus(bool status);

	void setDiskNum(int disk_num);

	void setPriority(int priority);

	void decrementTQ();





};


#endif PCB_H