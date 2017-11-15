#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "../include/utilities.h"
#include "../include/patcher.h"


int menu0(long filesPatchedCount);
int menu1();

void menuRemoveOrRestore(int mode);

/*
TODO :
- Ne plus identifier un fichier par son nom uniquement

- Support des arguments
*/

// Ajouter le support des arguments (argv/argc)
int main(int argc, char **argv)
{
    int menu0ChosenOption = -1;
    int menu1ChosenOption = -1;
    isWindows10 = (getWindowsVersion() == 10);

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
		getch();
		return 1;
	}
	
	if (argc <= 1)
	{
		// If there is not argument, we auto patch using the content of patch_ressource

		/*
		printf("\n\nWhat do you want to do?\n");
        printf("1 - Run Injector\n");
        printf("2 - Undo last changes\n");
        printf("3 - Restore Backup\n");
        printf("4 - List patched files\n");
        printf("5 - Prevent File Re-check\n");
        printf("0 - Exit\n");
		*/
		return autoPatch(1);	// 1= Run Injector
	}
	else
	{
		printf("Mod file: %s\n", argv[1]);
		printf("Not yet implemented.\n");
	}
	// Other argument : restore
	// Will put back the original pad00000.meta

    return 0;
}
