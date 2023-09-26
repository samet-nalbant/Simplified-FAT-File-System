#ifndef DIRECTORY_TABLE_H
#define DIRECTORY_TABLE_H

#include "DirectoryEntry.h"


using namespace std;

class DirectoryTable{


public:
    DirectoryTable(int blockSize);
    int getNumberOfEntries();
    DirectoryEntry *getDirectoryEntry(int index);
    void setDirectoryAt(int index, DirectoryEntry *directoryEntry);
    void printDirectoryTable();
    void printDirectoryContent();
    DirectoryEntry *getDirectoryEntry(string fileName);
    int getDirectoryEntryIndex(string fileName);
private:
    DirectoryEntry *directoryEntries;
    int numberOfEntries;

};









#endif