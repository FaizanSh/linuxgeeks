//4April
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
//#include "traceChild.h"
#include "data.h"
#include "list.h"



//////////////////////////////////////
//#include <cassert>
//#include <cstdio>
//#include <cstdlib>
//#include <ctime>
#include <sys/reg.h>
//#include <array>
//#include <thread>
//#include <mutex>
//////////////////////////////////////////
const char *name_of_signal_number(int num);

int traceChildProcess( pid_t tracedPid )
{
    fprintf( stderr, "I am in Parent\n" );
    struct node *pidlist =  (struct node*) malloc(sizeof(struct node));
    int mRC = 0 ; // Return code for this function
    int sRC = 0 ; // Return code for system calls
    int status = 0 ; // Status of the stopped child process
    pid_t stoppedPid = 0 ; // Process ID of stopped child process
    struct user_regs_struct registers; //user_regs_struct contains all the system registes as struct
    //dated31March
    unsigned int ptrace_setoptions = 0;
    ptrace_setoptions = PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACEEXEC
				 | PTRACE_O_TRACEEXIT;


    //printf(" \n\n Stopped_PID %d || tracedPid %d \n\n", stoppedPid, tracedPid);

    do
        stoppedPid = waitpid(tracedPid, &status, 0);
    while (!WIFSTOPPED(status));
    fprintf(stderr, "parent: initial stop observed\n");

    if ( -1 == stoppedPid )
    {
        eprintf( "waitpid failed: %s\n", strerror( errno ) ) ;
        mRC = 1 ;
        goto exit ;
    }
    insertFirst(stoppedPid,stoppedPid);

    //stoppedPid = waitpid(tracedPid, &status, 0); // wait for any responce from the child most probably
    //when get sigstop signal because child was said ptracetraceme and in that method sigstop was sent
    //to itself afterthat
    //ptrace(PTRACE_SETOPTIONS,stoppedPid,NULL,PTRACE_O_TRACEVFORK);
   ptrace_setoptions |= PTRACE_O_TRACECLONE | PTRACE_O_TRACEFORK | PTRACE_O_TRACEVFORK;
    fprintf(stderr, "parent: setting ptrace options...\n");
    ptrace(PTRACE_SETOPTIONS,stoppedPid,NULL,ptrace_setoptions); //set options VFORK is jumbbo
    fprintf(stderr, "Options Set\n");

    // (1) Syscall-enter-stop and syscall-exit-stop are observed by the
    // tracer as waitpid(2) returning with WIFSTOPPED(status) true, and-
    // if the PTRACE_O_TRACESYSGOOD option was set by the tracer- then
    // WSTOPSIG(status) will give the value (SIGTRAP | 0x80).


    // Send a SIGKILL signal to the tracee if the tracer exits. This option is
    // useful for ptrace jailers that want to ensure that tracees can never escape
    // the tracer's control.


    // Stop the tracee at the next clone(2) and automatically start tracing the
    // newly cloned process, which will start with a SIGSTOP, or PTRACE_EVENT_STOP
    // if PTRACE_SEIZE was used.  A waitpid(2) by the tracer will return a status
    // value such that
    //
    //  status>>8 == (SIGTRAP | (PTRACE_EVENT_CLONE<<8))
    //
    // The PID of the new process can be retrieved with PTRACE_GETEVENTMSG. This
    // option may not catch clone(2) calls in all cases.  If the tracee calls
    // clone(2) with the CLONE_VFORK flag, PTRACE_EVENT_VFORK will be delivered
    // instead if PTRACE_O_TRACEVFORK is set; otherwise if the tracee calls
    // clone(2) with the exit signal set to SIGCHLD, PTRACE_EVENT_FORK will be
    // delivered if PTRACE_O_TRACEFORK is set.
    //ptrace(PTRACE_SETOPTIONS,stoppedPid,NULL,PTRACE_O_EXITKILL | PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACECLONE | PTRACE_EVENT_VFORK);
    fprintf(stderr, "ptrace options set!\n");

//v1.3

    //printf("\n\n Stopped_PID %d || tracedPid %d \n\n", stoppedPid, tracedPid);



    //v1.2


// Tell the child to stop in a system call entry or exit don't care that if it was being already traced but this this means stop on every-NEXT entry and exit point
    ptrace( PTRACE_SYSCALL, stoppedPid, NULL, NULL); //March31

// This is the main tracing loop. When the child stops,
// we examine the system call and its arguments
//If wait() or waitpid() returns due to the delivery of a signal to the calling process,-1 shall be returned and errno set to [EINTR]
    pid_t newpid;
    int inchild=0;
    int get=0;
    siginfo_t si;
    intptr_t sig = 0;
    pid_t child_waited = stoppedPid;
    pid_t npid= stoppedPid;
    //ptrace(PTRACE_CONT, tracedPid, NULL, NULL);
    for (;;)
    {
        child_waited = waitpid(-1, &status,__WCLONE | __WALL);//if I am in forked thread then do I get to recieve signal from this forked thread?
                                                    // And in return get the pid of that thread?
        if (child_waited == -1)
        {
            fprintf(stderr, "parent: waitpid(1) failed : %s\n",
                    strerror(errno));
            return -1;
        }

        else
        {
            if (WIFSTOPPED(status) && (WSTOPSIG(status)))
            {
                //
                // the following kinds of ptrace-stops exist:
                //
                //   (1) syscall-stops
                //   (2) PTRACE_EVENT stops
                //   (3) group-stops
                //   (4) signal-delivery-stops
                //
                // they all are reported by waitpid(2) with WIFSTOPPED(status) true.
                // They may be differentiated by examining the value status>>8, and if
                // there is ambiguity in that value, by querying PTRACE_GETSIGINFO.
                // (Note: the WSTOPSIG(status) macro can't be used to perform this
                // examination, because it returns the value (status>>8) & 0xff.)
                //
                pid_t pastpid = child_waited;

                const int stopsig = WSTOPSIG(status);

                if (stopsig == SIGTRAP)
                {
                    //
                    // PTRACE_EVENT stops (2) are observed by the tracer as waitpid(2)
                    // returning with WIFSTOPPED(status), and WSTOPSIG(status) returns
                    // SIGTRAP.
                    //
                    const unsigned int event = (unsigned int)status >> 16;


                    switch (event)
                    {
                    case 0:
                    if (stopsig == (SIGTRAP | 0x80))
                        {
                             fprintf(stderr,
                                "parent: ptrace event (case 0) [%d]\n",
                                child_waited);
                                 ptrace( PTRACE_SYSCALL, child_waited, NULL, NULL);

                    //
                    // (1) Syscall-enter-stop and syscall-exit-stop are observed by the
                    // tracer as waitpid(2) returning with WIFSTOPPED(status) true, and-
                    // if the PTRACE_O_TRACESYSGOOD option was set by the tracer- then
                    // WSTOPSIG(status) will give the value (SIGTRAP | 0x80).
                    //
                    //return stoppedPid;

                    //goto exit ;
                      }
                    break;

                    case PTRACE_EVENT_VFORK:
                        //fprintf(stderr, "parent: ptrace event (PTRACE_EVENT_VFORK)\n");
                        ptrace(PTRACE_GETEVENTMSG,child_waited,NULL, (long) &newpid);
                        insertLast(newpid,newpid);
                        ptrace(PTRACE_SETOPTIONS,newpid,NULL,ptrace_setoptions);
                        ptrace(PTRACE_SYSCALL,newpid,NULL,NULL);
                        //ptrace(PTRACE_CONT, newpid, NULL, NULL);
                        fprintf(stderr,
                        "parent: ptrace event (PTRACE_EVENT_vFORK) [%d]\n",
                        newpid);
                        ptrace( PTRACE_SYSCALL, child_waited, NULL, NULL); //sir
                        inchild = 1;
                        //child_waited = newpid;


                        break;
                    case PTRACE_EVENT_FORK:
//                        fprintf(stderr, "parent: ptrace event (PTRACE_EVENT_FORK)\n");
                        ptrace(PTRACE_GETEVENTMSG,child_waited,NULL, (long) &newpid);
                        insertLast(newpid,newpid);
                        ptrace(PTRACE_SETOPTIONS,newpid,NULL,ptrace_setoptions);
                        ptrace(PTRACE_SYSCALL,newpid,NULL,NULL);
                        //ptrace(PTRACE_CONT, newpid, NULL, NULL);
                        fprintf(stderr,
                        "parent: ptrace event (PTRACE_EVENT_FORK) [%d]\n",
                        newpid);
                        ptrace( PTRACE_SYSCALL, child_waited, NULL, NULL); //sir
                        inchild = 1;


//                printf("Attached to offspring %ld\n",(pid_t)newpid);
                        break;
                    case PTRACE_EVENT_CLONE:
                        ptrace(PTRACE_GETEVENTMSG,child_waited,NULL, (long) &newpid);
                        insertLast(newpid,newpid);
                        ptrace(PTRACE_SETOPTIONS,newpid,NULL,ptrace_setoptions);
                        ptrace(PTRACE_SYSCALL,newpid,NULL,NULL);
                        //ptrace(PTRACE_CONT, newpid, NULL, NULL);
                        fprintf(stderr,
                        "parent: ptrace event (PTRACE_EVENT_CLONE) [%d]\n",
                        newpid);
                        ptrace( PTRACE_SYSCALL, child_waited, NULL, NULL); //sir
                        inchild = 1;
                        //child_waited = newpid;

                        break;
                    case PTRACE_EVENT_VFORK_DONE:
                        fprintf(stderr, "parent: ptrace event (PTRACE_EVENT_VFORK_DONE)\n");
                        break;
                    case PTRACE_EVENT_EXEC:
                        fprintf(stderr, "parent: ptrace event (PTRACE_EVENT_EXEC)\n");
                        break;
                    case PTRACE_EVENT_EXIT:
                        fprintf(stderr, "parent: ptrace event (PTRACE_EVENT_EXIT)\n");
                        break;
                    case PTRACE_EVENT_STOP:
                        fprintf(stderr, "parent: ptrace event (PTRACE_EVENT_STOP)\n");
                        break;
                    case PTRACE_EVENT_SECCOMP:
                        fprintf(stderr, "parent: ptrace event (PTRACE_EVENT_SECCOMP)\n");
                        break;
                    default:
//                        fprintf(stderr, "parent: unknown ptrace event %u\n", event);
                        break;
                    }
                }
                else if (ptrace(PTRACE_GETSIGINFO, child_waited, 0, &si) < 0)
                {
                    //
                    // (3) group-stop
                    //
                    fprintf(stderr, "parent: group-stop [%s]\n",
                            name_of_signal_number(stopsig));

                    // When restarting a tracee from a ptrace-stop other than
                    // signal-delivery-stop, recommended practice is to always pass 0 in
                    // sig.
                }
                else
                {
                    //
                    // (4) signal-delivery-stop
                    //commented 31March
                    //
//                    fprintf(stderr, "parent: signal-delivery-stop [%s]\n",
//                            name_of_signal_number(stopsig));

                    // deliver it
                    sig = stopsig;
                }
            }
            else
            {
                //
                // the child process terminated
                //

                fprintf(stderr, "parent: [%d] terminated [%d]\n",head->pid,last->pid);
                deletepid(child_waited);
                displayForward();
                if(isEmpty()){
                goto exit;
                }
//              else{
//              child_waited = stoppedPid;
//              }


//                child_waited = stoppedPid; //look at this jugaar //why it isnt getting the pid of parent :(
            }
        }


//
//        if(status >> 16 == PTRACE_EVENT_FORK) {
//            //ptrace(PTRACE_ATTACH,(pid_t)newpid,NULL,NULL);
//            ptrace(PTRACE_GETEVENTMSG,stoppedPid,NULL, (long) &newpid);
//            ptrace(PTRACE_SYSCALL,newpid,NULL,NULL);
//            printf("Attached to offspring %ld\n",(pid_t)newpid);
//            inchild = 1;
//
//        }
//        else if(WIFEXITED(status) || WIFSIGNALED(status) || WIFSTOPPED(status)) {
//
//            inchild = 0;
//        }

//        if(WIFEXITED(status))
//        {
//            printf("0 Child %d exited because of exited\n",newpid);
//            inchild = 0;
//        }
//             if(WIFSIGNALED(status)) {
//            printf("0 Child %d exited because of signaled\n",newpid);
//            inchild = 0;
//        }

//             if(WIFSTOPPED(status)) {
//            printf("0 Child %d exited because it stopped\n",newpid);
//            inchild = 0;
//        }



//        if(inchild)
//        {
//            printf("\n Child [%ld]:",(pid_t)newpid);
//
//            if(WIFEXITED(status))
//            {
//                printf("Child %d exited because of exited\n",newpid);
//                inchild = 0;
//            }
//
//            sRC = ptrace( PTRACE_GETREGS, (pid_t)newpid, 0, &registers);
////             if(WIFSIGNALED(status)) {
////            printf("Child %d exited because of signaled\n",newpid);
////            inchild = 0;
////        }
//
////             if(WIFSTOPPED(status)) {
////            printf("Child %d exited because it stopped\n",newpid);
////            inchild = 0;
////        }
//
//
//        }
//        if(!(inchild))
//        {
        sRC = ptrace(PTRACE_GETREGS, child_waited, 0, &registers); // lets fetch all the registers in already built in 'struc'ture to store registers
//        }


        if ( -1 == sRC )
        {
            eprintf( "\nptrace failed with request PTRACE_GETREGS:%s = [%d]\n", strerror( errno ), child_waited ) ;
//            mRC = 1 ;//march31
//            inchild = 0;
            //stoppedPid = waitpid(newpid, &status, __WALL);
            //goto exit ; //dated 11-02-2018
        }
        // BELOW tests the EAX register to see whether the process is stopped on an entry or exit from a system call.

        // When the traced process stops on a system call ENTRY, the EAX register will contain -ENOSYS

        if ( registers.rax == -ENOSYS)
        {

            //Stderr, also known as standard error, is the default file descriptor where a process can write error messages.
            //if the call is entry then we have got the syscall number that we are just entered into in origrax and it easily can be converted to name via array

            fprintf( stderr, "\n %d: %s( ", child_waited, syscalls[registers.orig_rax]) ; //the unistd_64.h file have all the numbers that ORIG_RAX will contain

            //The correct header file to get the system call numbers is sys/syscall.h. The constants are called SYS_### where ### is the name of the system call you are interested in. The __NR_### macros are implementation details and should not be used.
            fprintf( stderr, " %d:",registers.orig_rax);
            switch( registers.orig_rax )
            {
//            case 0:
//                fprintf( stderr, "why am in case 0");
//            break;
      //segmentation fault in case of write March31
            //__NR_open is a macro in the preprocessing it converts into 5 and the next statement would be like case 5:
            case __NR_open:   //define __NR_Open 5:
            {
// Get file name and print the "file name" argument
//in a more fancy way
                char* fileName;
////               readString( stoppedPid, (void *)registers.rbx,
////                           fileName, 1024 ) ;
//                fileName = readString( stoppedPid,registers.rbx);
//                fprintf( stderr, "\"%s\", %#08x, %#08x",
//                         fileName, registers.rcx, registers.rdx ) ;
                //I commented above and changed it to check the results
                fileName = readString( child_waited,registers.rcx);
                fprintf( stderr, "\n\"%s\", %#08x, %#08x",
                         fileName, registers.rcx, registers.rdx ) ;
            }
            break ;
            //__NR_Exit is a macro in the preprocessing it converts into 60 and the next statement would be like case 60:
            //segmentation fault in case of write March31
            case __NR_exit: //define __NR_Exit 60:
                fprintf( stderr, "\n%#08x, %#08x, %#08x ) = ?\n",
                         registers.rbx, registers.rcx, registers.rdx
                       ) ;
                // If the traced process is bailing, so should we
                goto exit ;
                break ;
            default:
            {

//                char* fileName;
//                fileName = readString( stoppedPid, registers.rbx);
//                fprintf( stderr, "\"%s\", %#08x, %#08x",
//                         fileName, registers.rcx, registers.rdx
//                       )


                    fprintf( stderr, "%#08x, %#08x, %#08x",
                         registers.rbx, registers.rcx, registers.rdx);

            }
            break ;
            }
            fprintf( stderr, " ) = " ) ;
        }
        else
        {
            if ( registers.rax < 0 )
            {
// error condition
                fprintf( stderr, "#Err: %s\n", errors[ abs(
                        registers.rax ) ] ) ;
            }
//            else if(registers.rax != 0)
            else
            {
// return code
                fprintf( stderr, "%#08x\n", registers.rax ) ;
            }
        }
        ptrace( PTRACE_SYSCALL, child_waited, 0, 0 ) ;
    }

exit :
    fclose( stdin ) ;
    fclose( stderr ) ;
    fclose( stdout ) ;
    exit( 1 ) ;
    return mRC ;

}



