#include "helpingFunctions.h"
#include "spawnChild.h"
#include "traceChild.h"
#include "list.h"
#include "stacktrace.h"

#include <asm/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
//he stdarg.h header defines a variable type va_list and three macros which can be used to get the arguments in a function when the number of arguments are not known i.e. variable number of arguments.
#include <errno.h>
//The <errno.h> header file defines the integer variable errno, which is set by system calls and some library functions in the event of an error to indicate what went wrong.
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <libunwind-ptrace.h>

int main(int argc, char *argv[], char *envp[])
{

    if (strcmp(argv[1], "-help") == 0)
    {
        fprintf(stderr, "\nusage: crashtrace [-p] <pid> -o Filename [-k] mycommand myskipcount\n");
        fprintf(stderr, "or: crashtrace [program path] -x -o Filename [-k] mycommand myskipcount\n");
        fprintf(stderr, "Tags Discription:\n");
        fprintf(stderr, "-p followed by a pid of the process that is to be attached\n");
        fprintf(stderr, "OR\n");
        fprintf(stderr, "full path of the program that is to be tested\n");
        fprintf(stderr, "-o file        send trace output to FILE\n");
        fprintf(stderr, "-x to print stacktrace\n");
        fprintf(stderr, "-k followed by name of the program to focus and skipcounts to skip futex kills\n");
        return 0;
        exit(0);
    }

    if (argc >= 5)
    {
        if (strcmp(argv[5] ,"-k") == 0)
            loadModule("kprobefutexwakekill.ko", argv[6], atoi(argv[7]));
    }
    int mRC = 0; // Return code for this function 'main return code'
    pid_t cpid;  // Child process ID
    cpid = fork();

    if (cpid > 0)
    {
        // Parent
        //fprintf(stderr, "hahahaha parent\n");
        //traceChildProcess( cpid ) ; //sending -1 because we need to wait for every child I guess
        traceChildProcess(-1, argc, argv); //sending -1 because we need to wait for every child I guess

        // fprintf(stderr, "hahahaha parent\n");
    }
    else if (0 == cpid)
    {
        // Child
        // fprintf(stderr, "hahahahah child\n");
        spawnChildProcess(argc, &argv[1], envp);
    }
    else
    {
        fprintf(stderr, "Could not fork child (%s)\n", strerror(errno));
        mRC = 1;
    }

    return mRC;
}
