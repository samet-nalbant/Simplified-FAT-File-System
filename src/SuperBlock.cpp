#include "../include/SuperBlock.h"


SuperBlock::SuperBlock(){}

SuperBlock::SuperBlock(int numberOfBlocks, int blockSize){
    this->numberOfBlocks = numberOfBlocks;
    this->numberOfFreeBlocks = numberOfBlocks;
    this->blockSize = blockSize;
    this->rootDirectoryBlockIndex = sizeof(FileAllocationTableEntry) * getNumberOfBlocks() / getBlockSize() + 1;
}

int SuperBlock::getNumberOfBlocks(){
    return this->numberOfBlocks;
}

int SuperBlock::getNumberOfFreeBlocks(){
    return this->numberOfFreeBlocks;
}

int SuperBlock::getBlockSize(){
    return this->blockSize;
}

void SuperBlock::setNumberOfBlocks(int numberOfBlocks){
    this->numberOfBlocks = numberOfBlocks;
}

void SuperBlock::setNumberOfFreeBlocks(int numberOfFreeBlocks){
    this->numberOfFreeBlocks = numberOfFreeBlocks;
}

void SuperBlock::setBlockSize(int blockSize){
    this->blockSize = blockSize;
}

void SuperBlock::printSuperBlock(){
    cout << "Super Block" << endl;
    cout << "-----------" << endl;
    cout << "Number of Blocks: " << this->numberOfBlocks << endl;
    cout << "Number of Free Blocks: " << this->numberOfFreeBlocks << endl;
    cout << "Block Size: " << this->blockSize << " byte." << endl;
    cout << "Root Directory Block Index: " << this->rootDirectoryBlockIndex << endl;
    cout << "-----------" << endl;
}

vector<uint8_t> SuperBlock::serializeSuperBlock() {
    vector<uint8_t> byteVector;
    uint8_t* numberOfBlocksPtr = reinterpret_cast<uint8_t*>(&numberOfBlocks);
    byteVector.insert(byteVector.end(), numberOfBlocksPtr, numberOfBlocksPtr + sizeof(numberOfBlocks));
    uint8_t* numberOfFreeBlocksPtr = reinterpret_cast<uint8_t*>(&numberOfFreeBlocks);
    byteVector.insert(byteVector.end(), numberOfFreeBlocksPtr, numberOfFreeBlocksPtr + sizeof(numberOfFreeBlocks));
    uint8_t* blockSizePtr = reinterpret_cast<uint8_t*>(&blockSize);
    byteVector.insert(byteVector.end(), blockSizePtr, blockSizePtr + sizeof(blockSize));
    uint8_t* rootDirectoryBlockIndexPtr = reinterpret_cast<uint8_t*>(&rootDirectoryBlockIndex);
    byteVector.insert(byteVector.end(), rootDirectoryBlockIndexPtr, rootDirectoryBlockIndexPtr + sizeof(rootDirectoryBlockIndex));
    return byteVector;
}


SuperBlock SuperBlock::deserializeSuperBlock(const vector<uint8_t>& byteVector) {
    SuperBlock superBlock;
    if (byteVector.size() >= sizeof(int) * 3) {
        uint8_t* numberOfBlocksPtr = reinterpret_cast<uint8_t*>(&superBlock.numberOfBlocks);
        copy(byteVector.begin(), byteVector.begin() + sizeof(int), numberOfBlocksPtr);
        uint8_t* numberOfFreeBlocksPtr = reinterpret_cast<uint8_t*>(&superBlock.numberOfFreeBlocks);
        copy(byteVector.begin() + sizeof(int), byteVector.begin() + (sizeof(int) * 2), numberOfFreeBlocksPtr);
        uint8_t* blockSizePtr = reinterpret_cast<uint8_t*>(&superBlock.blockSize);
        copy(byteVector.begin() + (sizeof(int) * 2), byteVector.begin() + (sizeof(int) * 3), blockSizePtr);
    }
    return superBlock;
}


int SuperBlock::getRootDirectoryBlockIndex() {
    return rootDirectoryBlockIndex;
}

void SuperBlock::setRootDirectoryBlockIndex(int rootDirectoryBlockIndex){
    this->rootDirectoryBlockIndex = rootDirectoryBlockIndex;
}

void SuperBlock::incrementNumberOfFreeBlocks(){
    this->numberOfFreeBlocks++;
}

void SuperBlock::decrementNumberOfFreeBlocks(){
    this->numberOfFreeBlocks--;
}