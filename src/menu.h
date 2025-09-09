#ifndef __MENU_H__
#define __MENU_H__

#include <gccore.h>

#include "menuman.h"

#define MAX_PATH    256
#define MAX_LABEL   512
#define MAX_BUTTONS 16

typedef enum
{
    BUTTONFLAG_NIL     = (0 << 0),
    BUTTONFLAG_VISIBLE = (1 << 1),
    BUTTONFLAG_ACTIVE  = (1 << 2),
} MenuButtonFlags;

typedef struct
{
    char text[MAX_LABEL];
    int  color;
} MenuLabel;

#define LabelC(text, color)                                                                    \
    (MenuLabel)                                                                                \
    {                                                                                          \
        text, color                                                                            \
    }
#define Label(text) LabelC (text, 0xFFFFFFFF)
#define LabelE      Label ("")

typedef struct
{
    void (*ptr)();
} MenuButtonAction;

typedef struct
{
    MenuButtonAction action;
    MenuLabel        label;
    u8               flags;
} MenuButton;

#define ButtonF(label, action, flags)                                                          \
    (MenuButton)                                                                               \
    {                                                                                          \
        (MenuButtonAction) { action }, label, flags                                            \
    }
#define Button(label, action)  ButtonF (label, action, BUTTONFLAG_NIL)
#define ButtonE(label)         Button (label, NULL)
#define ButtonEE               ButtonE (LabelE)
#define ButtonEF(label, flags) ButtonF (label, NULL, flags)
#define Buttons(...)           { __VA_ARGS__ }

typedef struct MenuEntry MenuEntry;

struct MenuEntry
{
    bool       active;
    bool       isBase;
    MenuLabel  longTitle;
    MenuLabel  shortTitle;
    MenuLabel  path;   // set when called
    u32        id;     // set when called, from order of calling
    u32        prevId; // set when called, previous menuId
    MenuEntry* basePtr;
    void       (*init) (MenuEntry* menu);
    void       (*start) (MenuEntry* menu);
    void       (*preupdate) (MenuEntry* menu);
    void       (*postupdate) (MenuEntry* menu);
    MenuButton options[MAX_BUTTONS];
};

#define Menu(longT, shortT, funcInit, funcStart, funcPreUpdate, funcPostUpdate, ...)           \
    (MenuEntry)                                                                                \
    {                                                                                          \
        false, false, longT, shortT, LabelE, 0, 0, NULL, funcInit, funcStart, funcPreUpdate,   \
            funcPostUpdate, Buttons (__VA_ARGS__)                                              \
    }

void Menu_InitStatic ();
void Menu_Init (MenuEntry* menu);
bool Menu_Update (MenuEntry* menu);
void Menu_StartPos ();
void Menu_Clear ();
void Menu_Print (const char* str, bool clear);
void Menu_PrintEx (const char* str, const char* header, bool clear);

#endif

