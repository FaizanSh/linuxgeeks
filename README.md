# Crash Analysis Tool for Profiling Multi-Tasking Applications 

# Motivation
In Linux, any process or thread is called a task. An application consists of multiple tasks (threads and(or) processes). When one task holds a resource, other tasks have to wait. In multitasking systems, different tasks can run simultaneously and even can share a common resource. And when sharing happens, the task which is going to use the resource will apply a lock (mutex, semaphore, spinlock etc.) to proclaim its ownership and prevent intruders from claiming that specific resource. In this way, several tasks can use the same resource, but exclusively.  

But sometimes things don’t go as planned. Your application lock something but operating system crashes the application or user plug out the power. In this case, the application’s credibility depends upon its locking mechanism because it didn’t get the time to unlock the resource. If the locking mechanism was not time-dependent or object-dependent as file locks, so when the application gets restarted it needs to acquire the same resource which is not locked by another process, but itself. This is when the problem arises, and software will get stuck along with the resource. The user will manually locate this problem to make the system running, but everyone will never know how and why this error occurred in the first place.  

The example of this issue is the MYSQL database. It uses a file to record socket lock, and once in a blue moon when system abruptly gets shut down it doesn’t get the time to remove that lock file which in result restrict acquiring that socket again or sometimes do not allow the database to restart.    

# Plan
The main purpose of my final year project is to make a terminal utility that will try to disrupt the application locks and identify any potential issues like software crashing, permanent locking or restarting only after a system reboot. If possible, we will try to identify the type of locks along with the critical resources that were locked. In addition to that, we will (if possible for that software) also identify the processes that were waiting for the resource as well as the back trace of specific calls that lead to this problem. This aiding information would be enough for the debugger/programmer to identify where the problem has occurred and where he/she needs to make changes to get the software live without errors

# Progress
I'm still in learning phase
Ptrace()
Signals
Fork
Exec
