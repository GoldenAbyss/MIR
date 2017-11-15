#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include "../include/data_types.h"
#include "../include/file_operations.h"
#include "../include/meta_explorer.h"

#define BACKUP_FILENAME "pad00000.meta.backup"

int compare_meta_offset(void const *a, void const *b) ;
int sort_by_folder_name_and_file_name(void const *a, void const *b);
typedef enum {RED,GREEN,YELLOW} Color;
long countPatchedFiles();
void printMainHeader();
long locateHash(long hash,MemFileInt* metaFileInMemory,MetaFileInfo* metaFileInfo);
char* concatenate (char* str1, char*str2); // Merges 2 strings into 1
char* endian_convert(int num);
int hexToInt(char* hex);
char* intToHex(int x);
void printColor(char* stringToPrint, Color COLOR);
void addToStringArray(char* fileNameToAdd, char*** ref_stringArray, int* ref_arrraySize);
int alreadyInStringArray(char* fileName,char** stringArray, int arrraySize);
long locateIntInMemory(int intToLocate,MemFileInt* arrayInMemory, MetaFileInfo* metaInfo);
void charReplace(char* str,char token,char replace);
char* substr(char* str,int start, int length);
void printFileBlock(FileBlock fileBlock);
int indexOf(char token, char* str, int skips);
char** getBackupList(long* backupCount);
int selectBackup(char** backupNames, long backupCount);
void createBackup();
void backupMenu();
int backupExists();
char* getLatestBackup();
void restoreBackup(char* backupName);
int isNumOrLetter(char c);
void undoLastChanges();
void createPath(char* pathToCreate);
void askConfirmation();
char* getTwoDigits(char* fileName);
char* getMiddleDigits(char* fileName);
char* getFourDigits(char* fileName);
int contains(char* fullString, char* substring);
char* getClassPrefix(char* fileName);
char* getLastFolder(char* folderName);

void createLogFile();
char* bdoRootFolder();
void generateReferenceFile();
int metaFileChangedSize();
int createMenu(MenuElements menu);
int createArrowMenu(MenuElements menu, char** subtittle, int defaultSelected);
int compare(void const *a, void const *b);
FileBlock* binarySearchFileBlock(char* key, FileBlock* fileBlocks, int fileblocksCount);
char* getFileBlockFullPath(FileBlock* fileBlock);
char** getTexturesFrom(FileBlock* fileBlock, char* fileLocation, int* return_texture_count);
int getWindowsVersion();
int isPatched(FileBlock* fileBlock, MetaFileInfo* metaFileInfo, FILE* metaFile);
void listPatchedFiles(int patchedFilesCount);
void PAUSE();
void preventFileRecheck();
#endif // UTILITIES_H
