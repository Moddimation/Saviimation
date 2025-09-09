#ifndef __CONSOLE_H__
#define __CONSOLE_H__

void Ui_ResizeBuffer (u32 size);
void Ui_ClearBuffer ();
void Ui_Update ();

void Ui_Printf (const char*, ...);
void Ui_Print (const char*);
void Ui_Printc (const char);

#endif
