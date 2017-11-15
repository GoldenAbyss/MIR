#include "../include/utilities.h"

int compare(void const *a, void const *b) {
    FileBlock *aa = (FileBlock *)a;
    FileBlock *bb = (FileBlock *)b;

    char* aaa = (char*)aa->fileName;
    char* bbb = (char*)bb->fileName;

    return strcmp(aaa, bbb);
}

int compare_meta_offset(void const *a, void const *b) {
    FileBlock *aa = (FileBlock *)a;
    FileBlock *bb = (FileBlock *)b;

    long aaa = (long)aa->metaOffset;
    long bbb = (long)bb->metaOffset;

    return (aaa - bbb);
}

int sort_by_folder_name_and_file_name(void const *a, void const *b) {
    FileBlock *aa = (FileBlock *)a;
    FileBlock *bb = (FileBlock *)b;

    char* folderNameA = (char*)aa->folderName;
    char* folderNameB = (char*)bb->folderName;

    char* fileNameA =  (char*)aa->fileName;
    char* fileNameB =  (char*)bb->fileName;

    char* completePathA = concatenate(folderNameA,fileNameA);
    char* completePathB = concatenate(folderNameB,fileNameB);

    int result = strcmp(completePathA, completePathB);

    free(completePathA);
    free(completePathB);

    return result;
}

long countPatchedFiles()
{
    printf("\nCounting patched files...");
    MetaFileInfo* metaFileInfo = getMetaFileInfo(getLatestBackup());
    long filesPatched = 0;
    int i = 0;
    FileBlock moddedFileBlocks;
    FILE* metaFile = openFile("pad00000.meta", "rb");

    fseek(metaFile,metaFileInfo->fileBlocksStart,SEEK_SET);

    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        fread(&moddedFileBlocks.hash,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks.folderNum,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks.fileNum,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks.pazNum,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks.fileOffset,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks.zsize,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks.size,sizeof(long),1,metaFile);

        if ((moddedFileBlocks.folderNum == RANDOM_FOLDER_NUM) && (moddedFileBlocks.fileNum == RANDOM_FILE_NUM))
        {
            filesPatched++;
        }
    }

    if (filesPatched == 0)
    {
        rewind(metaFile);
        long readNum1 = -1;
        long readNum2 = -1;
        fread(&readNum1,sizeof(long),1,metaFile);
        while(1)
        {
            if (!fread(&readNum2,sizeof(long),1,metaFile))
            {
                break;
            }
            if (readNum1 == RANDOM_FOLDER_NUM && readNum2 == RANDOM_FILE_NUM)
            {
                filesPatched++;
            }
            readNum1 = readNum2;
        }
    }
    fclose(metaFile);
    free(metaFileInfo);

    printf("\nDone.");

    return filesPatched;
}

char* endian_convert(int num)
{
    char big_end[12];
    sprintf(big_end,"%.8x", num);

    char* little_end = (char*) malloc(9);
    int i = 0;

    for(i = 0; i < strlen(big_end); i +=2)
    {
        char firstNum = big_end[strlen(big_end) - i - 2];
        if (firstNum >= 'a' && firstNum <= 'f')
        {
            firstNum -= 32;
        }
        char secondNum = big_end[strlen(big_end) - i - 1];
        if (secondNum >= 'a' && secondNum <= 'f')
        {
            secondNum -= 32;
        }
        little_end[i] = firstNum;
        little_end[i+1] = secondNum;
    }
    little_end[8]  = '\0';
    return little_end;
}

char* getTwoDigits(char* fileName)
{
    int start = indexOf('_',fileName,0) + 1;
    int end = indexOf('_',fileName,1);

    char* twoDigits = substr(fileName,start,end);

    return twoDigits;
}

char* getMiddleDigits(char* fileName)
{
    int  start = indexOf('_',fileName,1) + 1;
    int end = indexOf('_',fileName,2);

    char* middle = substr(fileName,start,end);

    return middle;
}

