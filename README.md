# Crashtrace -Crash Analysis Tool for Profiling Multi-Tasking Applications. 

# Abstract
Software applications are prone to crash due to resource snatching or resource unavailability. Many applications don’t have the mechanism to restart itself and try to reacquire unavailable resource in many cases the resource is available, but it is being held by a stray lock. Issues like these take days in manual debugging. This potential issue must be looked after by developing an application that robustly checks the locks; Whether they are susceptible to system crash, power failure and permanent locking. This will considerably improve software performance and assurance of its working. A software utility capable of analyzing crashes of multitasking application. With its help, developers can reassure that their application has the ability to handle anomalies like power off, sudden kill, accidental resources snatching, race-condition, resources unavailability, permanent locking and etc. As the common effects of these exceptions are, crashing, not restarting of the application, not able to score a particular r¬¬¬¬¬ecourse, a particular feature not responding, and many more. In general, the purpose of this software is to pinpoint any locking functions that might cause the application to behave oddly. Tools utilized in developing this software is kernel utility named as ptrace and libunwind (stack unwind library). 


# Problem Description
End users vigorously use software applications in a different environment. Sometimes application gets crashed and unable to get started. It needs a whole system reboot or software reinstallation. This shouldn’t be the solution of any malfunction occurred. If end users have tech knowledge; strace could be one solution to check any system call that compels application to crash. Most of the time issue lies in the required resource being locked. These locked resources are the main issues. Applications use locks to synchronize tasks, but if exceptional handling isn’t done right the application couldn’t unlock the acquired resource in case of sudden kill or power failure. This issue makes application unable to get the same resource and then they couldn’t get restarted without the system reboot or sometimes need software reinstallation
     

# Background
In multitasking application, threads/processes use locks to notify other tasks about the resource they are currently occupying. Sometimes the lock remains unhandled as the task had to stop abruptly or it was killed by the kernel. These stray locks compel the software to behave oddly. In many cases, the utility might not even start or needs a complete reboot. Sometimes the lock is permanent which needs extensive manual debugging before the user can use again the acquired resource. MYSQL Database is one example of it that uses file lock to acquire a socket which has, besides some plus points, more adverse effects. In case of system failure, the DBMS won’t get the chance to leave the resource, so in future, the file is already there, and system considers that socket already being locked, and not allowed to acquire that.

This is one example and there are documented and undocumented many examples to make oneself familiar with the complication that an incautious behavior of a programmer arises. To explore this problem furthermore, I have done manual testing on many software like Chrome, Opera, Firefox, PostgreSQL, SQLite-3, Level-DB, OpenStack, CloudStack, Apache-Hadoob,  Apache-Tomcat, FUSE and Angular-JS in the course of developing this tool to programmatically find the troublesome problematic lock and pinpoint the function on stack that might be responsible for the exception. The testing results of the above software has been documented and will be explained later. Keeping in mind the problems and the efforts end user might be needing to put in, our tool will make rigorous disruption in the locking mechanism of software and show the developer exact spot where the problem is arising. It will help the programmer to check whether the application that is being developed has some potential unhandled locking exceptions that could affect the software performance in the future. 


# Proposed Solution
Our solution comprises of tracing every system call the multitasking software of interest is making using ptrace and then handpick the futex system calls for further appraisal. The same futex call then nullified by kprobe kernel module, this will create a hypothetical condition in which software locking mechanism is robustly stimulated/tested which will generate different exceptions like resource unavailability, resource locked, etc. This makes the software prone to crash, which helps to do the crash analysis. When the software gets crash ‘libunwind’ could help in identifying the methods that result in a crash so, the programmer can do the due tweak if the application was malfunctioning.
 
# Aims and Objectives
Our solution aims to develop a diagnostic and debugging utility for Linux. It can be used to monitor the interactions like system calls, signal deliveries and process states between application and kernel. System administrators, diagnosticians, and trouble-shooters might find it useful for solving the issues where the source code isn’t available as the program’s recompilation isn’t needed. Further, it can be used to robustly check locks before the software release; that might be problematic in the future. And give the programmer a pinpoint location of code where he/she might need to do exception handling.

# Features
## Software Execution
Our software will have the ability to run other programs under its observation. The software utility needs to run the program in a traced environment in order to achieve the below features.
## Attaching to the running process
The software must have the ability to attach with the running process using its process id only. This makes it easier for the tester to test any running program at a certain time.

## Syscalls detection/tracer implementation
A diagnostic utility that can assure the communication between the kernel and the application needs to keep track of every system call it’s every thread is making. Our software needs to show the system calls along with the pid of the thread. These system calls could help the user in identifying the functionality that causes any problem.
## Lock Detection
As a debugging utility, our software will have an optional feature to robustly check the locks in order to identify the potential problematic situation that needs tweaking in code.
## Printing Backtrace
To identify the specific location where the code needs exception handling the stack trace will come to rescue. Without looking at the code programmer could see where the code demands to tweak.


# Tools Used
ptrace(), libunwind, C language, signal handling
