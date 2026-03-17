#include "Process.hpp"
#include "MemoryScanner.hpp"

// Ctor
Process::Process(int pid) : _procHandle(NULL), _pid(pid), _memoryscanner(nullptr) {}

// Dtor
Process::~Process() {

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


void Process::init() {
	// Initialise the object
	openHandleOnProcess();

	// Initialise memoryscanner
	_memoryscanner = new MemoryScanner(_pid, _procHandle);
	_memoryscanner->init();
}