#include "../include/patcher.h"


/* **********************************************************************
 *
 *                        RUN PATCHER
 *
 ************************************************************************/
/**
 * Patch the files automatically
 * @return 0 All files patched successfuly
 * @return 1 If the patching has failed
 * @return 2 Some file could not be patched
 */
int autoPatch()
{
    FileBlock* fileBlocks = NULL;
    FileBlock* filesToPatch = NULL;
    long filesToPatchCount = 0;
    MetaFileInfo* metaFileInfo = NULL;
    int filesNotPatchedCount = 0;
    int filesSkippedCount = 0;
    int filesNotFoundCount = 0;

    int i = 0, j = 0;

	int returnCode = 0;

    // printf("\nCounting files...\n\n");
    // Counts how many files there is in the "files_to_patch" folder assigns to filesToPatchCount,
    // and get all the file names in folders and sub folders and returns them as an array of strings
    filesToPatch = getAllFiles("files_to_patch\\","*",&filesToPatchCount);

    printf("%ld files to be patched...\n\n", filesToPatchCount);

    if (!backupExists())
    {
        createBackup();
    }

    metaFileInfo = getMetaFileInfo(getLatestBackup());
    fileBlocks = fillFileBlocks(metaFileInfo);

    /* for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        printf("%ld|%ld|%ld|%ld|%s|%s\n",fileBlocks[i].hash,fileBlocks[i].folderNum,fileBlocks[i].fileNum,fileBlocks[i].pazNum,fileBlocks[i].folderName,fileBlocks[i].fileName);
    }*/

    printf("\nSearching for files in the meta file...");

    FileBlock* fileBlockFound = NULL;

	// On force l'usage du chemin absolu
    for (j = 0; j < filesToPatchCount; j++)
    {
		// Deux fichiers ignoré. Peut-être pour éviter d'écraser les fichiers modifié par le mode resorpless. TODO vérifier si justifié
        if ((strcmpi(filesToPatch[j].fileName,"multiplemodeldesc.xml") == 0) ||(strcmpi(filesToPatch[j].fileName,"partcombinationdesc.xml") == 0))
        {
             printf("\n%d - %s%s ",filesNotFoundCount+1,filesToPatch[j].originalPath,filesToPatch[j].fileName);

             printColor("Skipped", YELLOW);
             filesToPatch[j].needPatch = -2;
             filesSkippedCount++;
        }
        else
        {
            // Searches all the File Blocks for the file name
            fileBlockFound = NULL;


            for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
            {
                //printf("%ld|%ld|%ld|%ld|%s|%s\n",fileBlocks[i].hash,fileBlocks[i].folderNum,fileBlocks[i].fileNum,fileBlocks[i].pazNum,fileBlocks[i].folderName,fileBlocks[i].fileName);

				// Si le fileblock a le même nom de fichier on continue
                if (strcmpi(fileBlocks[i].fileName,filesToPatch[j].fileName) == 0)
                {
					// Ensuite on vérifie qu'ils sont dans le même dossier
                    if (strcmpi(filesToPatch[j].originalPath,fileBlocks[i].folderName) == 0)
                    {
                        fileBlockFound = &fileBlocks[i];
                        break; // Prevents it to keep looking the other file blocks
                    }
                }
            }

			printf("\n%d - %s%s ", j+1, filesToPatch[j].originalPath, filesToPatch[j].fileName);

            if (fileBlockFound != NULL) // If the name was found
            {
                addToFilesToPatch(fileBlockFound, &filesToPatch[j]);
                printColor("Found!", GREEN);
            }
            else
            {
                printColor("Not Found.", RED);
                filesNotPatchedCount++;
                filesNotFoundCount++;
            }
        }
    }

	if (filesNotFoundCount >= filesToPatchCount)
	{
		// Cas dans lequel aucun fichier n'est trouvé
		printf("\nCould not apply patch... FAILURE\n");
		returnCode = 1;
	}
	if (filesNotFoundCount >= 1)
	{
		// Si l'un des fichiers n'a pas pu être patché
		// pour le moment on continue quand même l'opération
		// mais on indique qu'elle ne s'est pas passé correctement

		printf("\nSome files could not be patched...\n");
		returnCode = 2;
	}


	/// COPYING FILES
	// On copie les fichiers qui vont être patché.
	// Seul les fichiers patché sont copié car contenu dans *filesToPatch
	printf("\n\nCopying files to BDO root directory...\n");
	copyFilesBack(filesToPatch, filesToPatchCount);

	/// PATCHING META FILE
	printf("\nPatching meta file...\n");
	patchMetaFile(filesToPatch, filesToPatchCount, 1);

	printf("\n\nDone.\n");

	filesNotPatchedCount = 0;
	for (i = 0; i < filesToPatchCount; i++)
	{
		if (filesToPatch[i].needPatch == 0)
		{
			filesNotPatchedCount++;
		}
	}
	
	if(filesNotPatchedCount > 0)
    {
        printColor("\nThose files have failed to patch:\n", RED);
        for(i = 0; i < filesToPatchCount; i++)
        {
            if(filesToPatch[i].needPatch == 0)
            {
                printf("%s\n",filesToPatch[i].fileName);
            }
        }
        printf("\n");
    }


    printColor("\n\n-------------- Overall Results --------------\n", YELLOW);
    printf("Patched: \033[32;1m%ld\033[0m Not Patched: \033[31;1m%d\033[0m Skipped: \033[33;1m%d\033[0m\n",
		filesToPatchCount - filesNotPatchedCount - filesSkippedCount, filesNotPatchedCount, filesSkippedCount);

	// Free memory
    for(i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        free(fileBlocks[i].fileName);
        free(fileBlocks[i].folderName);
    }

    free(fileBlocks);

    for(i = 0; i < filesToPatchCount; i++)
    {
        free(filesToPatch[i].fileName);
        free(filesToPatch[i].originalPath);
    }
    free(filesToPatch);


	return returnCode;
}


