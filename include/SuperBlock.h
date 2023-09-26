#ifndef SUPER_BLOCK_H
#define SUPER_BLOCK_H


#include <iostream>
#include <vector>
#include "FileAllocationTableEntry.h"
using namespace std;





class SuperBlock{


public:
    SuperBlock(int numberOfBlocks, int blockSize);
    SuperBlock();
    int getNumberOfBlocks();
    int getNumberOfFreeBlocks();
    int getBlockSize();
    void setNumberOfBlocks(int numberOfBlocks);
    void setNumberOfFreeBlocks(int numberOfFreeBlocks);
    void setBlockSize(int blockSize);
    void printSuperBlock();
    static SuperBlock deserializeSuperBlock(const vector<uint8_t>& byteVector);
    vector<uint8_t> serializeSuperBlock();
    int getRootDirectoryBlockIndex();
    void setRootDirectoryBlockIndex(int rootDirectoryBlockIndex);
    void incrementNumberOfFreeBlocks();
    void decrementNumberOfFreeBlocks();

private:
    int numberOfBlocks;
    int numberOfFreeBlocks;
    int blockSize;
    int rootDirectoryBlockIndex;
};



#endif