#include "../include/FileManager.h"
FileManager::FileManager(string volumeName){
    FileSystem *fs = new FileSystem(volumeName);
    this->fileSystem = fs;
}
void FileManager::printSuperBlock(){
    fileSystem->superBlock->printSuperBlock();
}
void FileManager::createFileSystem(string volumeName, int blockSize){
    FileSystem::createFileSystem(volumeName, blockSize);
}
void FileManager::assignNextBlock(int currentBlockIndex, int nextBlockIndex){
    fileSystem->fat->setNextBlock(currentBlockIndex, nextBlockIndex);
    fileSystem->superBlock->decrementNumberOfFreeBlocks();
}
void FileManager::assignEndOfChain(int currentBlockIndex){
    fileSystem->fat->setNextBlock(currentBlockIndex, END_OF_CHAIN);
    fileSystem->superBlock->decrementNumberOfFreeBlocks();
}
void FileManager::assignSystemBlock(int blockIndex){
    fileSystem->fat->setNextBlock(blockIndex, SYSTEM_BLOCK);
    fileSystem->superBlock->decrementNumberOfFreeBlocks();
}

bool FileManager::isDirectoryPresentInDirectory(int parentDirectoryBlockNumber, string folderName){
    DirectoryTable *directoryTable = getDirectoryTable(parentDirectoryBlockNumber);
    int directoryIndex = findDirectoryInDirectoryTable(directoryTable, folderName);
    if(directoryIndex == NOT_FOUND){
        return false;
    }
    return true;
}

bool FileManager::isFilePresentInDirectory(int parentDirectoryBlockNumber, string fileName){
    DirectoryTable *directoryTable = getDirectoryTable(parentDirectoryBlockNumber);
    int fileIndex = findFileInDirectoryTable(directoryTable, fileName);
    if(fileIndex == NOT_FOUND){
        return false;
    }
    return true;
}

void FileManager::printDirectoryContent(string directoryPath){
    if(directoryPath=="\\"){
        int directoryBlockNumber = fileSystem->superBlock->getRootDirectoryBlockIndex();
        DirectoryTable *directoryTable = getDirectoryTable(directoryBlockNumber);
        directoryTable->printDirectoryContent();
        return;
    }

    if(!isDirectoryStringValid(directoryPath)){
        exit(0);
    }
    int directoryBlockNumber = getParentBlockNumber(directoryPath);
    if(directoryBlockNumber == NOT_FOUND){
        cout << "Directory not found" << endl;
        exit(0);
    }
    DirectoryTable *directoryTable = getDirectoryTable(directoryBlockNumber);
    directoryTable->printDirectoryContent();
    return;
}
void FileManager::removeDirectoryWithBlockNumber(int blockNumber){
    DirectoryTable* directoryTable = getDirectoryTable(blockNumber);
    for(int i = 0; i < directoryTable->getNumberOfEntries(); i++){
        if(directoryTable->getDirectoryEntry(i)->isDirectoryEntryUsed()){
            int directoryEntryBlockNumberWillBeDeleted = directoryTable->getDirectoryEntry(i)->getFirstBlockNumber();
            if(directoryTable->getDirectoryEntry(i)->getIsDirectory()){
                removeDirectoryWithBlockNumber(directoryEntryBlockNumberWillBeDeleted);
            }
            else{
                removeFileWithBlockNumber(directoryEntryBlockNumberWillBeDeleted);
            }
            cout << "Directory entry will be deleted: " << directoryTable->getDirectoryEntry(i)->getFileNameWithExtension() << endl;
            directoryTable->setDirectoryAt(i, new DirectoryEntry());
        }
    }
    fileSystem->fat->setNextBlock(blockNumber, EMPTY_BLOCK);
    fileSystem->superBlock->incrementNumberOfFreeBlocks();
    serializeSingleDirectory(directoryTable, blockNumber);
}

void FileManager::removeFileWithBlockNumber(int blockNumber){
    int nextBlockNumber = fileSystem->fat->getNextBlock(blockNumber);
    if(nextBlockNumber != END_OF_CHAIN){
        removeFileWithBlockNumber(nextBlockNumber);
    }
    fileSystem->fat->setNextBlock(blockNumber, EMPTY_BLOCK);
    fileSystem->superBlock->incrementNumberOfFreeBlocks();
}



