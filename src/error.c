#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>

#include "menu.h"

void
Error_Throw (const char* fmt, ...)
{
    va_list args;

    char str[4096];

    va_start (args, fmt);
    vsnprintf (str, sizeof (str), fmt, args);
    va_end (args);

    strlcat (str, "\nCannot continue, press HOME/START to exit.", sizeof (str));

    Menu_PrintEx (str, "FATAL ERROR OCCURED!", true);

    while (true)
    {
        VIDEO_WaitVSync();

        PAD_ScanPads();
        WPAD_ScanPads();
        u32 pressed   = WPAD_ButtonsDown (0);
        u16 gcpressed = PAD_ButtonsDown (0);

        if ((pressed & WPAD_BUTTON_HOME) || (gcpressed & PAD_BUTTON_START))
        {
            exit (1);
        }
    }
}
