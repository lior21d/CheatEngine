#ifndef PROCESS_HPP
#define PROCESS_HPP

// Includes
#include <iostream>
#include <Windows.h>

// Forward decleration
class MemoryScanner;

class Process {
public:
	Process(int pid); // Ctor
	virtual ~Process(); // Dtor

	// Methods

	void openHandleOnProcess();
	HANDLE getHandle();
	void setHandle(HANDLE handle);
	void init();

protected:
	HANDLE _procHandle;

private:
	int _pid;
	MemoryScanner* _memoryscanner;
};



#endif // PROCESS_HPP