void FileManager::removeDirectoryEntryFromParentDirectory(int parentDirectoryBlockNumber, string directoryName){
    DirectoryTable *parentDirectoryTable = getDirectoryTable(parentDirectoryBlockNumber);
    for(int i = 0; i < parentDirectoryTable->getNumberOfEntries(); i++){
        if(parentDirectoryTable->getDirectoryEntry(i)->getFileName() == directoryName){
            parentDirectoryTable->setDirectoryAt(i, new DirectoryEntry());
            serializeSingleDirectory(parentDirectoryTable, parentDirectoryBlockNumber);
            return;
        }
    }
}


void FileManager::removeDirectory(string directoryPath){
    if(directoryPath == "\\"){
        cout << "Root directory cannot be removed" << endl;
        exit(0);
    }
    if(!isDirectoryStringValid(directoryPath)){
        exit(0);
    }
    size_t lastSeparatorIndex = directoryPath.find_last_of('\\');
    string parentPath = directoryPath.substr(0, lastSeparatorIndex);
    string folderName = directoryPath.substr(lastSeparatorIndex + 1);
    int directoryBlockNumber = getParentBlockNumber(directoryPath);
    int parentDirectoryBlockNumber = getParentBlockNumber(parentPath);
    if(directoryBlockNumber == NOT_FOUND){
        cout << "Directory not found" << endl;
        exit(0);
    }
    removeDirectoryWithBlockNumber(directoryBlockNumber); 
    removeDirectoryEntryFromParentDirectory(parentDirectoryBlockNumber, folderName);
}


void FileManager::createFile(string filePath, string inputFileName){
    if(!isDirectoryStringValid(filePath)){
        exit(0);
    }
    size_t lastSeparatorIndex = filePath.find_last_of('\\');
    string parentPath = filePath.substr(0, lastSeparatorIndex);
    string fileName = filePath.substr(lastSeparatorIndex + 1);
    int parentDirectoryBlockNumber = getParentBlockNumber(parentPath);
    if(parentDirectoryBlockNumber == NOT_FOUND){
        cout << "Parent directory not found" << endl;
        exit(0);
    }
    bool isFilePresent = isFilePresentInDirectory(parentDirectoryBlockNumber, fileName);
    if(isFilePresent){
        cout << "File: "<< fileName << " already present in the parent directory" << endl;
        exit(0);
    }
    int fileSize;
    uint8_t *fileContent = readFileContent(inputFileName, &fileSize);
    int numberOfBlocksNeeded = numberOfRequiredBlocks(fileSize);
    if(numberOfBlocksNeeded > fileSystem->superBlock->getNumberOfFreeBlocks()){
        cout << "Not enough space in the file system" << endl;
        exit(0);
    }
    vector<int> freeBlocks = findFreeBlocks(numberOfBlocksNeeded);

    int firstBlockNumber = freeBlocks[0];
    writeContentToBlocks(freeBlocks, fileContent, fileSize);
    DirectoryTable *parentDirectoryTable = getDirectoryTable(parentDirectoryBlockNumber);
    int freeDirectoryEntryIndex = findFreeDirectoryEntry(parentDirectoryBlockNumber);
    char fileNameWithoutExtension[8];
    char fileExtension[3];

    size_t dotPosition = fileName.find_last_of(".");
    if (dotPosition != string::npos) {
        strncpy(fileNameWithoutExtension, fileName.substr(0, dotPosition).c_str(), 8);
        fileNameWithoutExtension[7] = '\0';
        strncpy(fileExtension, fileName.substr(dotPosition + 1).c_str(), 3);
        fileExtension[2] = '\0';
    } else {
        strncpy(fileNameWithoutExtension, fileName.c_str(), 8);
        fileNameWithoutExtension[7] = '\0';
        fileExtension[0] = '\0';
    }
    DirectoryEntry *directoryEntry = new DirectoryEntry(fileNameWithoutExtension, fileExtension, firstBlockNumber, false);
    directoryEntry->setFileSize(fileSize);

    parentDirectoryTable->setDirectoryAt(freeDirectoryEntryIndex, directoryEntry);
    parentDirectoryTable->getDirectoryEntry(freeDirectoryEntryIndex)->setFileSize(fileSize);
    serializeSingleDirectory(parentDirectoryTable, parentDirectoryBlockNumber);
    for(int i = 0; i < freeBlocks.size(); i++){
        fileSystem->superBlock->decrementNumberOfFreeBlocks();
    }
}