const char *name_of_signal_number(int num)
{
    switch (num)
    {
#define _CHECK_SIGNAL(NM)                                                      \
  case NM:                                                                     \
    return #NM;

#ifdef SIGHUP
        _CHECK_SIGNAL(SIGHUP)
#endif
#ifdef SIGINT
        _CHECK_SIGNAL(SIGINT)
#endif
#ifdef SIGQUIT
        _CHECK_SIGNAL(SIGQUIT)
#endif
#ifdef SIGILL
        _CHECK_SIGNAL(SIGILL)
#endif
#ifdef SIGTRAP
        _CHECK_SIGNAL(SIGTRAP)
#endif
#ifdef SIGABRT
        _CHECK_SIGNAL(SIGABRT)
#endif
#ifdef SIGBUS
        _CHECK_SIGNAL(SIGBUS)
#endif
#ifdef SIGFPE
        _CHECK_SIGNAL(SIGFPE)
#endif
#ifdef SIGKILL
        _CHECK_SIGNAL(SIGKILL)
#endif
#ifdef SIGUSR1
        _CHECK_SIGNAL(SIGUSR1)
#endif
#ifdef SIGSEGV
        _CHECK_SIGNAL(SIGSEGV)
#endif
#ifdef SIGUSR2
        _CHECK_SIGNAL(SIGUSR2)
#endif
#ifdef SIGPIPE
        _CHECK_SIGNAL(SIGPIPE)
#endif
#ifdef SIGALRM
        _CHECK_SIGNAL(SIGALRM)
#endif
#ifdef SIGTERM
        _CHECK_SIGNAL(SIGTERM)
#endif
#ifdef SIGSTKFLT
        _CHECK_SIGNAL(SIGSTKFLT)
#endif
#ifdef SIGCHLD
        _CHECK_SIGNAL(SIGCHLD)
#endif
#ifdef SIGCONT
        _CHECK_SIGNAL(SIGCONT)
#endif
#ifdef SIGSTOP
        _CHECK_SIGNAL(SIGSTOP)
#endif
#ifdef SIGTSTP
        _CHECK_SIGNAL(SIGTSTP)
#endif
#ifdef SIGTTIN
        _CHECK_SIGNAL(SIGTTIN)
#endif
#ifdef SIGTTOU
        _CHECK_SIGNAL(SIGTTOU)
#endif
#ifdef SIGURG
        _CHECK_SIGNAL(SIGURG)
#endif
#ifdef SIGXCPU
        _CHECK_SIGNAL(SIGXCPU)
#endif
#ifdef SIGXFSZ
        _CHECK_SIGNAL(SIGXFSZ)
#endif
#ifdef SIGVTALRM
        _CHECK_SIGNAL(SIGVTALRM)
#endif
#ifdef SIGPROF
        _CHECK_SIGNAL(SIGPROF)
#endif
#ifdef SIGWINCH
        _CHECK_SIGNAL(SIGWINCH)
#endif
#ifdef SIGPOLL
        _CHECK_SIGNAL(SIGPOLL)
#endif
#ifdef SIGSYS
        _CHECK_SIGNAL(SIGSYS)
#endif
    }

    return "UNKNOWN";
}
