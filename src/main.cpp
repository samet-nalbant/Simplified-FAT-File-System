#include <iostream>

#include "../include/FileManager.h"

using namespace std;

typedef enum Operations{
    FILE_SYSTEM_OPER,
    MAKE_FILE_SYSTEM,
    INVALID_OPERATION
} Operations;

typedef enum Commands{
    MKDIR,
    DIR,
    WRITE,
    RMDIR,
    DEL,
    DUMPE2FS,
    READ, 
    INVALID_COMMAND
} Commands;


void fileSystemOperHandler(int argc, char *argv[]);
void makeFileSystemHandler(int argc, char *argv[]);
Operations getOperationType(int argc, char *argv[]);
Commands getCommandType(int argc, char *argv[]);
int main(int argc, char *argv[])
{

    Operations operation = getOperationType(argc, argv);
    switch (operation)
    {
    case Operations::FILE_SYSTEM_OPER:
        fileSystemOperHandler(argc, argv);  
        break;
    case Operations::MAKE_FILE_SYSTEM:
        makeFileSystemHandler(argc, argv);
        break;
    case Operations::INVALID_OPERATION:
        cout << "INVALID_OPERATION" << endl;
        break;
    default:
        break;
    }
    return 0;
}

Operations getOperationType(int argc, char *argv[]){
    if(argc < 2 || argc > 6){
        cout << "Invalid number of arguments" << endl;
        return Operations::INVALID_OPERATION;
    }
    if(strcmp(argv[1], "fileSystemOper") == 0){
        return Operations::FILE_SYSTEM_OPER;
    }
    else if(strcmp(argv[1], "makeFileSystem") == 0){
        return Operations::MAKE_FILE_SYSTEM;
    }
    else{
        return Operations::INVALID_OPERATION;
    }
}

Commands getCommandType(int argc, char *argv[]){
    if(argc < 4){
        cout << "Invalid number of arguments" << endl;
        return Commands::INVALID_COMMAND;
    }
    if(strcmp(argv[3], "mkdir") == 0){
        return Commands::MKDIR;
    }
    else if(strcmp(argv[3], "dir") == 0){
        return Commands::DIR;
    }
    else if(strcmp(argv[3], "write") == 0){
        return Commands::WRITE;
    }
    else if(strcmp(argv[3], "rmdir") == 0){
        return Commands::RMDIR;
    }
    else if(strcmp(argv[3], "del") == 0){
        return Commands::DEL;
    }
    else if(strcmp(argv[3], "dumpe2fs") == 0){
        return Commands::DUMPE2FS;
    }
    else if(strcmp(argv[3], "read") == 0){
        return Commands::READ;
    }
    else{
        return Commands::INVALID_COMMAND;
    }
}

void makeFileSystemHandler(int argc, char *argv[]){
    if(argc != 4){
        cout << "Invalid number of arguments" << endl;
        return;
    }
    else{
        FileManager::createFileSystem(argv[3], atoi(argv[2]));
    }
}


void fileSystemOperHandler(int argc, char *argv[]){
    Commands command = getCommandType(argc, argv);
    switch (command)
    {
    case Commands::MKDIR:
        if(argc != 5){
            cout << "Invalid number of arguments" << endl;
            return;
        }
        else{
            string volumeName = argv[2];
            string directoryName = argv[4];
            FileManager *fm = new FileManager(volumeName);
            fm->createDirectory(directoryName);
            fm->saveAndExit();
            delete fm;
        }
        break;
    case Commands::DIR:
        if(argc != 5){
            cout << "Invalid number of arguments" << endl;
            return;
        }
        else{
            string volumeName = argv[2];
            string directoryName = argv[4];
            FileManager *fm = new FileManager(volumeName);
            fm->printDirectoryContent(directoryName);
            delete fm;
        }
        break;
    case Commands::WRITE:
        if(argc != 6){
            cout << "Invalid number of arguments" << endl;
            return;
        }
        else{
            string volumeName = argv[2];
            string filePath = argv[4];
            string contentFile = argv[5];
            FileManager *fm = new FileManager(volumeName);
            fm->createFile(filePath, contentFile);
            fm->saveAndExit();
            delete fm;
        }
        break;
    case Commands::RMDIR:
        if(argc != 5){
            cout << "Invalid number of arguments" << endl;
            return;
        }
        else{
            string volumeName = argv[2];
            string directoryName = argv[4];
            FileManager *fm = new FileManager(volumeName);
            fm->removeDirectory(directoryName);
            fm->saveAndExit();
            delete fm;
        }
        break;
    case Commands::DEL:
        if(argc != 5){
            cout << "Invalid number of arguments" << endl;
            return;
        }
        else{
            string volumeName = argv[2];
            string filePath = argv[4];
            FileManager *fm = new FileManager(volumeName);
            fm->removeFile(filePath);
            fm->saveAndExit();
            delete fm;
        }
        break;
    case Commands::DUMPE2FS:
        if(argc != 4){
            cout << "Invalid number of arguments" << endl;
            return;
        }
        else{
            string volumeName = argv[2];
            FileManager *fm = new FileManager(volumeName);
            fm->dumpe2fs();
            delete fm;
        }
        break;
    case Commands::READ:
        if(argc != 6){
            cout << "Invalid number of arguments" << endl;
            return;
        }
        else{
            string volumeName = argv[2];
            string filePath = argv[4];
            string outputFile = argv[5];
            FileManager *fm = new FileManager(volumeName);
            fm->readFile(filePath, outputFile);
            fm->saveAndExit();
            delete fm;
        }


        break;
    case Commands::INVALID_COMMAND: 
        cout << "INVALID_COMMAND" << endl;
        break;
    default:    
        break;
    }   
}