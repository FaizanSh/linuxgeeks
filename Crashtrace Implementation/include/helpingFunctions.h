#ifndef HELPINGFUNCTIONS_H
#define HELPINGFUNCTIONS_H

#include <sys/types.h> //pid_t etc
void loadModule(char *module, char *name, int skipcount);
void unloadModule(char *module);
void detachAll();
void unwindStack();
void sigint_handler(int signo);

void eprintf(char const *fmt, ...);
char *readString(pid_t child, unsigned long addr);
#endif