uint8_t* FileManager::readFileContent(int firstBlockNumber, int fileSize){

    uint8_t *fileContent = new uint8_t[fileSize];
    int currentBlockNumber = firstBlockNumber;
    int currentByteIndex = 0;
    int remainingBytes = fileSize;
    while(currentBlockNumber != END_OF_CHAIN){
        if(remainingBytes < fileSystem->superBlock->getBlockSize()){
            memcpy(fileContent + currentByteIndex, fileSystem->blocks[currentBlockNumber], remainingBytes);
            break;
        }
        else{
            memcpy(fileContent + currentByteIndex, fileSystem->blocks[currentBlockNumber], fileSystem->superBlock->getBlockSize());
            currentByteIndex += fileSystem->superBlock->getBlockSize();
            remainingBytes -= fileSystem->superBlock->getBlockSize();
        }
        currentBlockNumber = fileSystem->fat->getNextBlock(currentBlockNumber);
    }
    return fileContent;
}

void FileManager::readFile(string filePath, string outputFileName){
    if(!isDirectoryStringValid(filePath)){
        exit(0);
    }
    size_t lastSeparatorIndex = filePath.find_last_of('\\');
    string parentPath = filePath.substr(0, lastSeparatorIndex);
    string fileName = filePath.substr(lastSeparatorIndex + 1);

    int parentDirectoryBlockNumber = getParentBlockNumber(parentPath);
    if(parentDirectoryBlockNumber == NOT_FOUND){
        cout << "Parent directory not found" << endl;
        exit(0);
    }
    bool isFilePresent = isFilePresentInDirectory(parentDirectoryBlockNumber, fileName);
    if(!isFilePresent){
        cout << "File: "<< fileName << " is couldn't found!" << endl;
        exit(0);
    }
    DirectoryTable *parentDirectoryTable = getDirectoryTable(parentDirectoryBlockNumber);
    DirectoryEntry *directoryEntry = parentDirectoryTable->getDirectoryEntry(fileName);
    int fileSize = directoryEntry->getFileSize();
    int firstBlockNumber = directoryEntry->getFirstBlockNumber();
    uint8_t *fileContent = readFileContent(firstBlockNumber, fileSize);

    ofstream outputFile;
    outputFile.open(outputFileName, ios::out | ios::binary);
    outputFile.write((char *)fileContent, fileSize);
    outputFile.close();
    delete[] fileContent;
}

void FileManager::removeFile(string filePath){
    if(!isDirectoryStringValid(filePath)){
        exit(0);
    }
    size_t lastSeparatorIndex = filePath.find_last_of('\\');
    string parentPath = filePath.substr(0, lastSeparatorIndex);
    string fileName = filePath.substr(lastSeparatorIndex + 1);

    int parentDirectoryBlockNumber = getParentBlockNumber(parentPath);
    if(parentDirectoryBlockNumber == NOT_FOUND){
        cout << "Parent directory not found" << endl;
        exit(0);
    }
    bool isFilePresent = isFilePresentInDirectory(parentDirectoryBlockNumber, fileName);
    if(!isFilePresent){
        cout << "File: "<< fileName << " is couldn't found!" << endl;
        exit(0);
    }

    DirectoryTable *parentDirectoryTable = getDirectoryTable(parentDirectoryBlockNumber);
    DirectoryEntry *directoryEntry = parentDirectoryTable->getDirectoryEntry(fileName);
    int firstBlockNumber = directoryEntry->getFirstBlockNumber();
    removeFileWithBlockNumber(firstBlockNumber);
    int directoryEntryIndex = parentDirectoryTable->getDirectoryEntryIndex(fileName);
    parentDirectoryTable->setDirectoryAt(directoryEntryIndex, new DirectoryEntry());
    serializeSingleDirectory(parentDirectoryTable, parentDirectoryBlockNumber);
}


