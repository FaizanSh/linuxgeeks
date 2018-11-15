#include <sys/ptrace.h> //sys/ptrace.h defines ptrace and the __ptrace_request constants
#include <sys/reg.h> //we'll need sys/reg.h to help decode system calls
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <locale.h>

//We'll start with the entry point. We check that we were passed a
//command, and then we fork() to create two processes — one to execute
//the program to be traced, and the other to trace it.

static void init(int argc, char *argv[]){
 if (argc < 2) {
        fprintf(stderr, "Usage: %s prog args\n", argv[0]);
        exit(1);
    }
}


int do_child(int argc, char *argv[]);


int do_trace(pid_t child);
//wait_for_syscall, defined below, will
//run the child until either entry to or exit from a system call. If it
//returns non-zero, the child has exited
int wait_for_syscall(pid_t child); //we prototype a function we'll need later



int do_child(int argc, char *argv[]) {
    char *args [argc+1];



//      copy memory area
//      void *memcpy(void *dest, const void *src, size_t n); size_t is the unsign datatype
    memcpy(args, argv, argc * sizeof(char*));

//       The  memcpy()  function  copies  n bytes from memory area src to memory
//       area dest.  The memory areas must not overlap.  Use memmove(3)  if  the
//       memory areas do overlap.

    args[argc] = NULL;


    ptrace(PTRACE_TRACEME);
    kill(getpid(), SIGSTOP);
    return execvp(args[0], args);

//  The child starts with some trivial marshalling of arguments, since
//  execvp wants a NULL-terminated argument array.
}
/*
* Next, we just execute the provided argument list, but first, we need
* to start the tracing process, so that the parent can start tracing the
* newly-executed program from the very start.

* If a child knows that it wants to be traced, it can make the
* PTRACE_TRACEME ptrace request, which starts tracing. In addition, it
* means that the next signal sent to this process wil stop it and notify
* the parent (via wait), so that the parent knows to start tracing. So,
* after doing a TRACEME, we SIGSTOP ourselves, so that the parent can
* continue our execution with the exec call. */



