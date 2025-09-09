#ifndef __MENUMAN_H__
#define __MENUMAN_H__

#include <gccore.h>

typedef struct MenuEntry MenuEntry;

void MenuMan_Init ();
bool MenuMan_Update (u32 down_wii, u32 down_gc);
bool MenuMan_SetMenu (const MenuEntry* m);

#endif
