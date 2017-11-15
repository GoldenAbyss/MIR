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
    
	if (!fileExists("pad00000.meta"))
	{
		printf("\nError: could not locate pad00000.meta\n");
		printf("\nMake sure you place this program in the same folder as the pad00000.meta file.\n");
		printf("\nIf it already is. Try running this program as administrator.\n");
		printf("\nIf that doesn't work either. Move your entire game's folder to your desktop and try again.");
		system("pause");
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
		runPatcher(1);	// 1= Run Injector
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
int menu0(long filesPatchedCount)
{
    char input = '\0';
    int nOptions = 5;
    int optionSelected = -1;



     // Display the menu to select the region
    while(1) // This condition makes the menu repeat itself until a valid input is entered
    {
        system("cls"); // clears the screen
        printMainHeader();
        printf("Meta file status: ");
        if (filesPatchedCount == 0)
        {
            printColor("Clean", GREEN);
        }
        else
        {
            printf("%ld files currently patched", filesPatchedCount);
        }


        printf("\n\nEnter your choice: ");
        fflush(stdin);
        scanf("%c",&input);
        fflush(stdin);

        if (input == 10) // ENTER
        {
            return 1;
        }

        // Converts the read string to int
        optionSelected = input - '0';

        // This variable also tells the program later which one of the "moddedBytes" the program should use when replacing the bytes

        if (optionSelected == 0)
        {
            return 0;
        }


         // If the user entered an invalid option, displays a error message
        if(optionSelected < 1 || optionSelected > nOptions)
        {
             printf("\nInvalid option! %c(%d)\n\n", input, input);
            Sleep(500);
        }
        else
        {
            break;
        }
    }
    return optionSelected;
}
