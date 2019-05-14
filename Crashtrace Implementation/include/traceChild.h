#ifndef TRACECHILD_H
#define TRACECHILD_H

#include <sys/types.h>//pid_t etc
int traceChildProcess( pid_t tracedPid,int argc, char *argv[] );
#endif