void FileManager::writeContentToBlocks(vector<int> freeBlocks, uint8_t *fileContent, int fileSize){
    int currentBlockIndex = 0;
    int currentByteIndex = 0;
    int bytesLeft = fileSize;
    while(bytesLeft > 0){
        int bytesToWrite = min(fileSystem->superBlock->getBlockSize(), bytesLeft);
        writeBlock(freeBlocks[currentBlockIndex], fileContent + currentByteIndex, bytesToWrite);
        bytesLeft -= bytesToWrite;
        currentByteIndex += bytesToWrite;
        if(bytesLeft > 0){
            int nextBlock = freeBlocks[currentBlockIndex + 1];
            fileSystem->fat->setNextBlock(freeBlocks[currentBlockIndex], nextBlock);
            currentBlockIndex += 1;
        }
    }
    fileSystem->fat->setNextBlock(freeBlocks[currentBlockIndex], END_OF_CHAIN);
}


void FileManager::writeBlock(int blockNumber, uint8_t* content, int numberOfBytes){
    for(int i = 0; i < numberOfBytes; i++){
        fileSystem->blocks[blockNumber][i] = content[i];
    }
}

int FileManager::numberOfRequiredBlocks(int fileSize){
    int numberOfBlocks = fileSize / fileSystem->superBlock->getBlockSize();
    return numberOfBlocks + 1;
}

vector<int> FileManager::findFreeBlocks(int numberOfRequiredBlocks){
    vector<int> freeBlocks;
    for(int i = 0; i < fileSystem->superBlock->getNumberOfBlocks(); i++){
        if(fileSystem->fat->getNextBlock(i) == EMPTY_BLOCK){
            freeBlocks.push_back(i);
            if(freeBlocks.size() == numberOfRequiredBlocks){
                return freeBlocks;
            }
        }
    }
    return freeBlocks;
}
uint8_t *FileManager::readFileContent(string file, int *fileSize){
    ifstream inputFile(file, ios::binary);
    if(!inputFile){
        cout << "File couldn't be opened!" << endl;
        exit(0);
    }
    inputFile.seekg(0, ios::end);
    *fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);
    uint8_t *fileContent = new uint8_t[*fileSize];
    inputFile.read((char*)fileContent, *fileSize);
    inputFile.close();
    return fileContent;
}

void FileManager::createDirectory(string directoryPath){
    if(!isDirectoryStringValid(directoryPath)){
        exit(0);
    }
    size_t lastSeparatorIndex = directoryPath.find_last_of('\\');
    string parentPath = directoryPath.substr(0, lastSeparatorIndex);
    string folderName = directoryPath.substr(lastSeparatorIndex + 1);
    if(isPresentInRootDirectory(directoryPath)){
        parentPath = "";
    }
    int parentDirectoryBlockNumber = getParentBlockNumber(parentPath);
    if(parentDirectoryBlockNumber == NOT_FOUND){
        cout << "Parent directory not found" << endl;
        exit(0);
    }
    int freeBlockIndex = findFreeBlock();
    if(freeBlockIndex == NOT_FOUND){
        cout << "Free block couldn't be found!" << endl;
    }
    bool isDirectoryPresent = isFilePresentInDirectory(parentDirectoryBlockNumber, folderName);
    if(isDirectoryPresent){
        cout << "Directory: "<< folderName << " already present in the parent directory" << endl;
        exit(0);
    }
    int freeDirectoryEntryIndex = findFreeDirectoryEntry(parentDirectoryBlockNumber);
    if(freeDirectoryEntryIndex == NOT_FOUND){
        //extend block
    }
    else{
        assignEndOfChain(freeBlockIndex);
        assignDirectoryEntryForFolder(parentDirectoryBlockNumber, freeDirectoryEntryIndex, freeBlockIndex, folderName);
    }
}
void FileManager::assignDirectoryEntryForFolder(int blockIndex, int directoryIndex, int freeBlock, string directoryName){
    DirectoryTable *directoryTable = getDirectoryTable(blockIndex);
    directoryTable->setDirectoryAt(directoryIndex, new DirectoryEntry((char*)directoryName.c_str(), (char*)"", freeBlock, true));
    initializeBlockForFolder(freeBlock);
    serializeSingleDirectory(directoryTable, blockIndex);
}

bool FileManager::isDirectoryStringValid(string directoryPath){
    if(directoryPath[0] != '\\'){
        cout << "Directory name must start with \\" << endl;
        return false;
    }
    if(directoryPath[directoryPath.length() - 1] == '\\'){
        cout << "Directory name must not end with \\" << endl;
        return false;
    }
    for(int i=0; i<directoryPath.length(); i++){
        if(directoryPath[i] == '\\'){
            if(directoryPath[i+1] == '\\'){
                cout << "Directory name must not contain \\\\" << endl;
                return false;
            }
        }
    }

    return true;
}

