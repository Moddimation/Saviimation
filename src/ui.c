#include "ui.h"

#include <malloc.h>
#include <stdio.h>

static const char** str_buf = NULL;
static u32          str_max;
static u32          str_num;

void
Ui_ResizeBuffer (u32 size)
{
    Ui_ClearBuffer();

    str_buf = malloc (size * sizeof (char*));
    str_max = size;
}

void
Ui_ClearBuffer ()
{
    if (str_buf)
    {
        for (u32 i = 0; i < str_max; ++i)
        {
            if (str_buf[i] != NULL)
                free ((void*)str_buf[i]);
        }
        free ((void*)str_buf);
    }

    str_num = 0;
    str_max = 0;
}

void
Ui_Update ()
{}

void
Ui_Printf (const char*, ...)
{}

void
Ui_Print (const char*)
{}

void
Ui_Printc (const char)
{}
