#include "MemoryScanner.hpp"

// Ctor
MemoryScanner::MemoryScanner(int pid, HANDLE handle) : Process(pid) {
    this->_procHandle = handle;
    _firstScan = TRUE;
}


void MemoryScanner::init() {
	calcPageSizeAndMaxRange();
	queryVirtualMemoryRanges();
    scanProcess();
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


void MemoryScanner::typesCreate() {
    _types.insert({ "int", 4 });
    _types.insert({ "string", 0 });
}


void MemoryScanner::getUserInput()
{
    if (_firstScan == TRUE)
    {
        // Get user input
        std::string type;
        printf("Enter your type of variable:\n(int, string)\n");
        std::cin >> type;

        // Create the map
        typesCreate();

        // Iterate over them and see what the user chose
        for (const auto& pair : _types)
        {
            if (pair.first == type)
            {
                _chosenType = pair;
            }

        }
    }


    if (_chosenType.first == "string") {
        
        // Put into buffer
        printf("Enter your string: ");
        std::string input;
        std::cin >> input;

        // Initialise buffer with the hex value
        _chosenType.second = input.length();
        _value = std::vector<unsigned char>(_chosenType.second);
        std::memcpy(_value.data(), input.data(), _chosenType.second);

        // Enter new value
        if (_firstScan == TRUE) {
            printf("Enter new value: ");
            std::cin >> input;
            _newValue = std::vector<unsigned char>(_chosenType.second);
            std::memcpy(_newValue.data(), input.data(), _chosenType.second);
        }
    }

    else if (_chosenType.first == "int")
    {
        printf("Enter your int: ");
        int input;
        std::cin >> input;

        _value = std::vector<unsigned char>(_chosenType.second);
        std::memcpy(_value.data(), &input, _chosenType.second);

        // Enter new value
        if (_firstScan == TRUE) {
            printf("Enter new value: ");
            std::cin >> input;
            _newValue = std::vector<unsigned char>(_chosenType.second);
            std::memcpy(_newValue.data(), &input, _chosenType.second);
        }
    }

    _firstScan = FALSE;
}


void MemoryScanner::scanProcess() {
    getUserInput();

    // Iterate over ranges
    std::vector<unsigned char> buffer(_pageSize);
    uintptr_t offset = 0;

    for (const auto& range : _ranges) {
        
        // Reset offset
        offset = 0;

        // Calculate how many pages are in the range
        int pageAmount = range.second / _pageSize;

        // Iterate over each page in the range
        for (int i = 0; i < pageAmount; i++) {
            scanPage(range.first, offset, buffer);
            offset = offset + (uintptr_t)_pageSize;
        }
    }

    // Now we probably have many addresses with the value so now we run it again until we
    // only have one
    findValueFinal();
    overwriteValue();
}


void MemoryScanner::scanPage(LPCVOID baseAddress, uintptr_t offset, std::vector<unsigned char>& buffer) {
    // Calculate address to scan
    LPCVOID address = (LPCVOID)((uintptr_t)baseAddress + offset);

    // Scan it and put into buffer
    BOOL read = ReadProcessMemory(
        _procHandle, // hProcess
        address, // lpBaseAddress
        buffer.data(), // lpBuffer
        _pageSize, // nSize
        NULL // lpNubmerOfBytesRead
    );
   
    // Try to find the value in it
    findValueFirstScan(buffer, address);
}

void MemoryScanner::findValueFirstScan(std::vector<unsigned char>& buffer, LPCVOID address) {
    for (size_t i = 0; i <= buffer.size() - _value.size(); ++i) {
        if (std::memcmp(&buffer[i], _value.data(), _value.size()) == 0) {
            // found
            LPCVOID foundAddr = (LPCVOID)((uintptr_t)address + i);
            _foundAddresses.push_back(foundAddr);

        }
    }
}


void MemoryScanner::findValueFinal() {
    std::vector<unsigned char> buffer(_chosenType.second);
    if (_foundAddresses.size() == 1) {
        printf("0x%p\n", _foundAddresses[0]);
    }
    while (_foundAddresses.size() > 1) {

        for (int i = 0; i < _foundAddresses.size(); ) {

            BOOL read = ReadProcessMemory(
                _procHandle,
                _foundAddresses[i],
                buffer.data(),
                buffer.size(),
                NULL
            );

            if (!read || std::memcmp(buffer.data(), _value.data(), _value.size()) != 0) {
                _foundAddresses.erase(_foundAddresses.begin() + i);
            }
            else {
                i++;
            }
        }
        printf("Found the value at %d addresses\n", _foundAddresses.size());
        for (const auto& address : _foundAddresses) {
            printf("0x%p\n", address);
        }
        printf("Do you want to overwrite now? y/n: ");
        std::string overwrite;
        std::cin >> overwrite;
        if (overwrite == "y") {
            break;
        }
        else {
            getUserInput();
        }
    }
}


void MemoryScanner::overwriteValue() {
    for (int i = 0; i < _foundAddresses.size(); i++)
    {
        BOOL write = WriteProcessMemory(
            _procHandle, // hProcess
            (LPVOID)_foundAddresses[i], // lpBaseAddress
            _newValue.data(), // lpBuffer
            _chosenType.second, // nSize
            NULL // lpNumberOfBytesWritten
        );
    }


}