int do_trace(pid_t child) {
    int status, syscall, retval; //retval=returnValue, syscall=syscallCode status=?
    waitpid(child, &status, 0); //wait for the child's pid process to broadcast any signal
//We immediately waitpid on the child, which will return once the child has sent itself the SIGSTOP above
    ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

    /*PTRACE_SETOPTIONS (since Linux 2.4.6; see BUGS for caveats)
                  Set ptrace options from data.  (addr is ignored.)  data is
                  interpreted as a bit mask of options, which are specified by
                  the following flags:

                  PTRACE_O_EXITKILL (since Linux 3.8)
                         Send a SIGKILL signal to the tracee if the tracer
                         exits.  This option is useful for ptrace jailers that
                         want to ensure that tracees can never escape the
                         tracer's control.

                  PTRACE_O_TRACECLONE (since Linux 2.5.46)
                         Stop the tracee at the next clone(2) and automatically
                         start tracing the newly cloned process, which will
                         start with a SIGSTOP, or PTRACE_EVENT_STOP if
                         PTRACE_SEIZE was used.  A waitpid(2) by the tracer will
                         return a status value such that

                           status>>8 == (SIGTRAP | (PTRACE_EVENT_CLONE<<8))

                         The PID of the new process can be retrieved with
                         PTRACE_GETEVENTMSG.

                         This option may not catch clone(2) calls in all cases.
                         If the tracee calls clone(2) with the CLONE_VFORK flag,
                         PTRACE_EVENT_VFORK will be delivered instead if
                         PTRACE_O_TRACEVFORK is set; otherwise if the tracee
                         calls clone(2) with the exit signal set to SIGCHLD,
                         PTRACE_EVENT_FORK will be delivered if PTRACE_O_TRACE‐
                         FORK is set.

                  PTRACE_O_TRACEEXEC (since Linux 2.5.46)
                         Stop the tracee at the next execve(2).  A waitpid(2) by
                         the tracer will return a status value such that

                           status>>8 == (SIGTRAP | (PTRACE_EVENT_EXEC<<8))

                         If the execing thread is not a thread group leader, the
                         thread ID is reset to thread group leader's ID before
                         this stop.  Since Linux 3.0, the former thread ID can
                         be retrieved with PTRACE_GETEVENTMSG.

                  PTRACE_O_TRACEEXIT (since Linux 2.5.60)
                         Stop the tracee at exit.  A waitpid(2) by the tracer
                         will return a status value such that

                           status>>8 == (SIGTRAP | (PTRACE_EVENT_EXIT<<8))

                         The tracee's exit status can be retrieved with
                         PTRACE_GETEVENTMSG.

                         The tracee is stopped early during process exit, when
                         registers are still available, allowing the tracer to
                         see where the exit occurred, whereas the normal exit
                         notification is done after the process is finished
                         exiting.  Even though context is available, the tracer
                         cannot prevent the exit from happening at this point.

                  PTRACE_O_TRACEFORK (since Linux 2.5.46)
                         Stop the tracee at the next fork(2) and automatically
                         start tracing the newly forked process, which will
                         start with a SIGSTOP, or PTRACE_EVENT_STOP if
                         PTRACE_SEIZE was used.  A waitpid(2) by the tracer will
                         return a status value such that

                           status>>8 == (SIGTRAP | (PTRACE_EVENT_FORK<<8))

                         The PID of the new process can be retrieved with
                         PTRACE_GETEVENTMSG.

                  PTRACE_O_TRACESYSGOOD (since Linux 2.4.6)
                         When delivering system call traps, set bit 7 in the
                         signal number (i.e., deliver SIGTRAP|0x80).  This makes
                         it easy for the tracer to distinguish normal traps from
                         those caused by a system call.  (PTRACE_O_TRACESYSGOOD
                         may not work on all architectures.)

                  PTRACE_O_TRACEVFORK (since Linux 2.5.46)
                         Stop the tracee at the next vfork(2) and automatically
                         start tracing the newly vforked process, which will
                         start with a SIGSTOP, or PTRACE_EVENT_STOP if
                         PTRACE_SEIZE was used.  A waitpid(2) by the tracer will
                         return a status value such that

                           status>>8 == (SIGTRAP | (PTRACE_EVENT_VFORK<<8))

                         The PID of the new process can be retrieved with
                         PTRACE_GETEVENTMSG.

                  PTRACE_O_TRACEVFORKDONE (since Linux 2.5.60)
                         Stop the tracee at the completion of the next vfork(2).
                         A waitpid(2) by the tracer will return a status value
                         such that

                           status>>8 == (SIGTRAP | (PTRACE_EVENT_VFORK_DONE<<8))

                         The PID of the new process can (since Linux 2.6.18) be
                         retrieved with PTRACE_GETEVENTMSG.

                  PTRACE_O_TRACESECCOMP (since Linux 3.5)
                         Stop the tracee when a seccomp(2) SECCOMP_RET_TRACE
                         rule is triggered.  A waitpid(2) by the tracer will
                         return a status value such that

                           status>>8 == (SIGTRAP | (PTRACE_EVENT_SECCOMP<<8))

                         While this triggers a PTRACE_EVENT stop, it is similar
                         to a syscall-enter-stop.  For details, see the note on
                         PTRACE_EVENT_SECCOMP below.  The seccomp event message
                         data (from the SECCOMP_RET_DATA portion of the seccomp
                         filter rule) can be retrieved with PTRACE_GETEVENTMSG.

                  PTRACE_O_SUSPEND_SECCOMP (since Linux 4.3)
                         Suspend the tracee's seccomp protections.  This applies
                         regardless of mode, and can be used when the tracee has
                         not yet installed seccomp filters.  That is, a valid
                         use case is to suspend a tracee's seccomp protections
                         before they are installed by the tracee, let the tracee
                         install the filters, and then clear this flag when the
                         filters should be resumed.  Setting this option
                         requires that the tracer have the CAP_SYS_ADMIN capa‐
                         bility, not have any seccomp protections installed, and
                         not have PTRACE_O_SUSPEND_SECCOMP set

    */
    while(1) {
        //for syscall entering check
        if (wait_for_syscall(child) != 0)
            break;
        //checking syscall number
        syscall = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*ORIG_RAX);
        fprintf(stderr, "syscall(%d) = ", syscall);
        //for exit syscall check
        if (wait_for_syscall(child) != 0)
            break;
        //geting return value
        retval = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*RAX);
        fprintf(stderr, "%d\n", retval);
    }
    return 0;
}

int wait_for_syscall(pid_t child) {
    int status;
    while (1) {
        ptrace(PTRACE_SYSCALL, child, 0, 0);
        waitpid(child, &status, 0);

        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
            return 0;

        if (WIFEXITED(status))
            return 1;
    }
}

char *read_string(pid_t child, unsigned long addr) {

//read_string takes a child to read from, and the address of the string
//it's going to read.

    char *val = malloc(4096);
    int allocated = 4096, read;
    unsigned long tmp;

//We need some variables. A buffer to copy the string into, counters of
//how much data we've copied and allocated, and a temporary variable for
//reading memory.

    while (1) {
        if (read + sizeof tmp > allocated) {
            allocated *= 2;
            val = realloc(val, allocated);
        }

//We grow the buffer if necessary. We read data one word at a time.

        tmp = ptrace(PTRACE_PEEKDATA, child, addr + read);
        if(errno != 0) {
            val[read] = 0;
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



int main(int argc, char *argv[]) {

    setlocale(LC_ALL, ""); //set localization
    //A C program inherits its locale environment variables when it starts up. This happens automatically. However, these variables do not automatically control the locale used by the library functions, because ISO C says that all programs start by default in the standard ‘C’ locale. To use the locales specified by the environment, you must call setlocale. Call it as follows:
    //
    //setlocale (LC_ALL, "");
    //to select a locale based on the user choice of the appropriate environment variables.
    //
    //You can also use setlocale to specify a particular locale, for general use or for a specific category.
    //
    //The symbols in this section are defined in the header file locale.h.

    //checking if we have passed a command
    init(argc, argv);


//if yes then fork the program to create tracer and tracee
    pid_t child = fork();


    if (child == 0) {
        return do_child(argc-1, argv+1);
    } else {
        return do_trace(child);
    }
}
