#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <string.h>

#define CHAR_WIDTH 8

u32 static inline Util_StrGetWidth (const char* str)
{
    return (u32)(strlen (str) * CHAR_WIDTH);
}

u32 static inline Util_StrCalcXRightAligned (int fb_width, const char* str)
{
    return fb_width - strlen (str);
}

u32 static inline Util_StrCalcYBottomAligned (int fb_height)
{
    return fb_height - 32;
}

void static inline Util_StrSetX (u32 x)
{
    printf ("\r\x1b[%iC", x);
}

void static inline Util_StrSetY (u32 y)
{
    printf ("\r\x1b[%d;%dH", y, 0);
}

#endif
