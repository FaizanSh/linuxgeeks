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
#include "list.h"

//#include "helpingFunctions.h"
void loadModule(char *module, char *name, int skipcount)
{
    // Allocates storage
    char *command = (char *)malloc(540 * sizeof(char));
    // Prints "Hello world!" on hello_world
    sprintf(command, "sudo insmod %s mycommand=%s myskipcount=%d", module, name, skipcount);

    system(command);

    free(command);
}

//#include "helpingFunctions.h"
void unloadModule(char *module)
{
    // Allocates storage
    char *command = (char *)malloc(140 * sizeof(char));
    // Prints "Hello world!" on hello_world
    sprintf(command, "sudo rmmod %s", module);

    system(command);
}

void detachAll()
{
    // char *command = (char *)malloc(540 * sizeof(char));
    //     sprintf(command, "ps -p %d -o pid,state,command", ptr->pid);
    //     system(command);

    struct node *ptr = head;
    pid_t pid;
    while (ptr != NULL)
    {
        pid = ptr->pid;

        ptrace(PTRACE_CONT, pid, NULL, NULL);
        kill(pid, SIGSTOP);
        waitpid(pid, NULL, 0);

        if (ptrace(PTRACE_DETACH, pid, NULL, NULL) != 0)
        {
            eprintf("\nptrace failed with request PTRACE_Detach:%s = [%d]\n", strerror(errno), pid);
            
        }
        else
        {
            printf("Detached from [%d] \n", pid);
        }

        kill(pid, SIGCONT);

        ptr = ptr->next;
    }
    //free(command);
}

void detach(pid_t pid)
{

    ptrace(PTRACE_CONT, pid, NULL, NULL);

    kill(pid, SIGSTOP);

    waitpid(pid, NULL, 0);

    if (ptrace(PTRACE_DETACH, pid, NULL, NULL) != 0)
    {
        eprintf("\nptrace failed with request PTRACE_Detach:%s = [%d]\n", strerror(errno), pid);
    }
    else
    {
        printf("Detached from [%d] \n", pid);
    }

    kill(pid, SIGCONT);
   // waitpid(pid,NULL,0); //very temporary
}

void unwindStack()
{

    //start from the beginning
    struct node *ptr = head;

    while (ptr != NULL)
    {
        printf("\n(%d,%d)\n ", ptr->key, ptr->pid);
        stackunwind(ptr->pid);
        ptr = ptr->next;
    }
}

void unwindpid(pid)
{
    stackunwind(pid);
}

// void sigint_handler(int signo)
// {
//     printf("\nReceived SIGINT\n");
//     //kill(getpid, SIGCONT);
//     if (ptrace(PTRACE_DETACH, pid, NULL, NULL)==0);
//         printf("Detached from [%d] \n",pid);
//     unloadModule("kprobefutexwakekill.ko");
//     exit(0);
// }

char *readString(pid_t child, unsigned long addr)
{

    //read_string takes a child to read from, and the address of the string
    //it's going to read.

    int allocated = 4096;
    char *val = malloc(allocated); //A buffer to copy the string into
    //    int allocated = 4096, read;
    int read = 0;          //counters of
                           //how much data we've copied and allocated
    unsigned long tmp = 0; //and a temporary variable for
                           //reading memory.
    errno = 0;             //it is check just to make error 0 and see what we get
                           //We need some variables. A buffer to copy the string into, counters of
                           //how much data we've copied and allocated, and a temporary variable for
                           //reading memory.

    while (1)
    {
        if (read + sizeof tmp > allocated)
        {
            allocated *= 2;
            val = realloc(val, allocated);
        }

        //We grow the buffer if necessary. We read data one word at a time.
        // eprintf( "\n Going to ptrace: %s \n", strerror( errno ) ) ;

        //this very line is generating input/output error
        tmp = ptrace(PTRACE_PEEKDATA, child, addr + read);
        //huhuhuhuhu

        if (errno != 0)
        {
            val[read] = 0;
            eprintf("\n Read String Failed due to: %s \n", strerror(errno));
            break;
        }

        //PTRACE_PEEKDATA returns a work of data from the child at the specified
        //offset. Because it uses its return for the value, we need to check
        //errno to tell if it failed. If it did (perhaps because the child
        //passed an invalid pointer), we just return the string we've got so
        //far, making sure to add our own NULL at the end.

        memcpy(val + read, &tmp, sizeof tmp);
        if (memchr(&tmp, 0, sizeof tmp) != NULL)
            break;
        read += sizeof tmp;

        //Then it's a simple matter of appending the data we read, and breaking
        //out if we found a terminating NULL, or else looping to read another
        //word.
    }
    return val;
}

/* *** eprintf ***
* Description: general error printing function
* https://www.tutorialspoint.com/c_standard_library/stdarg_h.htm
*/
//here fmt stand for format
//This is the C string that contains the text to be written to the stream.
//It can optionally contain embedded format tags that are replaced by the
//values specified in subsequent additional arguments and formatted as requested.
//https://www.tutorialspoint.com/c_standard_library/c_function_vfprintf.htm
void eprintf(char const *fmt, ...)
{
    va_list ap; //perhaps variable argument list
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

////
////
////
////int readString( pid_t pid, void *addr, char *string, size_t maxSi) {
////    int rc = 0 ;
////    long peekWord ;
////    char *peekAddr ;
////    int i ;
////    int stringIndex = 0 ;
////    char *tmpString ;
////    int stringFound = 0 ;
////    string[0] = '\0' ;
////    peekAddr = (char *) ((long)addr & ~(sizeof(long) - 1 ) ) ;
////// The PTRACE_PEEKDATA feature reads full words from the proce
////// address space.
////    peekWord = ptrace( PTRACE_PEEKDATA, pid, peekAddr, NULL ) ;
////    if ( -1 == peekWord ) {
////        perror( "ptrace( PTRACE_PEEKDATA..." ) ;
////        rc = -1 ;
////        goto exit ;
////    }
////    // Keep in mind that since peekAddr is aligned
////// it might contain a few characters at the beginning
////    //int charsToCopy = sizeof( long ) - ( (long)addr - long)peekAdd;
////    int charsToCopy = ( sizeof( long ) - ( (long)addr - long)peekAddr );
////    tmpString = (char *)&peekWord ;
////    tmpString += sizeof( long ) - charsToCopy ;
////    for ( i = 0 ; i < charsToCopy ; i++ ) {
////        string[ stringIndex ] = tmpString[ i ] ;
////        stringIndex++ ;
////        if ( maxSize - 1 == stringIndex ) {
////            string[ stringIndex ] = '\0';
////            goto exit ;
////        }
////    }
////    tmpString = (char *)&peekWord ;
////    peekAddr += sizeof( long) ;
////// Fall into a loop to find the end of the string
////    do
////    {
////        peekWord = ptrace( PTRACE_PEEKDATA, pid, peekAddr, NULL ) ;
////        if ( -1 == peekWord ) {
////            perror( "ptrace( PTRACE_PEEKDATA..." ) ;
////            rc = -1 ;
////            goto exit ;
////        }
////        for ( i = 0 ; i < sizeof(long) ; i++ ) {
////            string[ stringIndex ] = tmpString[ i ] ;
////            if ( maxSize - 1 == stringIndex ) {
////                string[ stringIndex ] = '\0';
////                goto exit ;
////            }
////            if ( string[ stringIndex ] == '\0' ) {
////                stringFound = 1 ;
////                break ;
////            }
////            stringIndex++ ;
////        }
////        peekAddr += sizeof( long) ;
////    } while ( !stringFound ) ;
////exit:
////    return rc ;
////}
////
////
