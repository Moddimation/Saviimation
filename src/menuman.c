#include "menuman.h"

#include <string.h>
#include <wiiuse/wpad.h>

#include "menu.h"
#include "menu/menu_main.h"

u8                buttonSelIdx = 0;
static MenuEntry* menuHistory[1024];
static u32        menuHistoryIdx = 0;
static MenuEntry  menu           = {};

void
MenuMan_Init ()
{
    (void)buttonSelIdx;
    MenuMan_SetMenu (&gMenu_Main);
    Menu_Update (&menu);
}

void
handleInput (u32 down_wii, u32 down_gc)
{
#define PRESSED(wii, wii_classic, gc)    /* wiimote, classic, gamecube */                      \
    ((down_wii & WPAD_BUTTON_##wii) || (down_wii & WPAD_CLASSIC_BUTTON_##wii_classic) ||       \
     (down_gc & PAD_BUTTON_##gc))

    if (PRESSED (DOWN, DOWN, DOWN))
    {
        buttonSelIdx++;
    }
    if (PRESSED (UP, UP, UP))
    {
        buttonSelIdx = buttonSelIdx == 0 ? 100 : buttonSelIdx - 1;
    }
    if (PRESSED (A, A, A))
    {
        if (menu.options[buttonSelIdx].action.ptr != NULL)
        {
            (menu.options[buttonSelIdx].action.ptr)();
        }
    }
    if (PRESSED (B, B, B))
    {
        if (!menu.isBase)
        {
            menuHistoryIdx -= 2;
            if (menuHistoryIdx < 0)
                menuHistoryIdx = 1024 - (0 - menuHistoryIdx);

            MenuMan_SetMenu (menuHistory[menuHistoryIdx]);
        }
    }
}

bool
MenuMan_Update (u32 down_wii, u32 down_gc)
{
    handleInput (down_wii, down_gc);
    return Menu_Update (&menu);
}

void
freeMenu ()
{
    if (menu->options != NULL)
    {
        for (u32 i = 0; i < menu->optionsNum; ++i)
        {
            if (menu->options[i] != NULL)
                free menu->options[i];
        }
    }
}

bool
MenuMan_SetMenu (MenuEntry* m)
{
    if (m->init != NULL)
        m->init (m);

    MenuEntry* __prevMenu = menu.basePtr;
    char       __path[MAX_PATH];
    strcpy (__path, menu.path.text);
    strcat (__path, "/");

    memcpy (&menu, m, sizeof (menu));
    if (menu.shortTitle.text[0] == '\0')
    {
        Error ("Error copying menu constants");
        return false;
    }

    menu->options = malloc (sizeog (MenuButton*) * m->optionNum);
    for (u32 i = 0; i < menu->optionsNum; ++i)
    {
        menu->options[i] = malloc (sizeof (MenuButton*));
        MenuButton* b    = menu->options[i];
        b->flags         = m->options[i] // just continue this.
    }

    strcat (__path, menu.shortTitle.text);
    strcpy (menu.path.text, __path);

    menu.basePtr                = m;
    menu.prevId                 = (__prevMenu == NULL) ? 0 : __prevMenu->id;

    menu.active                 = true;

    menuHistory[menuHistoryIdx] = m;
    menuHistoryIdx              = menuHistoryIdx >= 1024 ? 0 : menuHistoryIdx + 1;

    menu.id                     = m->id;
    menu.isBase                 = m->isBase;

    Menu_Init (&menu);

    return true;
}
