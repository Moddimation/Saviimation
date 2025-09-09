#include "menu_test_listtitles.h"

#include <stdio.h>


static void menu_test_listtitles_init (MenuEntry* menu);

const MenuEntry gMenu_Test_ListTitles = Menu (Label ("Test - List Titles"),
                                              Label ("test_listtitle"),
                                              menu_test_listtitles_init);


static void menu_test_listtitles_init (MenuEntry* menu) {
    puts("hi");
}
