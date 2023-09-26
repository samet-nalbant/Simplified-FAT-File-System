#ifndef FILEMANAGER_H
#define FILEMANAGER_H


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include "FileSystem.h"
#include <sstream>

class FileManager {
public:
    FileManager(string volumeName);
    static void createFileSystem(string volumeName, int blockSize);
    void createDirectory(string directoryPath);
    void printDirectoryContent(string directoryPath);
    void saveAndExit();
    void removeDirectory(string directoryPath);
    void printSuperBlock();
    void printFileAllocationTable();
    void createFile(string filePath, string inputFileName);
    void readFile(string filePath, string outputFileName);
    void removeFile(string filePath);
    void dumpe2fs();
private:
    FileSystem* fileSystem;
    void assignNextBlock(int currentBlockIndex, int nextBlockIndex);
    void assignEndOfChain(int currentBlockIndex);
    void assignSystemBlock(int blockIndex);
    void assignDirectoryEntryForFolder(int blockIndex, int directoryIndex, int freeBlock, string directoryName);
    bool isDirectoryStringValid(string directoryPath);
    int getParentBlockNumber(string path);
    vector<string> getParentDirectories(string path);
    int findDirectoryInDirectoryTable(DirectoryTable* directoryTable, string directoryName);
    int findFileInDirectoryTable(DirectoryTable* directoryTable, string fileName);
    DirectoryTable *getDirectoryTable(int directoryBlockNumber);
    bool isPresentInRootDirectory(string path);
    int findFreeBlock();
    int findFreeDirectoryEntry(int blockNumber);
    void extendDirectory(int blockNumber); //Directory entry yok boşta yeni block initialize edilmesi lazım.
    void initializeBlockForFolder(int blockNumber);
    void serializeSuperBlock();
    void serializeSingleDirectory(DirectoryTable *directoryTable, int blockNumber);
    void serializeFileAllocationTable();
    int getNumberOfFatEntryPerBlock();
    void writeBlocksToVolume();
    bool isDirectoryPresentInDirectory(int parentDirectoryBlockNumber, string folderName);
    vector<string> splitString(const string& input, char delimiter);
    bool isFilePresentInDirectory(int parentDirectoryBlockNumber, string fileName);
    void removeDirectoryWithBlockNumber(int blockNumber);
    void removeDirectoryEntryFromParentDirectory(int parentDirectoryBlockNumber, string directoryName);
    uint8_t *readFileContent(string file, int *fileSize);
    int numberOfRequiredBlocks(int fileSize);
    vector<int> findFreeBlocks(int numberOfRequiredBlocks);
    void writeContentToBlocks(vector<int> freeBlocks, uint8_t *fileContent, int fileSize);
    void writeBlock(int blockNumber, uint8_t* content, int numberOfBytes);
    uint8_t* readFileContent(int firstBlockNumber, int fileSize);
    void removeFileWithBlockNumber(int blockNumber);
    void printDirectoryAndFilesInfo(int blockNumber);
    int getDirectorySize(int blockNumber);
};

#endif
