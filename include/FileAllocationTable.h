#ifndef FILE_ALLOCATION_TABLE_H
#define FILE_ALLOCATION_TABLE_H

#include <iostream>
#include <string>

#include "FileAllocationTableEntry.h"

using namespace std;


class FileAllocationTable{


public:
    FileAllocationTable(int numberOfBlocks);
    FileAllocationTableEntry *entryAt(int index);
    bool isBlockUsed(int index);
    void setBlockUsed(int index, bool isUsed);
    int getNextBlock(int index);
    void setNextBlock(int index, int nextBlock);
    int getNumberOfBlocks();
    void setNumberOfBlocks(int numberOfBlocks);
    void printTable(); 
    ~FileAllocationTable();
    vector<int> getBlockNumbersOfAFile(int startingBlock);
private:
    int numberOfBlocks;
    FileAllocationTableEntry *entries;
};
















#endif