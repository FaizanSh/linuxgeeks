#ifndef DATA_H
#define DATA_H


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
    "finit_module",
    "sched_setattr",
    "sched_getattr",
    "renameat2",
    "seccomp",
    "getrandom",
    "memfd_create",
    "kexec_file_load",
    "bpf"
} ;
#define MAX_SYSCALLS (sizeof( syscalls )/sizeof( char * ) )

#endif // DATA_H

