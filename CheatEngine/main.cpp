#include <iostream>
#include "Process.hpp"

int main()
{

	std::cout << "Enter pid\n";
	int pid;
	std::cin >> pid;
	Process* proc = new Process(pid);
	proc->init();
	delete proc;

	return 0;
}