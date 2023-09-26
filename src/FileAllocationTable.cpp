#include "../include/FileAllocationTable.h"

FileAllocationTable::FileAllocationTable(int numberOfBlocks){
    this->numberOfBlocks = numberOfBlocks;
    this->entries = new FileAllocationTableEntry[numberOfBlocks];
}

FileAllocationTableEntry* FileAllocationTable::entryAt(int index){
    return &this->entries[index];
}

bool FileAllocationTable::isBlockUsed(int index){
    return this->entries[index].getIsUsed();
}

void FileAllocationTable::setBlockUsed(int index, bool isUsed){
    this->entries[index].setIsUsed(isUsed);
}

int FileAllocationTable::getNextBlock(int index){
    return this->entries[index].getNextBlock();
}

void FileAllocationTable::setNextBlock(int index, int nextBlock){
    this->entries[index].setNextBlock(nextBlock);
}

int FileAllocationTable::getNumberOfBlocks(){
    return this->numberOfBlocks;
}

void FileAllocationTable::setNumberOfBlocks(int numberOfBlocks){
    this->numberOfBlocks = numberOfBlocks;
}

void FileAllocationTable::printTable(){
    cout << "File Allocation Table" << endl;
    cout << "---------------------" << endl;
    cout << "Block\tNext Block\tUsed" << endl;
    for(int i = 0; i < this->numberOfBlocks; i++){
        cout << i << "\t" << ((this->entries[i].getNextBlock() == SYSTEM_BLOCK) ? "SYSTEM" : to_string(this->entries[i].getNextBlock())) << "\t\t" << this->entries[i].getIsUsed() << endl;
        if(i == 50){
            break;
        }
    }
}

FileAllocationTable::~FileAllocationTable(){
    delete[] this->entries;
}

vector<int> FileAllocationTable::getBlockNumbersOfAFile(int startingBlock){
    vector<int> blockNumbers;
    int currentBlock = startingBlock;
    while(currentBlock != SYSTEM_BLOCK && currentBlock != END_OF_CHAIN){
        blockNumbers.push_back(currentBlock);
        currentBlock = this->getNextBlock(currentBlock);
    }
    return blockNumbers;
}