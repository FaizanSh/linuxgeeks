#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
//#include <linux/user.h>
#include <sys/syscall.h>
const int long_size = sizeof(long);

void reverse()
{	
	char str[] = "Faizan";
	int i, j;
	char temp;
	for(i = 0, j = strlen(str) - 1;
		i <= j; ++i, --j) {
		temp = str[i];
	str[i] = str[j];
	str[j] = temp;
}
	printf("Faizan is converted to %s",str);
}

int main(int argc, char const *argv[])
{
	/* code */
	reverse();
	return 0;
}

