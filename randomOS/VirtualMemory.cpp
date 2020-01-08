#include "VirtualMemory.h"


uint8_t VirtualMemory::insertProgram(std::pair<int, std::vector<Page>> program) {
	for (auto &p : swapFile)
		if (p.first == program.first) return 96;
	if (program.second.empty()) return 97;
	swapFile.insert(program);
	return 0;
}

uint8_t VirtualMemory::updateQueue(int frameNumber) {
	if (frameNumber < 0 || frameNumber > queue.size() - 1) return 98;
	for (auto &frame : queue)
		if (frame.first == frameNumber) frame.second = true;
	return 0;
}

uint8_t VirtualMemory::updateSwapFilePage(int pid, int pageNumber, Page page) {
	if (swapFile.count(pid) == 0) return 100;
	if (pageNumber < 0 || pageNumber > swapFile.find(pid)->second.size() - 1) return 99;
	for (auto &program : swapFile) {
		if (program.first == pid)
			for (int i = 0; i < sizeof(page.data); i++)
				program.second[pageNumber].data[i] = page.data[i];
	}
	return 0;
}

int VirtualMemory::getVictimFrameNumber() {
	std::pair<int, bool> temp;
	while (queue.front().second) {
		temp = queue.front();
		queue.erase(queue.begin());
		temp.second = false;
		queue.push_back(temp);
	}
	temp = queue.front();
	queue.erase(queue.begin());
	temp.second = true;
	queue.push_back(temp);
	return temp.first;
}

std::pair<uint8_t, Page> VirtualMemory::getPage(int pid, int pageNumber) {
	if (swapFile.count(pid) == 0)
		return std::make_pair(100, Page());
	if (pageNumber < 0 || pageNumber > swapFile.find(pid)->second.size() - 1)
		return std::make_pair(99, Page());
	return std::make_pair(0, swapFile.find(pid)->second[pageNumber]);
}

void VirtualMemory::printSwapFile() {
	std::cout << "---------- SWAP FILE ----------" << std::endl;
	for (auto &program : swapFile) {
		std::cout << "PID: " << program.first << std::endl;
		for (auto &page : program.second)
			page.print();
		//std::cout << std::endl;
	}
}

void VirtualMemory::printQueue() {
	std::cout << "---------- QUEUE ----------" << std::endl;
	for (auto &frame : queue)
		std::cout << "{" << frame.first << ", " << frame.second << "} ";
	std::cout << std::endl;
}

void VirtualMemory::test() {
	std::cout
		<< "1. [INSERTING PROGRAMS]\nCreating 2 programs of 3 pages and inserting them into swapFile with pid = 1 and pid = 2."
		<< std::endl;
	std::cout << std::endl;

	int8_t data[16] = { 0 };
	std::vector<Page> pages;
	for (int8_t i = 0; i < 3; i++) {
		for (int8_t &j : data) {
			j = i;
		}
		pages.emplace_back(Page(data));
	}
	insertProgram(std::make_pair(1, pages));
	insertProgram(std::make_pair(2, pages));
	printSwapFile();
	std::cout << std::endl << std::endl;


	std::cout << "2. [GETTING VICTIM FRAMES, SWAPPING PAGES & UPDATING QUEUE]\nGetting victim frames." << std::endl;
	std::cout << "RAM is full, have to swap page in 1st frame" << std::endl;
	std::cout << std::endl;

	printQueue();
	std::cout << std::endl;
	std::cout << "Victim frame (got using getVictimFrameNumber()): " << getVictimFrameNumber() << std::endl;
	std::cout << std::endl;
	printQueue();
	std::cout << std::endl;

	std::cout
		<< "Imaging that during processes execution a page in 1st frame has changed to all 5 and it's the 2nd page of process with pid = 2"
		<< std::endl;
	Page *changedPage = new Page();
	for (int8_t &ch : changedPage->data) {
		ch = 5;
	}
	std::cout << "That page: ";
	changedPage->print();
	std::cout << std::endl;

	std::cout << "So we first get the desired page, e.g. 3rd page of process with pid = 1, and put it into RAM"
		<< std::endl;
	std::cout << "That page (got using getPage()): ";
	getPage(1, 2).second.print();
	std::cout << std::endl;

	std::cout << "Now we have to update the page that was taken down in SwapFile" << std::endl;
	updateSwapFilePage(2, 1, *changedPage);
	std::cout << "After update" << std::endl;
	printSwapFile();
	std::cout << std::endl;

	std::cout << "Referencing page in frame number 3" << std::endl;
	std::cout << "Queue after reference" << std::endl;
	updateQueue(3);
	printQueue();
}