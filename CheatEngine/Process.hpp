#ifndef PROCESS_HPP
#define PROCESS_HPP

// Includes
#include <iostream>
#include <Windows.h>


class Process {
public:
	Process(int pid); // Ctor
	~Process(); // Dtor

	// Methods
	void openHandleOnProcess();
	HANDLE getHandle();
	void setHandle(HANDLE handle);

private:
	int _pid;
	HANDLE _procHandle;
};



#endif // PROCESS_HPP