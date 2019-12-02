//
// Created by Michał Najborowski on 09/11/2019.
//

#include <iostream>
#include <vector>
#include "VirtualMemory.h"

VirtualMemory::Page::Page() = default;

VirtualMemory::Page::Page(uint8_t data[]) {
    for (int i = 0; i < 16; i++)
        this->data[i] = data[i];
}

void VirtualMemory::Page::print() const {
    for (auto &x : data) {
        if (x == 0) std::cout << "_";
        else std::cout << std::to_string(x);
    }
    std::cout << std::endl;
}

void VirtualMemory::insertProgram(std::pair<int, std::vector<Page>> program) {
    swapFile.insert(program);
}

void VirtualMemory::updateQueue(int frameNumber) {
    for (auto &frame : queue)
        if (frame.first == frameNumber) frame.second = true;
}

void VirtualMemory::updateSwapFilePage(int pid, int pageNumber, VirtualMemory::Page page) {
    for (auto &program : swapFile) {
        if (program.first == pid)
            for (int i = 0; i < sizeof(page.data); i++)
                program.second[pageNumber].data[i] = page.data[i];
    }
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

VirtualMemory::Page VirtualMemory::getPage(int pid, int pageNumber) {
    return swapFile.find(pid)->second[pageNumber];
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
