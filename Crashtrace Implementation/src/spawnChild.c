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
//#include <string.h>
//#include <stdlib.h>
//#include <stddef.h>
////#include <errno.h>
//#include <sys/ptrace.h>
//#include <sys/signal.h>

//#include "spawnChild.h"
//#include "helpingFunctions.h"

int spawnChildProcess(int argc, char *argv[], char *envp[])
{
    fprintf(stderr, "I am in child\n");
    int mRC = 0; // Return code for this function
    int sRC = 0; // Return code for system calls

    char *args[argc + 1];
    // execvp needs null terminated array thats why we are coppying this into new one and adding null in the end
    memcpy(args, argv, argc * sizeof(char *)); // mid * is multiply size of char* will give 8 for 64 bit and which means 8 bit for each address and 4 for 32 bit means 4 bit for each address
    args[argc] = NULL;
    //fprintf( stderr, "What do we have 0:%s 1:%s 2:%s\n",args[0],args[1],args[2]);
    //fprintf( stderr, "Entering Ptrace Attach\n" );
    if (strcmp(args[0], "-p") == 0)
    {
        goto exit;
        // fprintf(stderr, "Entered ptrace_Attach\n");
        // sRC = ptrace(PTRACE_ATTACH, atoi(args[1]), 0, 0);
        // fprintf(stderr, "Reward:sRC is %d \n", sRC);
        // //ptrace(PTRACE_TRACEME) sets things up so that the parent is the tracer. For any given tracee, only one process at a time can be its tra

        // if (-1 == sRC)
        // {
        //     eprintf("ptrace failed with request \"PTRACE_TRACEME\": %s\n", strerror(errno));
        //     fprintf(stderr, "Ops cant ptrace_Attach\n");
        //     sRC = errno;
        //     goto exit;
        // }

        // //lets send signal to it self which further will be recieved by the parent and parent will get the control
        // fprintf(stderr, "Gotta go ptrace_Attach\n");
        // fprintf(stderr, "Sending SIGSTOP signal\n");
        // kill(atoi(args[1]), SIGSTOP);
        // fprintf(stderr, "Going to exit\n");
        // goto exit;
    }

    //its a siren I just called that notifies the parent to just keep an eye on me
    //This tells the kernel that the process is being traced
    sRC = ptrace(PTRACE_TRACEME, 0, 0, 0);
    //ptrace(PTRACE_TRACEME) sets things up so that the parent is the tracer. For any given tracee, only one process at a time can be its tra
    if (-1 == sRC)
    {
        eprintf("ptrace failed with request \"PTRACE_TRACEME\": %s\n", strerror(errno));
        sRC = errno;
        goto exit;
    }

    //lets send signal to it self which further will be recieved by the parent and parent will get the control
    kill(getpid(), SIGSTOP);

    printf("child to be %s", argv[0]); //lets check what we got
    //and when the child executes the execve system call, it hands over control to its parent.
    sRC = execve(args[0], args, envp); // execvp needs null terminated array thats why we are coppying this into new one
    if (-1 == sRC)
    {
        eprintf("exec failed: %s\n", strerror(errno));
        sRC = errno;
        goto exit;
    }
exit:
    return mRC;
}
