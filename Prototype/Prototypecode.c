#include <asm/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>

const char *errors[] = {
    "NULL:","EPERM","ENOENT","ESRCH","EINTR","EIO","ENXIO","E2BIG","ENOEXEC","EBADF","ECHILD","EAGAIN","ENOMEM","EACCES"
    ,"EFAULT"
    ,"ENOTBLK"
    ,"EBUSY"
    ,"EEXIST"
    ,"EXDEV"
    ,"ENODEV"
    ,"ENOTDIR"
    ,"EISDIR"
    ,"EINVAL"
    ,"ENFILE"
    ,"EMFILE"
    ,"ENOTTY"
    ,"ETXTBSY"
    ,"EFBIG"
    ,"ENOSPC"
    ,"ESPIPE"
    ,"EROFS"
    ,"EMLINK" //numeric value: 31
    ,"EPIPE" //numeric value: 32
    ,"EDOM" //numeric value: 33
    ,"ERANGE" //numeric value: 34
    ,"EDEADLK" //numeric value: 35
    ,"ENAMETOOLONG" //numeric value: 36
    ,"ENOLCK" //numeric value: 37
    ,"ENOSYS" //numeric value: 38
    ,"ENOTEMPTY" //numeric value: 39
    ,"ELOOP" //numeric value: 40
    ,"NULL" //numeric value: 41
    ,"ENOMSG" //numeric value: 42
    ,"EIDRM" //numeric value: 43
    ,"ECHRNG" //numeric value: 44
    ,"EL2NSYNC" //numeric value: 45
    ,"EL3HLT" //numeric value: 46
    ,"EL3RST" //numeric value: 47
    ,"ELNRNG" //numeric value: 48
    ,"EUNATCH" //numeric value: 49
    ,"ENOCSI" //numeric value: 50
    ,"EL2HLT" //numeric value: 51
    ,"EBADE" //numeric value: 52
    ,"EBADR" //numeric value: 53
    ,"EXFULL" //numeric value: 54
    ,"ENOANO" //numeric value: 55
    ,"EBADRQC" //numeric value: 56
    ,"EBADSLT" //numeric value: 57
    ,"NULL" //numeric value: 58
    ,"EBFONT" //numeric value: 59
    ,"ENOSTR" //numeric value: 60
    ,"ENODATA" //numeric value: 61
    ,"ETIME" //numeric value: 62
    ,"ENOSR" //numeric value: 63
    ,"ENONET" //numeric value: 64
    ,"ENOPKG" //numeric value: 65
    ,"EREMOTE" //numeric value: 66
    ,"ENOLINK" //numeric value: 67
    ,"EADV" //numeric value: 68
    ,"ESRMNT" //numeric value: 69
    ,"ECOMM" //numeric value: 70
    ,"EPROTO" //numeric value: 71
    ,"EMULTIHOP" //numeric value: 72
    ,"EDOTDOT" //numeric value: 73
    ,"EBADMSG" //numeric value: 74
    ,"EOVERFLOW" //numeric value: 75
    ,"ENOTUNIQ" //numeric value: 76
    ,"EBADFD" //numeric value: 77
    ,"EREMCHG" //numeric value: 78
    ,"ELIBACC" //numeric value: 79
    ,"ELIBBAD" //numeric value: 80
    ,"ELIBSCN" //numeric value: 81
    ,"ELIBMAX" //numeric value: 82
    ,"ELIBEXEC" //numeric value: 83
    ,"EILSEQ" //numeric value: 84
    ,"ERESTART" //numeric value: 85
    ,"ESTRPIPE" //numeric value: 86
    ,"EUSERS" //numeric value: 87
    ,"ENOTSOCK" //numeric value: 88
    ,"EDESTADDRREQ" //numeric value: 89
    ,"EMSGSIZE" //numeric value: 90
    ,"EPROTOTYPE" //numeric value: 91
    ,"ENOPROTOOPT" //numeric value: 92
    ,"EPROTONOSUPPORT" //numeric value: 93
    ,"ESOCKTNOSUPPORT" //numeric value: 94
    ,"EOPNOTSUPP" //numeric value: 95
    ,"EPFNOSUPPORT" //numeric value: 96
    ,"EAFNOSUPPORT" //numeric value: 97
    ,"EADDRINUSE" //numeric value: 98
    ,"EADDRNOTAVAIL" //numeric value: 99
    ,"ENETDOWN" //numeric value: 100
    ,"ENETUNREACH" //numeric value: 101
    ,"ENETRESET" //numeric value: 102
    ,"ECONNABORTED" //numeric value: 103
    ,"ECONNRESET" //numeric value: 104
    ,"ENOBUFS" //numeric value: 105
    ,"EISCONN" //numeric value: 106
    ,"ENOTCONN" //numeric value: 107
    ,"ESHUTDOWN" //numeric value: 108
    ,"ETOOMANYREFS" //numeric value: 109
    ,"ETIMEDOUT" //numeric value: 110
    ,"ECONNREFUSED" //numeric value: 111
    ,"EHOSTDOWN" //numeric value: 112
    ,"EHOSTUNREACH" //numeric value: 113
    ,"EALREADY" //numeric value: 114
    ,"EINPROGRESS" //numeric value: 115
    ,"ESTALE" //numeric value: 116
    ,"EUCLEAN" //numeric value: 117
    ,"ENOTNAM" //numeric value: 118
    ,"ENAVAIL" //numeric value: 119
    ,"EISNAM" //numeric value: 120
    ,"EREMOTEIO" //numeric value: 121
    ,"EDQUOT" //numeric value: 122
    ,"ENOMEDIUM" //numeric value: 123
    ,"EMEDIUMTYPE" //numeric value: 124
} ;