char* getClassPrefix(char* fileName)
{
    return substr(fileName,0,3);
}

int contains(char* fullString, char* substring)
{
    if (strlen(substring) > strlen(fullString))
    {
        //printf("\nWarning: Wrong use of function 'contains(%s,%s)'. The first argument has to be longer than the second", fullString, substring);
        return 0;
    }
    int i = 0;
    int j = 0;
    int combo = 0;
    for (i = 0; i < strlen(fullString); i++)
    {
        if (substring[j] == fullString[i]
        || substring[j] + 32 == fullString[i]
        || substring[j] - 32 == fullString[i]
        || (substring[j] == '/' && fullString[i] == '\\')
        || (substring[j] == '\\' && fullString[i] == '/'))
        {
            combo++;
            if (j + 1 <= strlen(substring))
            {
                j++;
            }
            else
            {
                combo = 0;
                j = 0;
            }
        }
        else
        {
            combo = 0;
            j = 0;
        }
        if (combo == strlen(substring))
        {
            return 1;
        }
    }
    return 0;
}

char* getFourDigits(char* fileName)
{
    int start = indexOf('_',fileName,2) + 1;
    int end = indexOf('_',fileName,3);
    int dot = indexOf('.',fileName,0);

    if (dot < end)
    {
        end = dot;
    }

    char* fourDigits = substr(fileName,start,end);

    return fourDigits;
}

void createPath(char* pathToCreate)
{
    charReplace(pathToCreate,'/','\\');

    DIR* dir = opendir(pathToCreate);

    if (dir)
    {
        closedir(dir);
    }
    else
    {
        char* mdCommand = (char*) malloc(strlen("md \"%s\"") + strlen(pathToCreate) + 1);
        sprintf(mdCommand,"md \"%s\"",pathToCreate);
        //printf("\n%s\n",mdCommand);
        system(mdCommand);
        free (mdCommand);
    }
}

char* getLatestBackup()
{
    long backupCount = 0;
    char** backups = getBackupList(&backupCount);
    if (backupCount == 0)
    {
        return "pad00000.meta";
    }
    return backups[backupCount - 1];
}

// Merges 2 strings into 1
char* concatenate (char* str1, char*str2)
{
    char* result = (char*)malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(result,str1);	// unsafe
    strcat(result,str2);	// unsafe
    return result;
}


int isNumOrLetter(char c)
{
    if (c >= 'a' && c <='z')
    {
        return 1;
    }
    if (c >= 'A' && c <='Z')
    {
        return 1;
    }
    if (c >= '0' && c <='9')
    {
        return 1;
    }
    if (c == '_')
    {
        return 1;
    }
    return 0;
}

int hexToInt(char* hex)
{
   /*FILE* tmpFile = fopen("tmp.bin","wb");
   fwrite(hex,sizeof(int),1,tmpFile);
   int num = 0;
   rewind(tmpFile);
   fread(tmpFile)*/
   return 0;
}

char* intToHex(int x)
{
    char* hex = (char*) malloc(sizeof(int) + 1);
    sprintf(hex,"%d",x);
    return hex;
}

void printColor(char* stringToPrint, Color COLOR)
{
    if (isWindows10)
    {
        switch (COLOR)
        {
            case (RED):
            {
                printf("\033[31;1m%s\033[0m", stringToPrint);
            }break;
            case (GREEN):
            {
                printf("\033[32;1m%s\033[0m", stringToPrint);
            }break;
            case (YELLOW):
            {
                printf("\033[33;1m%s\033[0m", stringToPrint);
            }break;
        }
    }
    else
    {
        printf("%s", stringToPrint);
    }
}