void patchMetaFile(FileBlock* filesToPatch, int filesToPatchCount, int menu1ChosenOption)
{
    int i = 0;
    int filesPatched = 0;
    long random_folder_num = 1;
    long random_file_num = 60556;

    /*printf("\n\nWe are about to make changes to your pad00000.meta file.");
    printf("\nPlease make sure you have made a manual backup before proceeding...\n\n");

    system("PAUSE");*/

    //printf("\n\nPatching Meta File...\n");

    int needToPatchAtLeastOne = 0;
    for (i = 0; i < filesToPatchCount; i++)
    {
        if(filesToPatch[i].needPatch == 1)
        {
            needToPatchAtLeastOne = 1;
            break;
        }
    }
    if (needToPatchAtLeastOne == 0)
    {
        printf("No files to patch");
        return;
    }

     /// QUICK SORT "Files to patch" according to the meta offset
    qsort(filesToPatch,filesToPatchCount,sizeof(FileBlock),compare_meta_offset);


    FILE* metaFile = openFile("pad00000.meta","rb+");
    createPath("patcher_resources\\");
    FILE* modFile = fopen("patcher_resources\\latest_modifications_meta_injector.bin","wb+");

     printf("        HASH  FOLDER_NUM FILE_NUM BYTE_IN_META_FILE| FILE_NAME");
    for (i = 0; i < filesToPatchCount; i++)
    {
        if(filesToPatch[i].needPatch == 1)
        {
            if (menu1ChosenOption == 0)
            {
                 printf("\n%.3d - %s %s  %s    (%.8ld)    | %s%s",filesPatched + 1, endian_convert((int)filesToPatch[i].hash),endian_convert((int)filesToPatch[i].folderNum),endian_convert((int)filesToPatch[i].fileNum), filesToPatch[i].metaOffset, substr(filesToPatch[i].originalPath,strlen("files_to_patch/"), strlen(filesToPatch[i].originalPath)), filesToPatch[i].fileName);
            }
            else
            {
                 printf("\n%.3d - %s %s  %s    (%.8ld)    | %s",filesPatched + 1, endian_convert((int)filesToPatch[i].hash),endian_convert((int)filesToPatch[i].folderNum),endian_convert((int)filesToPatch[i].fileNum), filesToPatch[i].metaOffset, filesToPatch[i].fileName);
            }

            // Backup the original values to the  latest modification file
            fwrite(&filesToPatch[i].metaOffset,sizeof(long),1,modFile);
            fwrite(&filesToPatch[i].folderNum,sizeof(long),1,modFile);
            fwrite(&filesToPatch[i].fileNum,sizeof(long),1,modFile);

            fseek(metaFile,filesToPatch[i].metaOffset + sizeof(long), SEEK_SET); // Goes to the position right after the Hash, which has the folder num value

            /// The most important part, breaks the meta file index by changing the 2 next ints after the hash with any random value that doesn't have a folderNum && fileNum match
            fwrite(&random_folder_num, sizeof(long),1,metaFile);
            fwrite(&random_file_num, sizeof(long),1,metaFile);

            filesPatched++;
            /*if (filesToPatchCount < 1000)
            {
                Sleep(12);
            }
            else
            {
                Sleep(5);
            }*/
        }
    }

    fclose(metaFile);
    fclose(modFile);
}


