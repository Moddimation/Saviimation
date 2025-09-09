#include "menu_test_listtitles.h"

#include <stdio.h>
#include <ogc/es.h>
#include <malloc.h>


static void menu_test_listtitles_update (MenuEntry* menu);

MenuEntry gMenu_Test_ListTitles = Menu (Label ("Test - List Titles"),
                                              Label ("test_listtitle"), NULL, NULL, NULL,
                                              menu_test_listtitles_update);


static void menu_test_listtitles_update(MenuEntry* menu) {
    u32 titleCnt = 0;
    if(ES_GetNumTitles(&titleCnt) < 0) return;

    u64* titles = malloc(sizeof(u64) * titleCnt);
    if(!titles) return;

    if(ES_GetTitles(titles, titleCnt) < 0) {
        free(titles);
        return;
    }

    for(u32 i = 0; i < titleCnt; ++i) {
        printf("%016llX\n", titles[i]);
    }

    free(titles);
}
