#include <errno.h>
#include <gccore.h>
#include <malloc.h>
#include <network.h>
#include <ogc/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiiuse/wpad.h>

#include "menu.h"

static void* xfb = NULL;
GXRModeObj*  rmode;

void
cbPower ()
{
    SYS_ResetSystem (SYS_POWEROFF, 0, 0);
}

void
cbReset (u32 irq, void* ctx)
{
    exit (0);
}

void
cbHome ()
{
    Menu_Print ("Exit triggered, leaving application.", true);
}

void
init ()
{
    SYS_SetPowerCallback (cbPower);
    SYS_SetResetCallback (cbReset);

    if (PAD_Init() == PAD_ERR_TRANSFER)
    {
        Error_Throw ("GameCube Controller transfer error.");
    }
    if (WPAD_Init() < -2)
    {
        Error_Throw ("Wiimote error");
    }

    VIDEO_Init();

    rmode = VIDEO_GetPreferredMode (NULL);
    xfb   = MEM_K0_TO_K1 (SYS_AllocateFramebuffer (rmode));

    VIDEO_Configure (rmode);
    VIDEO_SetNextFramebuffer (xfb);
    VIDEO_SetBlack (false);
    VIDEO_Flush();

    CON_Init (xfb, 0, 0, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

    // Wait for Video setup to complete
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE)
        VIDEO_WaitVSync();

    Menu_InitStatic();

    MenuMan_Init();
}

int
proc ()
{
    while (true)
    {
        VIDEO_WaitVSync();

        PAD_ScanPads();
        u16 gcpressed = PAD_ButtonsDown (0);

        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown (0);

        if (pressed | gcpressed)
        {
            if ((pressed & WPAD_BUTTON_HOME) || (gcpressed & PAD_BUTTON_START))
            {
                cbHome();
                return (0);
            }

            if (MenuMan_Update (pressed, gcpressed) != true)
            {
                return (1);
            }
        }
    }
}

int
main (int argc, char** argv)
{
    init();

    return (proc());
}
