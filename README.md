# Simplified FAT File System

This project implements a simplified FAT12-like file system in C/C++. The file system design includes a directory table, file attributes, free block management, and a superblock that contains essential information about the file system.

## Overview

- **File System Structure:** The file system is structured similar to FAT12 and supports all possible FAT12 blocks.

- **Directory Entries:** File attributes include size, last modification date and time, and file names. Directory entries are used to organize and store files.

- **Free Block Management:** The system keeps track of free blocks using a File Allocation Table (FAT) structure. 

- **Superblock:** The superblock contains critical information about the file system, such as block size, root directory position, and block positions.

Explore this repository to learn more about the file system structure and how file system operations are implemented.

## Project Files

- `Constants.h`: Contains constants and macros used throughout the program.

- `DateTime.h`: Defines a DateTime struct and functions to retrieve the current date and time.

- `DirectoryEntry.h`: Contains attributes of directory entries and functions for building the file system.

- `DirectoryTable.h`: Manages a list of directory table entries and functions for file system construction.

- `FileAllocationTable.h`: Similar to the directory table, it manages file allocation table entries and related functions.

- `FileAllocationTableEntry.h`: Defines attributes for file allocation table entries, including the next block number.

- `FileManager.h`: The file manager class is responsible for managing the file system and handling user requests.

- `FileSystem.h`: Represents the file system, containing all blocks, file allocation tables, superblocks, and the root directory.

## Operations Implemented

- `Make File System`: Creates a file system with user-defined block size and file name.

- `Read File System`: Reads an existing file system from a data file.

- `Dir`: Lists the contents of a directory specified by a given path.

- `Mkdir`: Creates a new directory under a specified path.

- `Rmdir`: Removes a directory and its contents recursively.

- `Dumpe2fs`: Displays information about the file system, including block count, free blocks, number of files and directories, and block size.

- `Write`: Creates and writes data to a file in the file system.

- `Read`: Reads data from a file in the file system.

- `Del`: Deletes a file from the file system.

## Tests

- `MakeFileSystem`: Creates an empty file system.

- `Creating directory`: Creates directories.

- `Dir`: Lists the contents of directories.

- `Write`: Creates and writes data to files.

- `Read`: Reads data from files.

- `Rmdir`: Removes directories.

- `Del`: Deletes files.

- `Dumpe2fs`: Displays file system information.

This project provides a comprehensive file system simulation, allowing users to create, manage, and interact with files and directories in a simplified FAT-like file system.
