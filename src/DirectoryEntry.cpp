#include "../include/DirectoryEntry.h"

DirectoryEntry::DirectoryEntry(char fileName[8], char fileExtension[3], unsigned short int firstBlockNumber, bool isDirectory) {
    setFileName(fileName);
    setFileExtension(fileExtension);
    setFirstBlockNumber(firstBlockNumber);
    setIsDirectory(isDirectory);
    setFileSize(0);
    lastModificationTime = getCurrentDateTime();
    
}

DirectoryEntry::DirectoryEntry() {
    memset(fileName, 0, sizeof(fileName));
    memset(fileExtension, 0, sizeof(fileExtension));
    firstBlockNumber = EMPTY_BLOCK;
    fileSize = 0;
    lastModificationTime = getCurrentDateTime();
    isDirectory = false;
}

char* DirectoryEntry::getFileName() {
    return fileName;
}

char* DirectoryEntry::getFileExtension() {
    return fileExtension;
}

unsigned short int DirectoryEntry::getFirstBlockNumber() {
    return firstBlockNumber;
}

unsigned int DirectoryEntry::getFileSize() {
    return fileSize;
}

DateTime DirectoryEntry::getLastModificationTime() {
    return lastModificationTime;
}

bool DirectoryEntry::getIsDirectory() {
    return isDirectory;
}

void DirectoryEntry::setFileName(char fileName[8]) {
    strncpy(this->fileName, fileName, sizeof(this->fileName));
}

void DirectoryEntry::setFileExtension(char fileExtension[3]) {
    strncpy(this->fileExtension, fileExtension, sizeof(this->fileExtension));
}

void DirectoryEntry::setFirstBlockNumber(unsigned short int firstBlockNumber) {
    this->firstBlockNumber = firstBlockNumber;
}

void DirectoryEntry::setFileSize(unsigned int fileSize) {
    this->fileSize = fileSize;
}

void DirectoryEntry::updateLastModificationTime() {
    this->lastModificationTime = getCurrentDateTime();
}

void DirectoryEntry::setIsDirectory(bool isDirectory) {
    this->isDirectory = isDirectory;
}

bool DirectoryEntry::isDirectoryEntryUsed(){
    return firstBlockNumber == EMPTY_BLOCK ? false : true;
}

vector<uint8_t> DirectoryEntry::serialize() const {
    vector<uint8_t> byteVector;
    for (int i = 0; i < 8; ++i) {
        byteVector.push_back(static_cast<uint8_t>(fileName[i]));
    }
    for (int i = 0; i < 3; ++i) {
        byteVector.push_back(static_cast<uint8_t>(fileExtension[i]));
    }
    byteVector.push_back(static_cast<uint8_t>(firstBlockNumber >> 8));
    byteVector.push_back(static_cast<uint8_t>(firstBlockNumber & 0xFF));
    byteVector.push_back(static_cast<uint8_t>(fileSize >> 24));
    byteVector.push_back(static_cast<uint8_t>(fileSize >> 16));
    byteVector.push_back(static_cast<uint8_t>(fileSize >> 8));
    byteVector.push_back(static_cast<uint8_t>(fileSize & 0xFF));
    byteVector.push_back(static_cast<uint8_t>(lastModificationTime.year >> 8));
    byteVector.push_back(static_cast<uint8_t>(lastModificationTime.year & 0xFF));
    byteVector.push_back(static_cast<uint8_t>(lastModificationTime.month));
    byteVector.push_back(static_cast<uint8_t>(lastModificationTime.day));
    byteVector.push_back(static_cast<uint8_t>(lastModificationTime.hour));
    byteVector.push_back(static_cast<uint8_t>(lastModificationTime.minute));
    byteVector.push_back(static_cast<uint8_t>(lastModificationTime.second));
    byteVector.push_back(static_cast<uint8_t>(isDirectory));
    return byteVector;
}

void DirectoryEntry::deserialize(const uint8_t* bytePtr) {
    for (int i = 0; i < 8; ++i) {
        fileName[i] = static_cast<char>(bytePtr[i]);
    }
    for (int i = 0; i < 3; ++i) {
        fileExtension[i] = static_cast<char>(bytePtr[i + 8]);
    }
    firstBlockNumber = static_cast<unsigned short int>((bytePtr[11] << 8) | bytePtr[12]);
    fileSize = static_cast<unsigned int>((bytePtr[13] << 24) | (bytePtr[14] << 16) | (bytePtr[15] << 8) | bytePtr[16]);
    lastModificationTime.year = static_cast<unsigned short int>((bytePtr[17] << 8) | bytePtr[18]);
    lastModificationTime.month = static_cast<unsigned short int>(bytePtr[19]);
    lastModificationTime.day = static_cast<unsigned short int>(bytePtr[20]);
    lastModificationTime.hour = static_cast<unsigned short int>(bytePtr[21]);
    lastModificationTime.minute = static_cast<unsigned short int>(bytePtr[22]);
    lastModificationTime.second = static_cast<unsigned short int>(bytePtr[23]);
    isDirectory = static_cast<bool>(bytePtr[24]);
}

void DirectoryEntry::setLastModificationTime(DateTime lastModificationTime) {
    this->lastModificationTime = lastModificationTime;
}

string DirectoryEntry::getFileNameWithExtension(){
    if(fileExtension[0] == '\0'){
        return string(fileName);
    }
    return string(fileName) + "." + string(fileExtension);
}