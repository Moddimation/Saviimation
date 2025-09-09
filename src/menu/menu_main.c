#include "menu_main.h"

#include "menu_test.h"

static void cbTest ();
static void cbMenWii ();
static void cbMenSD ();
static void menu_main_init (MenuEntry*);

MenuEntry gMenu_Main = Menu (Label ("Main Menu"),
                             Label ("main"),
                             menu_main_init,
                             NULL,
                             NULL,
                             NULL,
                             Button ((Label ("Test Menu")), cbTest),
                             Button ((Label ("Wii Saves")), cbMenWii),
                             Button ((Label ("Backups")), cbMenSD));

static void
cbTest ()
{
    MenuMan_SetMenu (&gMenu_Test);
}

static void
cbMenWii ()
{}

static void
cbMenSD ()
{}

static void
menu_main_init (MenuEntry* m)
{
    m->isBase = true;
}
