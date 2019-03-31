#ifndef HELPINGFUNCTIONS_H
#define HELPINGFUNCTIONS_H

#include <sys/types.h>//pid_t etc
void eprintf( char const *fmt, ... );
char *readString(pid_t child, unsigned long addr);
#endif
