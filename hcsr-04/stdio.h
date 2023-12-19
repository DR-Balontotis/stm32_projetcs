#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdint.h>
#include <stdarg.h>

void printf_init(void (*_putc)(char), int (*_readc)(void));

int sprintf(char *buf, const char *fmt, ...);
void fprintf(const char *fmt, ...);

int fscanf(const char* format, ...);

#endif
