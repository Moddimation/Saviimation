#include "menu_test_listtitles.h"

#include <stdio.h>


static void menu_test_listtitles_update (MenuEntry* menu);

MenuEntry gMenu_Test_ListTitles = Menu (Label ("Test - List Titles"),
                                              Label ("test_listtitle"), NULL, NULL, NULL,
                                              menu_test_listtitles_update);


static void menu_test_listtitles_update (MenuEntry* menu) {
    puts("hi");
}
