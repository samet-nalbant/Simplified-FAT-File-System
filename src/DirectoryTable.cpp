#include "../include/DirectoryTable.h"

DirectoryTable::DirectoryTable(int blockSize){
    this->numberOfEntries = blockSize / sizeof(DirectoryEntry);
    this->directoryEntries = new DirectoryEntry[this->numberOfEntries];
}

int DirectoryTable::getNumberOfEntries(){
    return this->numberOfEntries;
}

DirectoryEntry *DirectoryTable::getDirectoryEntry(int index){
    return &this->directoryEntries[index];
}

void DirectoryTable::setDirectoryAt(int index, DirectoryEntry *directoryEntry){
    this->directoryEntries[index] = *directoryEntry;
}

void DirectoryTable::printDirectoryTable() {
    cout << "Directory Table" << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << left << setw(18) << "Directory Index";
    cout << left << setw(20) << "Directory Name";
    cout << left << setw(18) << "File Extension";
    cout << left << setw(15) << "Next Block";
    cout << left << setw(8) << "Used";
    cout << "Last Modification Time" << endl;
    for (int i = 0; i < numberOfEntries; i++) {
        cout << left << setw(18) << i;
        cout << left << setw(20) << (directoryEntries[i].isDirectoryEntryUsed() ? directoryEntries[i].getFileName() : "-");
        cout << left << setw(18) << (directoryEntries[i].isDirectoryEntryUsed() ? directoryEntries[i].getFileExtension() : "-");
        cout << left << setw(15) << directoryEntries[i].getFirstBlockNumber();
        cout << left << setw(8) << (directoryEntries[i].isDirectoryEntryUsed() ? "Yes" : "No");
        cout << directoryEntries[i].getLastModificationTime().toString() << endl;
    }
}

void DirectoryTable::printDirectoryContent() {
    cout << left << setw(23) << "Name";
    cout << setw(9) << "Size";
    cout << setw(8) << "Type";
    cout << setw(30) << "Last Modification Time" << endl;
    cout << "------------------------------------------------------------------" << endl;
    for (int i = 0; i < this->numberOfEntries; i++) {
        if (this->directoryEntries[i].isDirectoryEntryUsed()) {
            if (directoryEntries[i].getIsDirectory()) {
                cout << left << setw(23) << directoryEntries[i].getFileName();
                cout << setw(9) << directoryEntries[i].getFileSize();
                cout << setw(8) << "DIR";
                cout << setw(30) << directoryEntries[i].getLastModificationTime().toString();
            }
            else {
                cout << left << setw(23) << directoryEntries[i].getFileNameWithExtension();
                cout << setw(9) << directoryEntries[i].getFileSize();
                cout << setw(8) << "FILE";
                cout << setw(30) << directoryEntries[i].getLastModificationTime().toString();
            }
            cout << endl;
        }
    }
}



DirectoryEntry *DirectoryTable::getDirectoryEntry(string fileName){
    for(int i = 0; i < this->numberOfEntries; i++){
        if(this->directoryEntries[i].getFileNameWithExtension() == fileName){
            return &this->directoryEntries[i];
        }
    }
    return NULL;
}

int DirectoryTable::getDirectoryEntryIndex(string fileName){
    for(int i = 0; i < this->numberOfEntries; i++){
        if(this->directoryEntries[i].getFileNameWithExtension() == fileName){
            return i;
        }
    }
    return -1;
}