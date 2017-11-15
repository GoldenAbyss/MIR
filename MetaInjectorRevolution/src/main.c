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
	printf("- Meta Injector Revolution alpha 0.1					  -\n");
	printf("- based on BlackFireBR Meta Injector Reloaded 2.0e        -\n");
	printf("-----------------------------------------------------------\n\n");
    
	if (!fileExists("pad00000.meta"))
	{
		printf("\nError: could not locate pad00000.meta\n");
		printf("\nMake sure you place this program in the same folder as the pad00000.meta file.\n");
		printf("\nIf it already is. Try running this program as administrator.\n");
		printf("\nIf that doesn't work either. Move your entire game's folder to your desktop and try again.");
		return 1;
	}

	if (argc <= 1)
	{
		printf("Not enought arguments.\n");
		printf("Usage: mir.exe <command>\n");
		printf("\t-autopatch : Path files in topatch directory without asking for confirmations\n");
		printf("\t-restore : Remove last changes\n");
		printf("\tmod.zip : Install a mod\n");
		return 1;
	}

	if (!strcmpi(argv[1], "-autopatch"))
	{
		return autoPatch();
	}
	else if (!strcmpi(argv[1], "-restore"))
	{
		// TODO : Restaurer le fichier pad a avant le patch
	}
	else
	{ 
		printf("Installing mod file %s\n", argv[1]);
		// TODO: Check for zip extensions
		printf("Not yet implemented\n");
	}

    return 0;
}
