#include "menu_test.h"

#include "menu/test/menu_test_listtitles.h"

static void cbDoListTitles ();

const MenuEntry gMenu_Test = Menu (Label ("Test Menu"),
                                   Label ("test"),
                                   NULL,
                                   Button ((Label ("List all title ID's")), cbDoListTitles));

static void
cbDoListTitles ()
{
    MenuMan_SetMenu (&gMenu_Test_ListTitles);
}
