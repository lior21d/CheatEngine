#ifndef MEMORYSCANNER_HPP
#define MEMORYSCANNER_HPP

// Includes
#include "Process.hpp"
#include <map>
#include <string>
#include <utility>
#include <vector>


class MemoryScanner : public Process {
public:
    MemoryScanner(int pid, HANDLE handle); // Ctor

    // Methods
    void init();
    void calcPageSizeAndMaxRange();
    void queryVirtualMemoryRanges();
    void getUserInput(); // TODO: support for string
    void typesCreate();
    void scanProcess(); // TODO: Add scanning
    void scanPage(LPCVOID baseAddress, uintptr_t offset, std::vector<unsigned char>& buffer);
    void findValueFirstScan(std::vector<unsigned char>& buffer, LPCVOID address);

private:
    DWORD _pageSize;
    LPCVOID _maxVirtualRange;
    std::map<LPCVOID, SIZE_T> _ranges; // <address, length>
    std::map<std::string, int> _types; // <type name, size>
    std::pair<std::string, int> _chosenType;
    std::vector<LPCVOID> _foundAddresses;
    std::vector<unsigned char> _value;
};

#endif // MEMORYSCANNER_HPP