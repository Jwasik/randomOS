#ifndef RANDOMOS_VIRTUALMEMORY_H
#define RANDOMOS_VIRTUALMEMORY_H

#include "Includes.h"

class VirtualMemory {

private:

	// structure of a single process page
	struct Page {
		int8_t data[16]{ 0 }; // page data, initialized with zeroes
		Page();
		explicit Page(const int8_t data[]);
		void print() const;
	};

	// map of pairs (pid, pageVector) to store programs
	std::map<int, std::vector<Page>> swapFile;

	// queue of pairs (frameIndex, referenceBit) of last used frames
	std::vector<std::pair<int, bool>> queue
	{ {0, true},{1, true},{2, true},{3, true},{4, true},{5, true},{6, true},{7, true} };

public:

	// inserts program - pair of (pid, pageVector) - into swapFile
	// called every time a program is read from a file
	void insertProgram(std::pair<int, std::vector<Page>> program);

	// updates queue; the argument is index of a frame, that holds referenced page
	// called every time a page is referenced
	void updateQueue(int frameNumber);

	// updates data of a page, that is taken down from memory
	// called every time pages are being swapped
	void updateSwapFilePage(int pid, int pageNumber, Page page);

	// returns number of a victim frame
	// called every time pages are being swapped
	int getVictimFrameNumber();

	// returns referenced Page
	// called every time a page fault happens
	Page getPage(int pid, int PageNumber);

	void printSwapFile();

	void printQueue();

	void test();

	friend class Memory;
	friend class Shell;
};

#endif //RANDOMOS_VIRTUALMEMORY_H