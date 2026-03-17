#ifndef MEMORYSCANNER_HPP
#define MEMORYSCANNER_HPP

// Includes
#include "Process.hpp"
#include <map>
#include <string>


class MemoryScanner : public Process {
public:
    MemoryScanner(int pid, HANDLE handle); // Ctor
    
    // Methods
    void init();
    void calcPageSizeAndMaxRange();
    void queryVirtualMemoryRanges();
    void getUserInput(); // TODO: add user input
    void scan(); // TODO: Add scanning

private:
    DWORD _pageSize;
    LPCVOID _maxVirtualRange;
    std::map<LPCVOID, SIZE_T> _ranges; // <address, length>
    std::string _type;
    std::size_t _size
};

#endif // MEMORYSCANNER_HPP