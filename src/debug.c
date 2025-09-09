#include "debug.h"

#include <ctype.h>
#include <stdio.h>
#include <wiiuse/wpad.h>

void
Debug_HexDump (const void* data, size_t size)
{
    const unsigned char* bytes = (const unsigned char*)data;
    size_t               i     = 0;

    while (i < size)
    {
        // Wait for A to be held
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsHeld (0);
        if (!(pressed & WPAD_BUTTON_A))
            continue;

        // Print up to 10 lines per press
        int linesPrinted = 0;
        while (linesPrinted < 10 && i < size)
        {
            printf ("%04X  ", (unsigned int)i);

            // Hex part
            for (size_t j = 0; j < 16; ++j)
            {
                if (i + j < size)
                    printf ("%02X", bytes[i + j]);
            }

            printf ("  ");

            // ASCII part
            for (size_t j = 0; j < 16; ++j)
            {
                if (i + j < size)
                {
                    unsigned char c = bytes[i + j];
                    printf ("%c", isprint (c) ? c : '.');
                }
            }

            printf ("\n");
            i += 16;
            ++linesPrinted;
        }

        // Wait until A is released
        do {
            WPAD_ScanPads();
            pressed = WPAD_ButtonsHeld (0);
        }
        while (pressed & WPAD_BUTTON_A);
    }
}

