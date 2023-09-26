#ifndef DIRECTORY_ENTRY_H
#define DIRECTORY_ENTRY_H

#include <iostream>
#include <ctime>
#include <cstring>
#include "Constants.h"
#include "DateTime.h"
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

class DirectoryEntry{


public:
    DirectoryEntry(char fileName[8], char fileExtension[3], unsigned short int firstBlockNumber, bool isDirectory);
    DirectoryEntry();
    char* getFileName();
    char* getFileExtension();
    unsigned short int getFirstBlockNumber();
    unsigned int getFileSize();
    DateTime getLastModificationTime();
    bool getIsDirectory();
    void setFileName(char fileName[8]);
    void setFileExtension(char fileExtension[3]);
    void setFirstBlockNumber(unsigned short int firstBlockNumber);
    void setFileSize(unsigned int fileSize);
    void updateLastModificationTime();
    void setIsDirectory(bool isDirectory);
    vector<uint8_t> serialize() const;
    bool isDirectoryEntryUsed();
    void deserialize(const uint8_t* bytePtr);
    void setLastModificationTime(DateTime lastModificationTime);
    string getFileNameWithExtension();


private:
    char fileName[8];
    char fileExtension[3];
    unsigned short int firstBlockNumber;
    unsigned int fileSize;
    DateTime lastModificationTime;
    bool isDirectory;
};















#endif