#define MAX_ERRORS (sizeof( errors )/sizeof( char * ) )
const char *syscalls[] = {
    "read",
    "write",
    "open",
    "close",
    "stat",
    "fstat",
    "lstat",
    "poll",
    "lseek",
    "mmap",
    "mprotect",
    "munmap",
    "brk",
    "rt_sigaction",
    "rt_sigprocmask",
    "rt_sigreturn",
    "ioctl",
    "pread64",
    "pwrite64",
    "readv",
    "writev",
    "access",
    "pipe",
    "select",
    "sched_yield",
    "mremap",
    "msync",
    "mincore",
    "madvise",
    "shmget",
    "shmat",
    "shmctl",
    "dup",
    "dup2",
    "pause",
    "nanosleep",
    "getitimer",
    "alarm",
    "setitimer",
    "getpid",
    "sendfile",
    "socket",
    "connect",
    "accept",
    "sendto",
    "recvfrom",
    "sendmsg",
    "recvmsg",
    "shutdown",
    "bind",
    "listen",
    "getsockname",
    "getpeername",
    "socketpair",
    "setsockopt",
    "getsockopt",
    "clone",
    "fork",
    "vfork",
    "execve",
    "exit",
    "wait4",
    "kill",
    "uname",
    "semget",
    "semop",
    "semctl",
    "shmdt",
    "msgget",
    "msgsnd",
    "msgrcv",
    "msgctl",
    "fcntl",
    "flock",
    "fsync",
    "fdatasync",
    "truncate",
    "ftruncate",
    "getdents",
    "getcwd",
    "chdir",
    "fchdir",
    "rename",
    "mkdir",
    "rmdir",
    "creat",
    "link",
    "unlink",
    "symlink",
    "readlink",
    "chmod",
    "fchmod",
    "chown",
    "fchown",
    "lchown",
    "umask",
    "gettimeofday",
    "getrlimit",
    "getrusage",
    "sysinfo",
    "times",
    "ptrace",
    "getuid",
    "syslog",
    "getgid",
    "setuid",
    "setgid",
    "geteuid",
    "getegid",
    "setpgid",
    "getppid",
    "getpgrp",
    "setsid",
    "setreuid",
    "setregid",
    "getgroups",
    "setgroups",
    "setresuid",
    "getresuid",
    "setresgid",
    "getresgid",
    "getpgid",
    "setfsuid",
    "setfsgid",
    "getsid",
    "capget",
    "capset",
    "rt_sigpending",
    "rt_sigtimedwait",
    "rt_sigqueueinfo",
    "rt_sigsuspend",
    "sigaltstack",
    "utime",
    "mknod",
    "uselib",
    "personality",
    "ustat",
    "statfs",
    "fstatfs",
    "sysfs",
    "getpriority",
    "setpriority",
    "sched_setparam",
    "sched_getparam",
    "sched_setscheduler",
    "sched_getscheduler",
    "sched_get_priority_max",
    "sched_get_priority_min",
    "sched_rr_get_interval",
    "mlock",
    "munlock",
    "mlockall",
    "munlockall",
    "vhangup",
    "modify_ldt",
    "pivot_root",
    "_sysctl",
    "prctl",
    "arch_prctl",
    "adjtimex",
    "setrlimit",
    "chroot",
    "sync",
    "acct",
    "settimeofday",
    "mount",
    "umount2",
    "swapon",
    "swapoff",
    "reboot",
    "sethostname",
    "setdomainname",
    "iopl",
    "ioperm",
    "create_module",
    "init_module",
    "delete_module",
    "get_kernel_syms",
    "query_module",
    "quotactl",
    "nfsservctl",
    "getpmsg",
    "putpmsg",
    "afs_syscall",
    "tuxcall",
    "security",
    "gettid",
    "readahead",
    "setxattr",
    "lsetxattr",
    "fsetxattr",
    "getxattr",
    "lgetxattr",
    "fgetxattr",
    "listxattr",
    "llistxattr",
    "flistxattr",
    "removexattr",
    "lremovexattr",
    "fremovexattr",
    "tkill",
    "time",
    "futex",
    "sched_setaffinity",
    "sched_getaffinity",
    "set_thread_area",
    "io_setup",
    "io_destroy",
    "io_getevents",
    "io_submit",
    "io_cancel",
    "get_thread_area",
    "lookup_dcookie",
    "epoll_create",
    "epoll_ctl_old",
    "epoll_wait_old",
    "remap_file_pages",
    "getdents64",
    "set_tid_address",
    "restart_syscall",
    "semtimedop",
    "fadvise64",
    "timer_create",
    "timer_settime",
    "timer_gettime",
    "timer_getoverrun",
    "timer_delete",
    "clock_settime",
    "clock_gettime",
    "clock_getres",
    "clock_nanosleep",
    "exit_group",
    "epoll_wait",
    "epoll_ctl",
    "tgkill",
    "utimes",
    "vserver",
    "mbind",
    "set_mempolicy",
    "get_mempolicy",
    "mq_open",
    "mq_unlink",
    "mq_timedsend",
    "mq_timedreceive",
    "mq_notify",
    "mq_getsetattr",
    "kexec_load",
    "waitid",
    "add_key",
    "request_key",
    "keyctl",
    "ioprio_set",
    "ioprio_get",
    "inotify_init",
    "inotify_add_watch",
    "inotify_rm_watch",
    "migrate_pages",
    "openat",
    "mkdirat",
    "mknodat",
    "fchownat",
    "futimesat",
    "newfstatat",
    "unlinkat",
    "renameat",
    "linkat",
    "symlinkat",
    "readlinkat",
    "fchmodat",
    "faccessat",
    "pselect6",
    "ppoll",
    "unshare",
    "set_robust_list",
    "get_robust_list",
    "splice",
    "tee",
    "sync_file_range",
    "vmsplice",
    "move_pages",
    "utimensat",
    "epoll_pwait",
    "signalfd",
    "timerfd_create",
    "eventfd",
    "fallocate",
    "timerfd_settime",
    "timerfd_gettime",
    "accept4",
    "signalfd4",
    "eventfd2",
    "epoll_create1",
    "dup3",
    "pipe2",
    "inotify_init1",
    "preadv",
    "pwritev",
    "rt_tgsigqueueinfo",
    "perf_event_open",
    "recvmmsg",
    "fanotify_init",
    "fanotify_mark",
    "prlimit64",
    "name_to_handle_at",
    "open_by_handle_at",
    "clock_adjtime",
    "syncfs",
    "sendmmsg",
    "setns",
    "getcpu",
    "process_vm_readv",
    "process_vm_writev",
    "kcmp",
    "finit_module"
} ;
#define MAX_SYSCALLS (sizeof( syscalls )/sizeof( char * ) )

