#include "helpingFunctions.h"
#include "spawnChild.h"
#include "traceChild.h"
#include "list.h"

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


int main( int argc, char *argv[] ,char *envp[]) {

    int mRC = 0 ; // Return code for this function 'main return code'
    pid_t cpid ; // Child process ID
    cpid = fork() ;

    if ( cpid > 0 ) {
// Parent
        traceChildProcess( cpid ) ; //sending -1 because we need to wait for every child I guess
    } else if ( 0 == cpid ) {
// Child
        spawnChildProcess( argc, &argv[1], envp ) ;
    } else {
        fprintf( stderr, "Could not fork child (%s)\n", strerror(
                     errno ) ) ;
        mRC = 1 ;
    }
    return mRC ;
}
