#include "MemoryScanner.hpp"

// Ctor
MemoryScanner::MemoryScanner(int pid, HANDLE handle) : Process(pid) {
    this->_procHandle = handle;
}


void MemoryScanner::init() {
	calcPageSizeAndMaxRange();
	queryVirtualMemoryRanges();
}


void MemoryScanner::calcPageSizeAndMaxRange() {
	// Calculate the page size and max virtual address space
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	_pageSize = si.dwPageSize;
	_maxVirtualRange = si.lpMaximumApplicationAddress;
}

void MemoryScanner::queryVirtualMemoryRanges() {
    // We will iterate over the entire memory range and insert the ranges into the map of ranges

    uintptr_t address = 0; // Starting at 0
    MEMORY_BASIC_INFORMATION mbi;

    // We will iterate over each region, we will only check MEM_COMMIT and RW areas
    while (VirtualQueryEx(_procHandle, (LPCVOID)address, &mbi, sizeof(mbi)) != 0) {

        if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE) {
            _ranges.insert({ mbi.BaseAddress, mbi.RegionSize });
        }

        uintptr_t nextAddress = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;

        if (nextAddress <= address || (LPCVOID)nextAddress >= _maxVirtualRange) {
            break;
        }

        address = nextAddress;
    }


}


void MemoryScanner::getUserInput()
{

}