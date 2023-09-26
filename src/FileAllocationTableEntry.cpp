#include "../include/FileAllocationTableEntry.h"

FileAllocationTableEntry::FileAllocationTableEntry(short int nextBlock)
{
    this->nextBlock = nextBlock;
}

FileAllocationTableEntry::FileAllocationTableEntry(){
    this->nextBlock = EMPTY_BLOCK;
}

short int FileAllocationTableEntry::getNextBlock(){
    return this->nextBlock;
}

bool FileAllocationTableEntry::getIsUsed(){
    return this->nextBlock == EMPTY_BLOCK ? false : true;
}

void FileAllocationTableEntry::setNextBlock(short int nextBlock){
    this->nextBlock = nextBlock;
}

void FileAllocationTableEntry::setIsUsed(bool isUsed){
    this->nextBlock = END_OF_CHAIN;
}

vector<uint8_t> FileAllocationTableEntry::serialize() const {
    vector<uint8_t> data;
    data.reserve(sizeof(nextBlock));
    const uint8_t* nextBlockBytes = reinterpret_cast<const uint8_t*>(&nextBlock);
    data.insert(data.end(), nextBlockBytes, nextBlockBytes + sizeof(nextBlock));
    return data;
}
