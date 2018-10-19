#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/user.h>
int main() {
printf("1- Program Started \n");	
 	pid_t child;
	long orig_rax;
	child = fork();
		if(child == 0) {
			printf("2- Child Execution Has Started \n");			
			ptrace(PTRACE_TRACEME, 0, NULL, NULL);
			printf("2.5-  Child says 'I am now being traced' \n");			
			execl("/bin/ls", "ls", NULL); //execute file
			printf("3- Child Execution Has Ended \n");
		}
		else {
			printf("4- Parent Execution Has Started \n");
			wait(NULL);
			orig_rax = ptrace(PTRACE_PEEKUSER,child, 8 * ORIG_RAX, NULL);
			printf("5- The child made a system call %ld\n", orig_rax);
			//printf("6 -but what is this %d\n", ORIG_RAX);
			ptrace(PTRACE_CONT, child, NULL, NULL);
			printf("6- Parent Execution Has Ended \n");
		}
printf("7- Program Ended \n");
	return 0;
	}
/*
Hi, thanks a lot, indeed, the register was defined there.
Also, I did a little bit of research and I found out that on 64-bit, EAX's equivalent is RAX. ORIG_RAX (referenced in the AMD64 Architecture Programmer's manual as R11) has the address of 15 and in case you want to ptrace something on 64-bit, you would call ptrace(PTRACE_PEEKUSER, child, 15 * 8, NULL); //15 * 8 because 64-bit registers occupy 8 bytes.
I'm writing this to save people some headaches when tring to follow a ptrace tutorial designed for a 32-bit computer on a 64-bit one :)

cheers */

