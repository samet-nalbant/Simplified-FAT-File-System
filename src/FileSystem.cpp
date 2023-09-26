#include "../include/FileSystem.h"
#include "../include/FileManager.h"

FileSystem::FileSystem(string volumeName){
    this->volumeName = volumeName;
    vector<uint8_t> byteVector = readFileSystem();
    this->superBlock = readSuperBlock(byteVector);
    this->fat = new FileAllocationTable(superBlock->getNumberOfBlocks());
    this->rootDirectory = new DirectoryTable(superBlock->getBlockSize());
    this->numberOfBlocks = superBlock->getNumberOfBlocks();
    blocks = new uint8_t*[superBlock->getNumberOfBlocks()]; 
    for(int i=0; i<FAT12_BLOCK_NUMBER; i++){
        blocks[i] = new uint8_t[superBlock->getBlockSize()];
    }
    fillBlocks(byteVector);
    loadFileAllocationTable();
    loadRootDirectory();
}
// According readed file, It fills the blocks.
void FileSystem::fillBlocks(vector<uint8_t> &fileByteVector) {
    int blockIndex = 0;
    for (int i = 0; i < fileByteVector.size(); i++) {
        if (i % superBlock->getBlockSize() == 0 && i != 0) {
            blockIndex++;
        }
        memcpy(blocks[blockIndex] + (i % superBlock->getBlockSize()) * sizeof(uint8_t), &fileByteVector.at(i), sizeof(uint8_t));
    }
}

void FileSystem::loadFileAllocationTable(){
    int numberOfBlocksRequiredForFat = sizeof(FileAllocationTableEntry) * superBlock->getNumberOfBlocks() / superBlock->getBlockSize();
    int numberOfFatEntryPerBlock = superBlock->getBlockSize() / sizeof(FileAllocationTableEntry);
    FileAllocationTableEntry *tempEntry = new FileAllocationTableEntry();
    int count = 0;
    for(int i=1; i<numberOfBlocksRequiredForFat; i++){
        for(int j=0; j<numberOfFatEntryPerBlock; j++){
            memcpy(tempEntry, blocks[i] + j * sizeof(FileAllocationTableEntry), sizeof(FileAllocationTableEntry));
            fat->setBlockUsed(count, tempEntry->getIsUsed());
            fat->setNextBlock(count, tempEntry->getNextBlock());
            count++;
        }
    }
    delete tempEntry;
}

void FileSystem::loadRootDirectory(){
    rootDirectory = readDirectoryTable(superBlock->getRootDirectoryBlockIndex());
}

DirectoryTable * FileSystem::readDirectoryTable(int blockIndex){
    DirectoryTable *directoryTable = new DirectoryTable(superBlock->getBlockSize());
    int numberOfDirectoryEntryPerBlock = superBlock->getBlockSize() / sizeof(DirectoryEntry);
    for(int i=0; i<numberOfDirectoryEntryPerBlock; i++){
        directoryTable->getDirectoryEntry(i)->deserialize(blocks[blockIndex] + i * sizeof(DirectoryEntry));
    }
    return directoryTable;
}

// Filesystem will be casted to byte vector and written to disk.
void FileSystem::writeFileSystem(){
    ofstream outputFile(volumeName, ios::binary);
    if(outputFile){
        for(int i=0; i<numberOfBlocks; i++){
            for(int j=0; j<superBlock->getBlockSize(); j++){
                outputFile << blocks[i][j];
            }
        }
        outputFile.close();
    }
}


vector<uint8_t> FileSystem::readFileSystem() {
    ifstream inputFile(volumeName, ios::binary);
    if (!inputFile) {
        return {};
    }

    inputFile.seekg(0, ios::end);
    streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    vector<uint8_t> byteVector(fileSize);

    inputFile.read(reinterpret_cast<char*>(byteVector.data()), fileSize);
    inputFile.close();
    return byteVector;
}

SuperBlock* FileSystem::readSuperBlock(const vector<uint8_t>& byteVector) {
    if (byteVector.size() >= sizeof(SuperBlock)) {
        SuperBlock* superBlock = new SuperBlock();
        memcpy(superBlock, byteVector.data(), sizeof(SuperBlock));
        return superBlock;
    }
    return nullptr;
}

void FileSystem::createFileSystem(string volumeName, int blockSize) {
    cout << "Creating file system..." << endl;
    SuperBlock superBlock(FAT12_BLOCK_NUMBER, blockSize * KB);
    FileAllocationTable fat(FAT12_BLOCK_NUMBER);
    uint8_t **initialBlocks;
    int numberOfBlocks = FAT12_BLOCK_NUMBER;
    initialBlocks = new uint8_t*[numberOfBlocks]; 
    for(int i=0; i<FAT12_BLOCK_NUMBER; i++){
        initialBlocks[i] = new uint8_t[superBlock.getBlockSize()];
    }
    for(int i=0; i<FAT12_BLOCK_NUMBER; i++){
        for(int j=0; j<blockSize * KB; j++){
            initialBlocks[i][j] = '\0';
        }
    }
    int numberOfBlocksRequiredForFat = sizeof(FileAllocationTableEntry) * superBlock.getNumberOfBlocks() / superBlock.getBlockSize();
    int numberOfFatEntryPerBlock = superBlock.getBlockSize() / sizeof(FileAllocationTableEntry);
    for(int i = 0; i <numberOfBlocksRequiredForFat+1; i++){
        fat.setNextBlock(i, SYSTEM_BLOCK);
        superBlock.decrementNumberOfFreeBlocks();
    }
    fat.setNextBlock(numberOfBlocksRequiredForFat+1, SYSTEM_BLOCK);
    superBlock.decrementNumberOfFreeBlocks();
    int blockIndex = 1;
    for(int i=0; i< fat.getNumberOfBlocks(); i++){
        if(i % numberOfFatEntryPerBlock == 0 && i != 0){
            blockIndex++;
        }
        memcpy(initialBlocks[blockIndex] + (i % numberOfFatEntryPerBlock) * sizeof(FileAllocationTableEntry), fat.entryAt(i)->serialize().data(), sizeof(FileAllocationTableEntry));
    }
    int numberOfDirectoryEntriesPerBlock = superBlock.getBlockSize() / sizeof(DirectoryEntry);
    DirectoryTable *directoryTable = new DirectoryTable(superBlock.getBlockSize());
    for(int i=0;i<numberOfDirectoryEntriesPerBlock;i++){
        memcpy(initialBlocks[superBlock.getRootDirectoryBlockIndex()] + i * sizeof(DirectoryEntry), directoryTable->getDirectoryEntry(i)->serialize().data(), sizeof(DirectoryEntry));
    }
    vector<uint8_t> byteVector = superBlock.serializeSuperBlock();
    initialBlocks[0] = byteVector.data();
    ofstream outputFile(volumeName, ios::binary | ios::trunc);
    if(outputFile){
        for(int i=0; i<numberOfBlocks; i++){
            for(int j=0; j<superBlock.getBlockSize(); j++){
                outputFile << initialBlocks[i][j];
            }
        }
        outputFile.close();
    }
    return;
}

void FileSystem::printBlock(int index){
    for(int i=0; i < superBlock->getBlockSize();i++){
        char ch = static_cast<char>(blocks[index][i]);
        cout << ch;
    }
}
