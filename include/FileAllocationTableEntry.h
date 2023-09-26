#ifndef FILE_ALLOCATION_TABLE_ENTRY_H
#define FILE_ALLOCATION_TABLE_ENTRY_H


#include <iostream>
#include <vector>

#include "Constants.h"

using namespace std;

class FileAllocationTableEntry{


public:
    FileAllocationTableEntry(short int nextBlock);
    FileAllocationTableEntry();
    short int getNextBlock();
    bool getIsUsed();
    void setNextBlock(short int nextBlock);
    void setIsUsed(bool isUsed);
    vector<uint8_t> serialize() const;
private:
    short int nextBlock;
};





#endif