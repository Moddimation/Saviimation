#include <gccore.h>
#include <stdarg.h>

#ifndef __ERROR_H__
#define __ERROR_H__

void Error_Throw (const char*, ...);

#define Error(fmt, ...) Error_Throw ("Error occured at %s() : " fmt, __func__, ##__VA_ARGS__)

#endif
