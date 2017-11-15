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
#define TEMP_DIR "temp\\"
#define BDO_REGISTRY_KEY "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{C1F96C92-7B8C-485F-A9CD-37A0708A2A60}"
#define BDO_DEFAULT_ROOT_DIRECTORY "C:\\Program Files (x86)\\Black Desert Online\\"

int compare_meta_offset(void const *a, void const *b) ;
int sort_by_folder_name_and_file_name(void const *a, void const *b);
typedef enum {RED,GREEN,YELLOW} Color;
long countPatchedFiles();
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
int indexOf(char token, char* str, int skips);
char** getBackupList(long* backupCount);
int selectBackup(char** backupNames, long backupCount);
void createBackup();
int backupExists();
char* getLatestBackup();
void restoreBackup(char* backupName);
int isNumOrLetter(char c);
void undoLastChanges();
void createPath(char* pathToCreate);
char* getTwoDigits(char* fileName);
char* getMiddleDigits(char* fileName);
char* getFourDigits(char* fileName);
int contains(char* fullString, char* substring);
char* getClassPrefix(char* fileName);

int getBDORootFolder(char *buffer, int bufferLen);

int compare(void const *a, void const *b);
int isPatched(FileBlock* fileBlock, MetaFileInfo* metaFileInfo, FILE* metaFile);
void PAUSE();
void preventFileRecheck();
#endif // UTILITIES_H
