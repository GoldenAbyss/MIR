#ifndef PATCHER_H
#define PATCHER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/global.h"
#include "../include/utilities.h"
#include "../include/file_operations.h"
#include "../include/meta_explorer.h"

int autoPatch();
void patchMetaFile(const char *padFilePath, FileBlock* filesToPatch, const int filesToPatchCount);
void addToFilesToPatch(FileBlock* fileBlockFound, FileBlock* filesToPatch);
void copyFilesBack(const char *patchDir, FileBlock* filesToPatch, int filesToPatchCount);
#endif
