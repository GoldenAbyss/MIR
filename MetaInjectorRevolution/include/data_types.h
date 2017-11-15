#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <stdio.h>
#include <stdlib.h>
typedef struct
{
    long hash;
    long folderNum;
    long fileNum;
    long pazNum;
    long metaOffset;
    long fileOffset;
    long zsize;
    long size;
    char* folderName;
    char* fileName;
    char* originalPath;
    int needPatch;
    int changeID;
} FileBlock;

typedef struct
{
    char* bytes;
    long size;
} MemFile;

typedef struct
{
    long pazCount;
    long filesCount;
    long fileBlocksCount;
    long fileBlocksStart;
    long fileBlocksEnd;
    long originalFileBlocksStart;
    long originalFileBlocksEnd;
    long actualFileBlocksCount;
    long missingFilesCount;
} MetaFileInfo;

typedef struct
{
    int* block;
    long size;
} MemFileInt;

typedef struct
{
    char header[256];
    char menuoptions[256][2048];
    int nOptions;
} MenuElements;

typedef struct
{
    char* fileName;
    int patched;
    int totalFails;
} FileToPatch;

#endif // DATA_TYPES_H
