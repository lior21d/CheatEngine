#include "Process.hpp"

// Ctor
Process::Process(int pid) : _procHandle(NULL), _pid(pid) {}

// Dtor
Process::~Process() {

	// Exit the process and try to also close the handle
	if (CloseHandle(_procHandle) == FALSE) {
		int errorCode = GetLastError();
		printf("Error Code: %d, Failed to close handle, exiting.", errorCode);
		ExitProcess(0);
	}
}


// Methods

void Process::openHandleOnProcess() {

	// Open a handle to the process
	_procHandle = OpenProcess(
		PROCESS_ALL_ACCESS, // dwDesiredAccess
		FALSE, // bInheritHandle
		_pid // dwProcessId
	);

	if (_procHandle == NULL) {
		int errorCode = GetLastError();
		printf("Error Code: %d, Failed to open handle on process, exiting.", errorCode);
		ExitProcess(0);
	}
}


HANDLE Process::getHandle() {
	return _procHandle;
}


void Process::setHandle(HANDLE handle) {
	_procHandle = handle;
}