#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <cstring>


#include "FileAllocationTable.h"
#include "SuperBlock.h"
#include "DirectoryTable.h"

class FileManager;

#define FAT12_BLOCK_NUMBER pow(2, 12)
#define KB 1024

using namespace std;


class FileSystem{

public:

    FileSystem(string volumeName);
    static void createFileSystem(string volumeName, int blockSize);
    friend class FileManager;

private:

    SuperBlock *superBlock;
    FileAllocationTable *fat;
    DirectoryTable *rootDirectory;
    uint8_t** blocks;
    int blockSize;
    int numberOfBlocks;
    string volumeName;
    vector<uint8_t> readFileSystem();
    SuperBlock *readSuperBlock(const vector<uint8_t>& byteVector);
    void fillBlocks(vector<uint8_t> &fileByteVector);
    void printBlock(int blockIndex);
    void loadFileAllocationTable();
    void loadRootDirectory();
    void writeFileSystem();
    DirectoryTable *readDirectoryTable(int blockIndex);
};

#endif