void addToStringArray(char* fileNameToAdd, char*** ref_stringArray, int* ref_arrraySize)
{
    // If the variable that holds the fileNames of the files that failed to patch hasn't been initialized yet
    if ((*ref_stringArray) == NULL)
    {
        (*ref_stringArray) = (char**) malloc(sizeof(char*));
    }
    else // If it has been initialized
    {
         // Increases the size
        (*ref_stringArray) = (char**) realloc((*ref_stringArray), (((*ref_arrraySize) + 1) * sizeof(char*)));
    }
    // Allocates memory to copy the failed file name
    (*ref_stringArray)[(*ref_arrraySize)] = (char*) malloc(strlen(fileNameToAdd) + 1);
    strcpy((*ref_stringArray)[(*ref_arrraySize)],fileNameToAdd); // Copies the name of the failed file

    (*ref_arrraySize)++;
}

int isPatched(FileBlock* fileBlock, MetaFileInfo* metaFileInfo, FILE* metaFile)
{
    fseek(metaFile,fileBlock->metaOffset,SEEK_SET);
    int hashFound = 0;
    long hash = -1;
    fread(&hash,sizeof(long),1,metaFile);
    if (hash != fileBlock->hash)
    {
        while(ftell(metaFile) >= metaFileInfo->fileBlocksStart)
        {
            fseek(metaFile,-8, SEEK_CUR); // -2 * sizeof(long)
            fread(&hash,sizeof(long),1,metaFile);
            if (hash == fileBlock->hash)
            {
                hashFound = 1;
                break;
            }
        }
        if (hashFound == 0)
        {
            fseek(metaFile,fileBlock->metaOffset + sizeof(long),SEEK_SET);
            while(ftell(metaFile) < metaFileInfo->fileBlocksEnd)
            {
                fread(&hash,sizeof(long),1,metaFile);
                if (hash == fileBlock->hash)
                {
                    hashFound = 1;
                    break;
                }
            }
        }
    }
    else
    {
        hashFound = 1;
    }
    if (hashFound == 0)
    {
        return 0;
    }
    else
    {
        long folderNum = -1;
        long fileNum = -1;
        fread(&folderNum,sizeof(long),1,metaFile);
        fread(&fileNum,sizeof(long),1,metaFile);

        if (folderNum == RANDOM_FOLDER_NUM && fileNum == RANDOM_FILE_NUM)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

int alreadyInStringArray(char* fileName,char** stringArray, int arrraySize)
{
    int i = 0;
    for (i = 0; i < arrraySize; i++)
    {
        if (strcmpi(stringArray[i],fileName) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void printHex(char* str)
{
    int i = 0;
    printf("\n");
    for (i = 0; i < strlen(str); i++)
    {
        printf("%x ", str[i]);
    }
    printf("\n");
}


void charReplace(char* str,char token,char replace)
{
    int i = 0;
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == token)
        {
            str[i] = replace;

        }
    }
}

void printNextAsHex(char* str,int start, int length)
{
    int i = 0;

    for (i = start; i < (start + length); i++)
    {
        printf("%x ", str[i]);
    }
    printf("\n");
}

void printNextAsDec(char* str,int start, int length)
{
    int i = 0;
    printf("\n");
    for (i = start; i < (start + length); i++)
    {
        printf("%d ", str[i]);
    }
    printf("\n");
}

void printIntAsHex(int num)
{
    char* converted = intToHex(num);
    printHex(converted);
}

int nextFourBytesAsInt(char* str, int startLocation)
{
    int i = 0;
    char fourBytes[5] = "";
    fourBytes[4] = '\0';
    for (i = 0; i < 4; i++)
    {
        fourBytes[i] = str[startLocation+i];
    }
    return hexToInt(fourBytes);
}

char* substr(char* str,int start, int end)
{
    if (end < start)
    {
        //printf("\ntrying to create a substring where end < start |  %s start: %d   end: %d\n", str, start, end);
        return "";
    }
    if (end == start)
    {
        return "";
    }
    char* sub = (char*) malloc((end - start) + 1);
    int j = 0;
    int i = 0;
    for (i = start; i <= end; i++)
    {
        sub[j] = str[i];
        j++;
    }
    sub[j - 1] = '\0';
    return sub;
}

int indexOf(char token, char* str, int skips)
{
    int i = 0;
    int skipped = 0;
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == token)
        {
            if (skipped == skips)
            {
                return i;
            }
            else
            {
                skipped++;
            }
        }
    }
    return i -1;
}

long locateHash(long hash,MemFileInt* metaFileInMemory,MetaFileInfo* metaFileInfo)
{
    long currentBlock = 0;
    int readNum = 0;

    int startBlock = metaFileInfo->fileBlocksStart/sizeof(int);
    int endBlock = metaFileInfo->fileBlocksEnd/sizeof(int);

    for (currentBlock = startBlock; currentBlock < endBlock; currentBlock++)
    {
        readNum = metaFileInMemory->block[currentBlock];

        if(readNum == hash)
        {
            return currentBlock;
        }
    }
    return NOT_FOUND;
}


long locateIntInMemory(int intToLocate,MemFileInt* arrayInMemory, MetaFileInfo* metaFileInfo)
{
    long currentBlock = 0;
    int readNum = 0;

    for (currentBlock = 0; currentBlock < arrayInMemory->size; currentBlock++)
    {
        readNum = arrayInMemory->block[currentBlock];


        int startBlock = metaFileInfo->fileBlocksStart/sizeof(int);
        int endBlock = metaFileInfo->fileBlocksEnd/sizeof(int);


        if(readNum == intToLocate)
        {
            if ((currentBlock < startBlock || currentBlock > endBlock))
            {
                printf("                       OUTSIDE!");
                 return NOT_FOUND;
            }
            return currentBlock;

        }

    }
    return NOT_FOUND;
}

long locate2IntsInMemory(int int1, int int2 ,MemFileInt* arrayInMemory)
{
    //printf("\nlooking for: %.8x | %.8x\n", int1, int2);
    long currentBlock = 0;
    for (currentBlock = 0; currentBlock < arrayInMemory->size; currentBlock++)
    {

        if(arrayInMemory->block[currentBlock] == int1 && arrayInMemory->block[currentBlock + 1] == int2)
        {
            return currentBlock;
        }

    }
    return NOT_FOUND;
}

void undoLastChanges()
{
	FILE *metaFile, *modFile;
	char modFilePath[MAX_PATH];
	sprintf(modFilePath, "%s\\latest_modifications_meta_injector.bin", TEMP_DIR);

    metaFile = openFile("pad00000.meta","rb+");
    modFile = fopen(modFilePath,"rb");

    if (modFile == NULL)
    {
        printf("\nNo last modifications detected.\n\n");
        fclose(metaFile);
        system("PAUSE");
        return;
    }

    printf("\n\nReverting File");
    if (getFileSize(modFile) == 0)
    {
        printf("\nNo changes to undo.\n\n");
        fclose(metaFile);
        fclose(modFile);
        system("PAUSE");
        return;
    }
    else
    {
        MetaFileInfo* metaFileInfo = getMetaFileInfo("pad00000.meta");
        fseek(metaFile,metaFileInfo->fileBlocksStart,SEEK_SET);;

        long offset = 0;
        long folderNum = 0;
        long fileNum = 0;
        long changesCount = 0;


        while(fread(&offset,sizeof(long),1,modFile))
        {
            fread(&folderNum,sizeof(long),1,modFile);
            fread(&fileNum,sizeof(long),1,modFile);

            fseek(metaFile,offset + sizeof(long),SEEK_SET);

            fwrite(&folderNum,sizeof(long),1,metaFile);
            fwrite(&fileNum,sizeof(long),1,metaFile);

            changesCount++;
        }

        printf("\nReverted changes: %ld\n\n", changesCount);

        fclose(modFile);
        remove(modFilePath);

        fclose(metaFile);
        free(metaFileInfo);
        system("PAUSE");
    }
}


/*void toHex(int decimalNumber)
{
	int remainder;
	int quotient;
    int i=1,j;
    char hexadecimalNumber[100];

    quotient = decimalNumber;

    while(quotient!=0)
	{
         remainder = quotient % 16;

      //To convert integer into character
      if( remainder < 10)
           remainder = remainder + 48;		// Add 48(become ascii later) value to remainder if less than 10--see ascii table for more info
      else
         remainder = remainder + 55;		// Add 55(become ascii later) value to remainder if greater than 10--see ascii table for more info

      hexadecimalNumber[i++]= remainder;

      quotient = quotient / 16;
    }

    //printf("Equivalent hexadecimal value of %d is: ",decimalNumber);

    for(j = i -1 ;j> 0;j--)
    {
      printf("%c",hexadecimalNumber[j]);
    }

}

void printNextBytesAsHex(FILE* fp, int howManyBytes)
{
    int i = 0;
    char c;
    for (i = 0; i < howManyBytes; i++)
    {
        fread(&c,1,1,fp);
        printf("%x ", c);
    }
}*/



char** getBackupList(long* backupCount)
{
    return getFilesSingleFolder(getCurrentPath(),"backup",backupCount);
}

int selectBackup(char** backupNames, long backupCount)
{
    long i = 0;
     char input[32];
     int optionSelected = -1;
    int backupSelected = -1;

    printf("\n\nList of backups:\n");
    for (i = 0; i < backupCount; i++)
    {
       printf("%ld - %s\n", i + 1, backupNames[i]);
    }
    printf("0 - Back\n");
    while (1)
    {
        printf("\nSelect backup number: ");
        fgets(input,32,stdin);

        optionSelected = atoi(input);
        backupSelected = optionSelected - 1;

        if (optionSelected == EXIT)
        {
            return -1;
        }

        if (optionSelected < 0 || optionSelected > backupCount)
        {
            printf("\nInvalid option!\n\n");
        }
        else
        {
            break;
        }
    }

    return backupSelected;
}

void createBackup()
{
	char cmd[256];
	char modFilePath[MAX_PATH];

	sprintf(modFilePath, "%s\\latest_modifications_meta_injector.bin", TEMP_DIR);
    remove(modFilePath);

	// TODO Eviter d'utiliser une commande système pour simplement copier un fichier
	// Risque de blocage dans des cas particulier
	sprintf(cmd,"copy /y pad00000.meta %s", BACKUP_FILENAME);
    system(cmd);

    if (fileExists(BACKUP_FILENAME))
    {
        printf("\nA new backup was created: %s\n", BACKUP_FILENAME);
    }
    else
    {
        printf("\nCould not create a backup\n");
        printf("Try running the program as administrator\n");
        printf("\nThe program will try to ignore this file, but it will most likely, not work properly.");
        PAUSE();
    }
}

int backupExists()
{
    long backupCount = 0;

	// Donne un listing de tous les fichiers finissant par backup
    char** backups = getFilesSingleFolder(getCurrentPath(),"backup", &backupCount);

    FILE* backupFile = NULL;
    FILE* metaFile = openFile("pad00000.meta","rb");

	// TODO Vérification par checksum plutôt que par taille de fichier
    int i = 0;
    for(i = 0; i < backupCount; i++)
    {
        backupFile = fopen(backups[i],"rb");
        if (getFileSize(backupFile) == getFileSize(metaFile))
        {
            fclose(metaFile);
            fclose(backupFile);
            return 1;
        }
        fclose(backupFile);
    }
    fclose(metaFile);
    return 0;
}

void restoreBackup(char* backupName)
{
	char modFilePath[MAX_PATH];
    char command[2048];
    sprintf(command,"xcopy /y \"%s\" pad00000.meta",backupName);
    system(command);
    preventFileRecheck();

	sprintf(modFilePath, "%s\\latest_modifications_meta_injector.bin", TEMP_DIR);
    remove(modFilePath);
}

/**
 * Get the BDO Root folder using a registry key.
 * If the key is not found, we assume default directory
 * @param[out] buffer Buffer where the BDO root directory will be copied
 * @param[in] bufferLen Length of the buffer
 * @return 1 if the registry key wasn't found or couldn't be opened
 */
int getBDORootFolder(char *buffer, int bufferLen)
{
	char lszValue[MAX_PATH];
	HKEY hKey;
	LONG returnStatus;
	DWORD dwType = REG_SZ;
	DWORD dwSize = MAX_PATH;
	returnStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, BDO_REGISTRY_KEY, NULL, KEY_READ, &hKey);
	if (returnStatus == ERROR_SUCCESS)
	{
		returnStatus = RegQueryValueEx(hKey, TEXT("InstallLocation"), NULL, &dwType, (LPBYTE)&lszValue, &dwSize);
		if (returnStatus == ERROR_SUCCESS)
		{
			strcat(lszValue, "\\");
			strncpy(buffer, lszValue, bufferLen); //strlen(lszValue));
			printf("debug: %s\n", buffer);

			RegCloseKey(hKey);
			return 0;
		}
	}

	printf("Could not guess default directory (%d)\n", returnStatus);
	printf("Fall back to %s\n", BDO_DEFAULT_ROOT_DIRECTORY);
	RegCloseKey(hKey);

	return 1;
}