void addToFilesToPatch(FileBlock* fileBlockFound, FileBlock* filesToPatch)
{
    filesToPatch->metaOffset = fileBlockFound->metaOffset;
    filesToPatch->hash = fileBlockFound->hash;
    filesToPatch->folderNum = fileBlockFound->folderNum;
    filesToPatch->fileNum = fileBlockFound->fileNum;
    filesToPatch->folderName = fileBlockFound->folderName;
    filesToPatch->needPatch = 1;
}

void copyFilesBack(FileBlock* filesToPatch, int filesToPatchCount)
{
	char root[MAX_PATH];

	long filesCopied = 0;
    int filesNotCopied = 0;
    int filesToCopyCount = 0;

	getBDORootFolder(root, sizeof(root));

    for(int i = 0; i < filesToPatchCount; i++)
    {
        if(filesToPatch[i].needPatch == 1)
        {
            filesToCopyCount++;
        }
    }

    for(int i = 0; i < filesToPatchCount; i++)
    {
        if(filesToPatch[i].needPatch == 1)
        {
            char* folderPath = concatenate(root,filesToPatch[i].folderName);
            char* filePath = concatenate(folderPath,filesToPatch[i].fileName);
            createPath(folderPath);

			// On replace tous les fichiers directement. Pas de confirm
            if (fileExists(filePath) && getFileSizeByName(filePath) > 0)
            {
                printf("(%ld/%d) - %s ", filesCopied+1, filesToCopyCount,filesToPatch[i].fileName);
                if(!systemCopy(filesToPatch[i].fileName,concatenate("files_to_patch/",filesToPatch[i].originalPath),folderPath))
                {
                    filesToPatch[i].needPatch = 0;
                    filesNotCopied++;
				}
            }
            else
            {
                printf("(%ld/%d) - %s ", filesCopied+1, filesToCopyCount,filesToPatch[i].fileName);

                if(!systemCopy(filesToPatch[i].fileName,concatenate("files_to_patch/",filesToPatch[i].originalPath),folderPath))
                {
                   filesToPatch[i].needPatch = 0;
                   filesNotCopied++;
                }
            }

                filesCopied++;

         }
    }
    printf("Done\n\n    Copied: %ld of %d files.\n",filesCopied - filesNotCopied, filesToCopyCount);


    if (filesNotCopied != 0)
    {
        printColor("\nNot copied:", RED);
        for(int i = 0; i < filesToPatchCount; i++)
        {
            if(filesToPatch[i].needPatch == 0)
            {
                printf("\n%s",filesToPatch[i].fileName);
            }
        }
        printColor("\n\nWarning: Those files failed to copy.", RED);
        PAUSE();
    }
}





