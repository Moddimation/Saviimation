#include "menuman.h"

#include <string.h>
#include <wiiuse/wpad.h>

#include "menu.h"
#include "menu/menu_main.h"

u8               buttonSelIdx = 0;
static MenuEntry menu         = {};

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
#define PRESSED(wii, wii_classic, gc) /* wiimote, classic, gamecube */                         \
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
        if (menu.prevPtr != NULL)
        {
            MenuMan_SetMenu (menu.prevPtr);
        }
    }
}

bool
MenuMan_Update (u32 down_wii, u32 down_gc)
{
    handleInput (down_wii, down_gc);
    return Menu_Update (&menu);
}

bool
MenuMan_SetMenu (const MenuEntry* m)
{
    const MenuEntry* __prevMenu = menu.basePtr;
    char             __path[MAX_PATH];
    strcpy (__path, menu.path.text);
    strcat (__path, "/");

    memcpy (&menu, m, sizeof (menu));
    if (menu.shortTitle.text[0] == '\0')
    {
        Error ("Error copying menu constants");
        return false;
    }

    strcat (__path, menu.shortTitle.text);
    strcpy (menu.path.text, __path);

    menu.basePtr = m;
    menu.prevPtr = __prevMenu;

    menu.active  = true;

    Menu_Init (&menu);

    return true;
}
