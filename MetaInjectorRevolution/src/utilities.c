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

void getLatestBackup(char *buffer, const int len)
{
	char padFilePath[MAX_PATH];

	if (fileExists(BACKUP_FILENAME))
	{
		strncpy(buffer, BACKUP_FILENAME, len);
	}
	else
	{
		getPadFilePath(buffer, len);
    }
}

// Merges 2 strings into 1
char* concatenate (char* str1, char*str2)
{
    char* result = (char*)malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(result,str1);	// unsafe
    strcat(result,str2);	// unsafe
    return result;
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

/**
 * Create a substring
 */
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
	// Free no in the same function. Danger !
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

char** getBackupList(long* backupCount)
{
    return getFilesSingleFolder(getCurrentPath(),"backup",backupCount);
}

int createBackup()
{
	char cmd[256];
	char modFilePath[MAX_PATH];
	char root[MAX_PATH];

	getBDORootFolder(root, sizeof(root));

	sprintf(modFilePath, "%s\\latest_modifications_meta_injector.bin", TEMP_DIR);
    remove(modFilePath);

	// TODO Eviter d'utiliser une commande système pour simplement copier un fichier
	// Risque de blocage dans des cas particulier
	sprintf(cmd,"copy /y \"%s%s\" %s", root, META_FILE_SUBDIR, BACKUP_FILENAME);
    system(cmd);

    if (fileExists(BACKUP_FILENAME))
    {
		return 0;
    }
	return 1;
}

/*
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
			// Si le meta file actuel correspond à l'un des backup, cela veut dire qu'on est bon
			// Si par contre le fichier actuel n'a aucun backup, on le créé
            return 1;
        }
        fclose(backupFile);
    }
    fclose(metaFile);
    return 0;
}
*/

/**
 * Erase the current pad00000.meta and replace it by the backup file
 * The param has been remove because there is only one backup now
 */
void restoreBackup()
{
	char modFilePath[MAX_PATH];
	char root[MAX_PATH];
    char command[2048];

	getBDORootFolder(root, sizeof(root));

    sprintf(command,"xcopy /y \"%s\" \"%s%s\"", BACKUP_FILENAME, root, META_FILE_SUBDIR);
    system(command);

    //preventFileRecheck();

	sprintf(modFilePath, "%s\\latest_modifications_meta_injector.bin", TEMP_DIR);
    remove(modFilePath);
}

/**
 * Get the pad00000.meta file path in the buffer
 * It is "guessed" from the getBDORootFolder() function
 * @param[out] path Buffer that will contain the path to the pad file
 */
int getPadFilePath(char *path, const int len)
{
	getBDORootFolder(path, len);
	strcat(path, META_FILE_SUBDIR); // strncat
}

/**
 * Get the BDO Root folder using a registry key.
 * If the key is not found, we assume default directory
 * @param[out] buffer Buffer where the BDO root directory will be copied
 * @param[in] bufferLen Length of the buffer
 * @return 1 if the registry key wasn't found or couldn't be opened
 */
int getBDORootFolder(char *buffer, const int len)
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
			strncpy(buffer, lszValue, len); //strlen(lszValue));

			RegCloseKey(hKey);
			return 0;
		}
	}

	printf("Could not guess default directory (%d)\n", returnStatus);
	printf("Fall back to %s\n", BDO_DEFAULT_ROOT_DIRECTORY);

	strncpy(buffer, BDO_DEFAULT_ROOT_DIRECTORY, len);

	RegCloseKey(hKey);

	return 1;
}

/**
 * If the pad file 
 */
void fetchPadFile()
{

}

void PAUSE()
{
    printf("\n\n");
    system("PAUSE");
}

void preventFileRecheck()
{
	char root[MAX_PATH];
	char latestBackupPath[MAX_PATH];

    printf("\nPreventing File Check...\n");

	getBDORootFolder(root, sizeof(root));

    char* configFileCheckPath = concatenate(root,"config.filecheck");
    char* versionDatPath = concatenate(root,"version.dat");


	getLatestBackup(latestBackupPath, sizeof(latestBackupPath));
    FILE* metaFile = openFile(latestBackupPath,"rb");

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

