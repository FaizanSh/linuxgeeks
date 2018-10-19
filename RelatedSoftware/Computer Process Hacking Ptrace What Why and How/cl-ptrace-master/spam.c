
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/user.h> // for: struct user_regs_struct !

char string[] = "Find this String in memory";

int x = 0;
int flagg = 0xabcd;
int flagg22 = 0xdada;
int forks = 99;
pid_t any_pid;

int main(int argc, char **argv) {
  /* flagg will be found in the RAX register! */
  while(flagg == 0xABCD) {
    x++;
	
  }

printf("I am out of the while loop!!! Now What \n");
forks = fork();
raise(SIGCONT);
printf("Child Status = %d!\n", forks);
raise(SIGCONT);
if (forks == 0){
any_pid = getpid();
printf("I am being traced and my pid is = %d!\n", any_pid);
printf("Secret string is: %s\n", string);
}
else if (forks > 0){
	any_pid = getpid();
	printf("I was the father and my pid was = %d!\n", any_pid);
	printf("Secret string is: %s\n", string);
 	
}  
 
 	 
return x;
}