int FileManager::findFreeBlock(){
    for(int i=0; i < fileSystem->fat->getNumberOfBlocks();i++){
        if(!fileSystem->fat->entryAt(i)->getIsUsed()){
            return i;
        }
    }
    return NOT_FOUND;
}

int FileManager::findFreeDirectoryEntry(int blockNumber){
    DirectoryTable *directoryTable = getDirectoryTable(blockNumber);
    for(int i=0; i < directoryTable->getNumberOfEntries();i++){
        if(!directoryTable->getDirectoryEntry(i)->isDirectoryEntryUsed()){
            return i;
        }
    }
    return NOT_FOUND;
}

int FileManager::findDirectoryInDirectoryTable(DirectoryTable* directoryTable, string directoryName){
    for(int i = 0; i< directoryTable->getNumberOfEntries(); i++){
        DirectoryEntry *directoryEntry = directoryTable->getDirectoryEntry(i);
        if(directoryEntry->getIsDirectory() == false || directoryEntry->isDirectoryEntryUsed() == false){
            continue;
        }
        else{
            if(directoryEntry->getFileName() == directoryName){
                return directoryEntry->getFirstBlockNumber();
            }
        }
    }
    return NOT_FOUND;
}

int FileManager::findFileInDirectoryTable(DirectoryTable* directoryTable, string fileName){
    for(int i = 0; i< directoryTable->getNumberOfEntries(); i++){
    DirectoryEntry *directoryEntry = directoryTable->getDirectoryEntry(i);
    if(directoryEntry->isDirectoryEntryUsed() == false){
        continue;
    }
    else{
        if(directoryEntry->getFileNameWithExtension() == fileName){
            return directoryEntry->getFirstBlockNumber();
        }
    }
}
    return NOT_FOUND;
}

int FileManager::getParentBlockNumber(string path){
    if(path.empty()){
        return fileSystem->superBlock->getRootDirectoryBlockIndex();
    }
    vector<string> parentDirectories = getParentDirectories(path);
    DirectoryTable *currentDirectory;
    int currentBlock = fileSystem->superBlock->getRootDirectoryBlockIndex();
    int directoryBlockNumber;
    for(int i=0; i<parentDirectories.size(); i++){
        string directoryName = parentDirectories[i];
        DirectoryTable *currentDirectory = getDirectoryTable(currentBlock);
        directoryBlockNumber = findDirectoryInDirectoryTable(currentDirectory, directoryName);
        if(directoryBlockNumber == NOT_FOUND){
            return NOT_FOUND;
        }
        else if(i == parentDirectories.size() - 1){
            return directoryBlockNumber;
        }
        else{
            currentBlock = directoryBlockNumber;
        }
    }
    return NOT_FOUND;
}

DirectoryTable * FileManager::getDirectoryTable(int directoryBlockNumber){
    int blockIndex = directoryBlockNumber;
    DirectoryTable *directoryTable = new DirectoryTable(fileSystem->superBlock->getBlockSize());
    directoryTable = fileSystem->readDirectoryTable(blockIndex);
    return directoryTable;
}

