#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <switch.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void log_printf(const char* format, ...);

#ifdef __cplusplus
}
#endif