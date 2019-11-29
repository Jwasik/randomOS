//
// Created by Michał Najborowski on 09/11/2019.
//

#ifndef RANDOMOS_VIRTUALMEMORY_H
#define RANDOMOS_VIRTUALMEMORY_H


#include <string>
#include <map>
#include <list>

class VirtualMemory {

private:

    // structure of a single process page
    struct Page {
        uint8_t data[16]{0}; // page data, initialized with zeroes

        Page();

        explicit Page(uint8_t data[]);

        void print() const;
    };

    // map of pairs (pid, pageVector) to store programs
    std::map<int, std::vector<Page>> swapFile;

    // queue of pairs (frameIndex, referenceBit) of last used frames
    std::vector<std::pair<int, bool>> queue{
            {0, true},
            {1, true},
            {2, true},
            {3, true},
            {4, true},
            {5, true},
            {6, true},
            {7, true}
    };

public:

    void insertProgram(std::pair<int, std::vector<Page>> process);

    void updateQueue(int frameNumber);

    void updateSwapFilePage(int pid, int pageNumber, Page page);

    int getVictimFrameNumber();

    Page getPage(int pid, int PageNumber);

    void printSwapFile();

    void printQueue();
};

#endif //RANDOMOS_VIRTUALMEMORY_H