vector<string> FileManager::splitString(const string& input, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(input);
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

vector<string> FileManager::getParentDirectories(string path) {
    vector<string> directories = splitString(path, '\\');
    return directories;
}


bool FileManager::isPresentInRootDirectory(string path) {
    int totalCount = count(path.begin(), path.end(), '\\');
    return totalCount == 1;
}

void FileManager::extendDirectory(int blockNumber){

}

void FileManager::initializeBlockForFolder(int blockNumber){
    DirectoryTable *directoryTable = new DirectoryTable(fileSystem->superBlock->getBlockSize());
    serializeSingleDirectory(directoryTable, blockNumber);
}

void FileManager::saveAndExit(){
    serializeSuperBlock();
    serializeFileAllocationTable();
    writeBlocksToVolume();
}

void FileManager::serializeSuperBlock(){
    vector<uint8_t> byteVector = fileSystem->superBlock->serializeSuperBlock();
    memcpy(fileSystem->blocks[0], byteVector.data(), byteVector.size());
}
void FileManager::serializeFileAllocationTable(){
    int blockIndex = 1;
    for(int i=0; i< fileSystem->fat->getNumberOfBlocks(); i++){
        if(i % getNumberOfFatEntryPerBlock() == 0 && i != 0){
            blockIndex++;
        }
        memcpy(fileSystem->blocks[blockIndex] + (i % getNumberOfFatEntryPerBlock()) * sizeof(FileAllocationTableEntry), fileSystem->fat->entryAt(i)->serialize().data(), sizeof(FileAllocationTableEntry));
    }
}

int FileManager::getNumberOfFatEntryPerBlock(){
    return fileSystem->superBlock->getBlockSize() / sizeof(FileAllocationTableEntry);
}

void FileManager::writeBlocksToVolume(){
    ofstream outputFile(fileSystem->volumeName, ios::binary | ios::trunc);
    if(outputFile){
        for(int i=0; i< fileSystem->superBlock->getNumberOfBlocks(); i++){
            for(int j=0; j<fileSystem->superBlock->getBlockSize(); j++){
                outputFile << fileSystem->blocks[i][j];
            }
        }
        outputFile.close();
    }
    else{
        cout << "Blocks couldn't written to the disk!" << endl;
    }
}

void FileManager::serializeSingleDirectory(DirectoryTable *directoryTable, int blockNumber){
    for(int i=0;i<directoryTable->getNumberOfEntries();i++){
        memcpy(fileSystem->blocks[blockNumber] + i * sizeof(DirectoryEntry), directoryTable->getDirectoryEntry(i)->serialize().data(), sizeof(DirectoryEntry));
    }
}

void FileManager::printFileAllocationTable(){
    fileSystem->fat->printTable();
}

int FileManager::getDirectorySize(int blockNumber){
    DirectoryTable *directoryTable = getDirectoryTable(blockNumber);
    int size = 0;
    for(int i=0; i< directoryTable->getNumberOfEntries(); i++){
        DirectoryEntry *directoryEntry = directoryTable->getDirectoryEntry(i);
        if(directoryEntry->isDirectoryEntryUsed() == false){
            continue;
        }
        else{
            if(directoryEntry->getIsDirectory() == true){
                size += getDirectorySize(directoryEntry->getFirstBlockNumber());
            }
            else{
                size += directoryEntry->getFileSize();
            }
        }
    }
    return size;
}

void FileManager::printDirectoryAndFilesInfo(int blockNumber){
    DirectoryTable *directoryTable = getDirectoryTable(blockNumber);
    for(int i=0; i < directoryTable->getNumberOfEntries(); i++){
        DirectoryEntry *directoryEntry = directoryTable->getDirectoryEntry(i);
        if(directoryEntry->isDirectoryEntryUsed() == false){
            continue;
        }
        else{
            if(directoryEntry->getIsDirectory() == true){
                printDirectoryAndFilesInfo(directoryEntry->getFirstBlockNumber());
                cout << "Directory : " << directoryEntry->getFileName() << endl;
                cout << "Directory block number : " << directoryEntry->getFirstBlockNumber() << endl;
                cout << "Directory size : " << getDirectorySize(directoryEntry->getFirstBlockNumber()) << endl;
                cout << "----------------------------------" << endl;
            }
            else{
                cout << "File : " << directoryEntry->getFileNameWithExtension() << endl;
                
                vector<int> blockNumbers = fileSystem->fat->getBlockNumbersOfAFile(directoryEntry->getFirstBlockNumber());
                cout << "File block numbers : ";
                for(int i=0; i<blockNumbers.size(); i++){
                    cout << blockNumbers[i] << " ";
                }
                cout << endl;
                cout << "File size : " << directoryEntry->getFileSize() << endl;
                cout << "----------------------------------" << endl;
            }
        }
    }
}


void FileManager::dumpe2fs(){

    cout << "Superblock : " << endl;
    cout << "Block count : " << fileSystem->superBlock->getNumberOfBlocks() << endl;
    cout << "Block size : " << fileSystem->superBlock->getBlockSize() << endl;
    cout << "Free block count : " << fileSystem->superBlock->getNumberOfFreeBlocks() << endl;

    cout << "----------------------------------" << endl;

    cout << "Files and Directories : " << endl;
    printDirectoryAndFilesInfo(fileSystem->superBlock->getRootDirectoryBlockIndex());
}