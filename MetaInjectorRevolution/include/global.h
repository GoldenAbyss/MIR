#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdlib.h>
#include <stdio.h>
#define SLEEP_TIME 1000
#include "../include/data_types.h"
#pragma warning(disable:4996)

#define RANDOM_FOLDER_NUM 1
#define RANDOM_FILE_NUM 60556

#define NOT_FOUND -1
#define ENTER 13
#define PAGE_UP 73
#define PAGE_DOWN 81
#define BACKSPACE 8
#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define ESC 27


#define REMOVE_ALL_ARMORS 0
#define REMOVE_STARTER_ARMORS 1
#define REMOVE_REGULAR_ARMORS 2
#define REMOVE_LIFE_SKILL_ARMORS 3
#define KEEP_LIFE_SKILL_ARMORS_ONLY 4
#define KEEP_ALL_ARMORS 5

#define REMOVE_ALL_UNDERWEAR 0
#define REMOVE_STARTER_UNDERWEAR 1
#define KEEP_ALL_UNDERWEAR 2

#define REMOVE_PANTIES_AND_PANTS 0
#define REMOVE_STARTER_PANTIES 1
#define REMOVE_PANTIES_ONLY 2
#define REMOVE_PANTS_ONLY 3
#define REMOVE_ALL_PANTIES_AND_PANTS 4
#define KEEP_ALL_PANTIES_AND_PANTS 5

#define REMOVE_ALL_GLOVES 0
#define REMOVE_STARTER_GLOVES 1
#define REMOVE_REGULAR_GLOVES 2
#define KEEP_ALL_GLOVES 3

#define REMOVE_ALL_BOOTS 0
#define REMOVE_STARTER_BOOTS 1
#define REMOVE_REGULAR_BOOTS 2
#define KEEP_ALL_BOOTS 3

#define REMOVE_ALL_HELMETS 1
#define KEEP_ALL_HELMETS 0

#define REMOVE_ALL_STOCKINGS 0
#define REMOVE_STARTER_STOCKINGS 1
#define KEEP_ALL_STOCKINGS 2


#define REMOVE_SORC_WEAPONS 0
#define REMOVE_STARTER_WEAPONS 1
#define REMOVE_IN_WEAPONS 2
#define REMOVE_BOTH_WEAPONS 3
#define REMOVE_BOWS_AND_SHIELDS 4
#define REMOVE_ALL_WEAPONS 5
#define KEEP_ALL_WEAPONS 6

#define FEMALE_ONLY 0
#define MALE_ONLY 1
#define BOTH_GENDERS 2

#define SUCCESS 1
#define FAILURE 0

#define EXIT 0
#define BACK 8

#define F1 59

#define INSTALL 1
#define REMOVE 2
#define RESTORE 3
#define BACKUP_RESTORE 4

#define TOTAL_PANTIES 66
#define TOTAL_PANTS 41

#define PATCHER_OPTIONS_SIZE 15



typedef enum {ARMOR,UNDERWEAR,LOWERBODY,GLOVES,BOOTS,HELMETS,STOCKINGS,WEAPONS,GENDER,SHOULDER,CLOAK} PatcherOption;
typedef enum {SHAVED,FULL_BUSH,MEDIUM_BUSH,MEDIUM_BUSH_2,SMALL_BUSH,SMALL_BUSH_2,THIN_LANDING_STRIP,WIDE_LANDING_STRIP,TRIMMED,WIDER_TRIMMED} PubicHairOption;
typedef enum {REMOVE_ALL,STARTER,KEEP_ALL,REGULAR,LIFE_SKILL,PANTIES_ONLY,IN} Restriction;

typedef enum {WARRIOR,SORCERESS,RANGER,BERSERKER,TAMER,BLADER,VALKYRIE,WIZARD,WITCH,KUNOICHI,NINJA,PLUM} Class;

typedef enum {PHM,PHW,PEW,PGM,PBW,PKM,PVW,PWM,PWW,PNW,PNM,PKWW} ClassPrefix;

extern int isWindows10;

extern char *starters[][12];
extern char* panties[];
extern char* pants[];
extern char* starterStockings[];
extern char* starterUnderwearTextures[];
extern char* nudeTextures[];
extern char* pubicHairFolder[];

extern int classRestrictions[12];
extern char* cutExceptionList[];
extern char * stockings[];
#endif // GLOBAL_H