/* *** eprintf ***
* Description: general error printing function
*/
void eprintf( char const *fmt, ... ) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf( stderr, fmt, ap ) ;
    va_end(ap);
}


char *readString(pid_t child, unsigned long addr) {

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

int spawnChildProcess( int argc, char *argv[] ) {
    int mRC = 0 ; // Return code for this function
    int sRC = 0 ; // Return code for system calls

    char *args [argc+1];
    // execvp needs null terminated array thats why we are coppying this into new one and adding null in the end
    memcpy(args, argv, argc * sizeof(char*)); // mid * is multiply size of char* will give 8 for 64 bit and which means 8 bit for each address and 4 for 32 bit means 4 bit for each address
    args[argc] = NULL;

    //its a siren I just called that notifies the parent to just keep an eye on me
    sRC = ptrace( PTRACE_TRACEME, 0, 0, 0 ) ;
    //ptrace(PTRACE_TRACEME) sets things up so that the parent is the tracer. For any given tracee, only one process at a time can be its tra
    if ( -1 == sRC ) {
        eprintf( "ptrace failed with request \"PTRACE_TRACEME\": %s\n", strerror( errno ) ) ;
        sRC = errno ;
        goto exit ;
    }

    //lets send signal to it self which further will be recieved by the parent
    kill(getpid(), SIGSTOP);

    printf("child to be %s",argv[0]);//lets check what we got
    sRC = execvp(args[0], args) ; // execvp needs null terminated array thats why we are coppying this into new one
    if ( -1 == sRC ) {

        eprintf( "exec failed: %s\n", strerror( errno ) ) ;
        sRC = errno ;
        goto exit ;
    }
exit :
    return mRC ;
}



int traceChildProcess( pid_t tracedPid ) {
    int mRC = 0 ; // Return code for this function
    int sRC = 0 ; // Return code for system calls
    int status = 0 ; // Status of the stopped child process
    pid_t stoppedPid = 0 ; // Process ID of stopped child process
    struct user_regs_struct registers;
    stoppedPid = waitpid( tracedPid, &status, 0 ); // wait for any responce from the child most probably when get sigstop signal because child was said ptracetraceme and in that method sigstop was sent to itself afterthat
    printf( "Child process stopped for exec\n" );
    if ( -1 == stoppedPid ) {
        eprintf( "waitpid failed: %s\n", strerror( errno ) ) ;
        mRC = 1 ;
        goto exit ;
    }

// Tell the child to stop in a system call entry or exit don't care that if it was being already traced but this this means stop on every entry and exit point
    ptrace( PTRACE_SYSCALL, stoppedPid, 0, 0 ) ;

// This is the main tracing loop. When the child stops,
// we examine the system call and its arguments
//If wait() or waitpid() returns due to the delivery of a signal to the calling process,-1 shall be returned and errno set to [EINTR]

    while ( ( stoppedPid = waitpid( tracedPid, &status, 0 ) ) != -1 ) {

        sRC = ptrace( PTRACE_GETREGS, stoppedPid, 0, &registers ) ; // lets fetch all the registers in already built in 'struc'ture to store registers

        if ( -1 == sRC ) {
            eprintf( "ptrace failed with request PTRACE_GETREGS:%s\n", strerror( errno ) ) ;
            mRC = 1 ;
            goto exit ;
        }
        // BELOW tests the EAX register to see whether the process is stopped on an entry or exit from a system call.

        // When the traced process stops on a system call ENTRY, the EAX register will contain -ENOSYS

        if ( registers.rax == -ENOSYS) {
            //if the call is entry then we have got the syscall number that we are just entered into in origrax and it easily can be converted to name via array
            fprintf( stderr, " %d: %s( ", stoppedPid, syscalls[registers.orig_rax]) ; //the unistd_64.h file have all the numbers that ORIG_RAX will contain
            //The correct header file to get the system call numbers is sys/syscall.h. The constants are called SYS_### where ### is the name of the system call you are interested in. The __NR_### macros are implementation details and should not be used.
            fprintf( stderr, " %d:",registers.orig_rax);
            switch( registers.orig_rax ) {
            //__NR_open is a macro in the preprocessing it converts into 5 and the next statement would be like case 5:
            case __NR_openat || __NR_open: { //define __NR_Open 5:
// Get file name and print the "file name" argument
//in a more fancy way
                char* fileName;
//                readString( stoppedPid, (void *)registers.rbx,
//                            fileName, 1024 ) ;
                fileName = readString( stoppedPid, registers.rbx);
                fprintf( stderr, "\"%s\", %#08x, %#08x",
                         fileName, registers.rcx, registers.rdx ) ;
            }
            break ;
            //__NR_Exit is a macro in the preprocessing it converts into 60 and the next statement would be like case 60:
            case __NR_exit: //define __NR_Exit 1:
                fprintf( stderr, "%#08x, %#08x, %#08x ) = ?\n",
                         registers.rbx, registers.rcx, registers.rdx
                       ) ;
                // If the traced process is bailing, so should we
                goto exit ;
                break ;
            default: {

//                char* fileName;
//                fileName = readString( stoppedPid, registers.rbx);
//                fprintf( stderr, "\"%s\", %#08x, %#08x",
//                         fileName, registers.rcx, registers.rdx
//                       ) ;


                fprintf( stderr, "%#08x, %#08x, %#08x",
                         registers.rbx, registers.rcx, registers.rdx);

            }
            break ;
            }
            fprintf( stderr, " ) = " ) ;
        } else {
            if ( registers.rax < 0 ) {
// error condition
                fprintf( stderr, "#Err: %s\n", errors[ abs(
                        registers.rax ) ] ) ;
            } else {
// return code
                fprintf( stderr, "%#08x\n", registers.rax ) ;
            }
        }
        ptrace( PTRACE_SYSCALL, stoppedPid, 0, 0 ) ;
    }

exit :
    fclose( stdin ) ;
    fclose( stderr ) ;
    fclose( stdout ) ;
    exit( 1 ) ;
    return mRC ;

}


int main( int argc, char *argv[] ) {
    int mRC = 0 ; // Return code for this function 'main return code'
    pid_t cpid ; // Child process ID
    cpid = fork() ;

    if ( cpid > 0 ) {
// Parent
        traceChildProcess( -1 ) ;
    } else if ( 0 == cpid ) {
// Child
        spawnChildProcess( argc, &argv[1] ) ;
    } else {
        fprintf( stderr, "Could not fork child (%s)\n", strerror(
                     errno ) ) ;
        mRC = 1 ;
    }
    return mRC ;
}
