#include "menu.h"

#include <malloc.h>
#include <stdio.h>

#include "menu_main.h"
#include "menuman.h"
#include "util.h"

extern GXRModeObj* rmode;
extern u8          buttonSelIdx;
u8                 buttonNum   = 0;
bool               tried_reset = false;
u16                charNum_h   = 0;
u16                charNum_v   = 0;
char               strSeparator[128];

#define PRINT_SEP  printf ("%s", strSeparator)

#define PRINT_MT   puts ("")

#define CREDIT_DEV "2025, Moddimation."

#define CTRL_HELP                                                                              \
    " Up/Down = Move Around  |  A = Select  |  B = Go Back  |  Home/Start = Exit App"

void
Menu_InitStatic ()
{
    charNum_h   = (rmode->fbWidth / CHAR_WIDTH);
    charNum_v   = (rmode->efbHeight / (CHAR_WIDTH - 1)) - 3;

    // initialize separator
    u16 sepSize = 0;
    for (; sepSize < charNum_h; ++sepSize) { strSeparator[sepSize] = '='; }
}

void
Menu_Init (MenuEntry* menu)
{
    // flag all valid buttons as visible
    buttonNum = 0;
    for (; menu->options[buttonNum]->label.text[0] != '\0' ||
           menu->options[buttonNum]->action.ptr != NULL;
         ++buttonNum)
    {
        menu->options[buttonNum]->flags |= BUTTONFLAG_VISIBLE;
    }

    Menu_Clear();

    if (menu->start != NULL)
        menu->start (menu);
}

inline void
Menu_StartPos ()
{
    puts ("\x1b[1;0H");
}

inline void
Menu_Clear ()
{
    for (u16 j = charNum_h; j > 0; --j)
    {
        for (u16 i = 0; i < charNum_v; ++i) { printf (" "); }
    }
    Menu_StartPos();
}

bool
Menu_Update (MenuEntry* menu)
{
    int w, h;

    Menu_StartPos();

    if (!menu->active)
    {
        if (!tried_reset)
        {
            tried_reset = true;
            MenuMan_SetMenu (&gMenu_Main);
            return true;
        }
        else
        {
            Error ("Unable to reset to main menu after menu was unactive, internal error");
            return false;
        }
    }
    else
    {
        tried_reset = false;
    }

    if (buttonSelIdx >= buttonNum)
    {
        buttonSelIdx = buttonSelIdx == 100 ? buttonNum - 1 : 0;
    }

    w = rmode->fbWidth;
    h = rmode->efbHeight;
    (void)w;
    (void)h;

    printf (DEF_APP_NAME ": %s", menu->longTitle.text);

    Util_StrSetX (Util_StrCalcXRightAligned (charNum_h, CREDIT_DEV));
    printf (CREDIT_DEV);

    PRINT_SEP;
    PRINT_MT;

    if (menu->preupdate != NULL)
        menu->preupdate (menu);

    for (u8 buttonIdx = 0; buttonIdx <= MAX_BUTTONS; ++buttonIdx)
    {
        char str[MAX_LABEL];

        if ((menu->options[buttonIdx]->flags & BUTTONFLAG_VISIBLE) == false)
        {
            continue;
        }

        snprintf (str,
                  sizeof (str),
                  "   %c %s",
                  (buttonSelIdx == buttonIdx ? '>' : ' '),
                  menu->options[buttonIdx]->label.text);
        puts (str);
    }

    if (menu->postupdate != NULL)
        menu->postupdate (menu);

    Util_StrSetY (Util_StrCalcYBottomAligned (charNum_v));
    PRINT_SEP;
    printf (CTRL_HELP);

    return true;
}

void
Menu_PrintEx (const char* str, const char* header, bool clear)
{
    if (clear)
    {
        Menu_Clear();
    }
    else
    {
        Menu_StartPos();
    }

    if (header == NULL)
    {
        printf (DEF_APP_NAME);
    }
    else
    {
        printf (DEF_APP_NAME ": %s", header);
    }

    Util_StrSetX (Util_StrCalcXRightAligned (charNum_h, CREDIT_DEV));
    printf (CREDIT_DEV);

    PRINT_SEP;
    PRINT_MT;

    puts (str);
}

void
Menu_Print (const char* str, bool clear)
{
    Menu_PrintEx (strdup (str), NULL, clear);
}