void PAUSE()
{
    printf("\n\n");
    system("PAUSE");
}

void preventFileRecheck()
{
	char root[MAX_PATH];

    printf("\nPreventing File Check...\n");

	getBDORootFolder(root, sizeof(root));

    char* configFileCheckPath = concatenate(root,"config.filecheck");
    char* versionDatPath = concatenate(root,"version.dat");


    FILE* metaFile = openFile(getLatestBackup(),"rb");
    FILE* versionDatFile = fopen(versionDatPath,"rb");


    int clientVersion_versionDat = 0;
    int clientVersion_metaFile = 0;



    fread(&clientVersion_metaFile,sizeof(int),1,metaFile);
    if (versionDatFile != NULL)
    {
         long versionDatFileSize = getFileSize(versionDatFile);
         char* versionDatAsString = (char*) malloc(versionDatFileSize);

        fread(versionDatAsString,sizeof(char),versionDatFileSize,versionDatFile);
        fclose(versionDatFile);

        versionDatAsString[versionDatFileSize-1] = '\0';

        clientVersion_versionDat = atoi(versionDatAsString);
        free(versionDatAsString);

        if (clientVersion_versionDat != clientVersion_metaFile)
        {
            char proceed = 0;
            printf("\nWarning: Client version from version.dat (%d) differs from the pad00000.meta file (%d).", clientVersion_versionDat,clientVersion_metaFile);
            printf("\nAre you sure you want to proceed? (y/n)\n");
            printf(":");
            scanf("%c", &proceed);

            if (proceed != 'y' && proceed != 'Y')
            {
                free(configFileCheckPath);
                free(versionDatPath);
                free(root);
                fclose(metaFile);
                printf("\nNo.");
                return;
            }
            printf("\nYes.");
        }
    }

    if (!remove(configFileCheckPath))
    {
        printf("\nconfig.filecheck - successfully deleted.\n");
    }

    printf("\nCreating version.dat file...\n");
    versionDatFile = openFile(versionDatPath,"wb");
    if (versionDatFile != NULL)
    {
        char endByte = '\x0A';
        char clientVersionAsString[16];
        sprintf(clientVersionAsString,"%d",clientVersion_metaFile);
        fwrite(&clientVersionAsString,sizeof(char),strlen(clientVersionAsString),versionDatFile);
        fwrite(&endByte,1,1,versionDatFile);
        fclose(versionDatFile);
    }
    printf("Done.");
    free(configFileCheckPath);
    free(versionDatPath);
    free(root);
    fclose(metaFile);

}

