#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "../include/utilities.h"
#include "../include/patcher.h"

// Ajouter le support des arguments (argv/argc)
int main(int argc, char **argv)
{
	printf("-----------------------------------------------------------\n");
	printf("-- Meta Injector Revolution alpha 0.1                    --\n");
	printf("-- based on Meta Injector Reloaded 2.0e                  --\n");
	printf("-- Thanks to BlackFireBR                                 --\n");
	printf("-----------------------------------------------------------\n\n");

	if (argc <= 1)
	{
		printf("Not enought arguments.\n");
		printf("Usage: mir.exe <command>\n");
		printf("\t-autopatch : Path files in topatch directory without asking for confirmations\n");
		printf("\t-backup : Create a backup for the pad00000.meta file\n");
		printf("\t-restore : Remove last changes\n");
		printf("\tmod.zip : Install a mod\n");
#ifdef _DEBUG
		system("pause"); // TODO Remove me
#endif
		return 1;
	}

	if (!strcmpi(argv[1], "-autopatch"))
	{
		int r = autoPatch("to_patch\\");
#ifdef _DEBUG
		system("pause"); // TODO Remove me
#endif
		return r;
	}
	else if (!strcmpi(argv[1], "-restore"))
	{
		restoreBackup();
	}
	else if (!strcmp(argv[1], "-backup"))
	{
		printf("Performing a clean backup");
		if (createBackup())
		{
			printf("Error while creating backup file...\n");
		}
	}
	else
	{ 
		printf("Installing mod file %s\n", argv[1]);
		// TODO: Check for zip extensions
		printf("Not yet implemented\n");
	}

#ifdef _DEBUG
	system("pause");
#endif

    return 0;
}
