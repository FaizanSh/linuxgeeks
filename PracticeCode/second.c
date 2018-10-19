#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/user.h>
//#include <linux/user.h>
#include <sys/syscall.h> /* For SYS_write etc */
int main()
{ 
printf("1- Main started \n");
pid_t child;
pid_t check;
pid_t check2;
long orig_rax, rax;
long params[3];
int status=0;
int insyscall = 0;
child = fork();

if(child == 0) {
	check = getpid();
	check2 = getppid();
	printf("2- Entered Child %d of parent %d \n",check,check2);
	ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	execl("/bin/ls", "ls", NULL);
	printf("6- Child Exited \n");
	}
else {
	check = getpid();
	check2 = getppid();
	printf("3- Entered Parent %d of Any %d \n",check,check2);
	while(1) {
	printf("I am in while(1) \n");
	printf("status value is %d before wait \n",status);	
	wait(&status);
	printf("wait(%p) crossed; status value is %d  \n",&status,status);
	if(WIFEXITED(status))
	printf("WIFEXITED(status) crossed %d \n",status);
		break;

	orig_rax = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
	printf("orig_rax is %ld",orig_rax);
	printf("what is sys_write %d",SYS_write);
	if(orig_rax == SYS_write) {
		
		if(insyscall == 0) {
	/* Syscall entry */
			insyscall = 1;
			params[0] = ptrace(PTRACE_PEEKUSER,
			child, 8 * RBX,
			NULL);
			params[1] = ptrace(PTRACE_PEEKUSER,
			child, 8 * RCX,
			NULL);
			params[2] = ptrace(PTRACE_PEEKUSER,
			child, 8 * RDX,
			NULL);
			printf("Write called with "
			"%ld, %ld, %ld\n",
			params[0], params[1],
			params[2]);
		}
	else { /* Syscall exit */
		rax = ptrace(PTRACE_PEEKUSER,
		child, 8 * RAX, NULL);
		printf("Write returned "
		"with %ld\n", rax);
		insyscall = 0;
		}
	}
printf("7- PTRACE_SYSCALL CALLED \n");
ptrace(PTRACE_SYSCALL,child, NULL, NULL);

	}//while loop ended
printf("4- Parent Exited \n");
printf("5- Main Exited \n");
}
return 0;
}

/*10/14/2018 Playing with ptrace, Part I | Linux Journal
https://www.linuxjournal.com/article/6100 6/17 */

