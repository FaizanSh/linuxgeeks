#include <sys/ptrace.h>
#include <stdio.h>
#include <stdlib.h>

#include "die.h"

#include <string.h>
#include <errno.h>
#include <signal.h>
#include <libunwind-ptrace.h>

void stackunwind(pid_t pid)
{
	// if (argc != 2)
	// 	die("USAGE: unwind-pid <pid>\n");

	unw_addr_space_t as = unw_create_addr_space(&_UPT_accessors, 0);

	// pid_t pid = atoi(argv[1]);
	//die("Before Attaching %d pid\n", pid);
	//kill(getpid(),SIGSTOP);
	if (ptrace(PTRACE_ATTACH, pid, 0, 0) != 0)
		die("ERROR: cannot attach to %d\n", pid);

	//die("After Waiting");
	void *context = _UPT_create(pid);
	unw_cursor_t cursor;
	if (unw_init_remote(&cursor, as, context) != 0)
	{
		die("ERROR: cannot initialize cursor for remote unwinding of %d pid\n", pid);
		//edit
		ptrace(PTRACE_CONT, pid, NULL, NULL);
		die("1");
		kill(pid, SIGSTOP);
		die("2");
		waitpid(pid, NULL, 0);
		die("3");
		if (ptrace(PTRACE_DETACH, pid, NULL, NULL) != 0)
		{	
			die("4");
			eprintf("\nptrace failed with request PTRACE_Detach:%s = [%d]\n", strerror(errno), pid);
		}
		else
		{	
			die("5");
			printf("Detached from [%d] \n", pid);
		}

		kill(pid, SIGCONT);
		die("6");
		// Allocates storage
		char *command = (char *)malloc(540 * sizeof(char));
		// Prints "Hello world!" on hello_world
		sprintf(command, "sudo ./unwind-pid %d",pid);

		system(command);

		free(command);
		return;
		//kill(getpid(),SIGSTOP);
		//edit closed
	}
	do
	{
		unw_word_t offset, pc;
		char sym[4096];
		if (unw_get_reg(&cursor, UNW_REG_IP, &pc))
			die("ERROR: cannot read program counter\n");

		printf("0x%lx: ", pc);

		if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0)
			printf("(%s+0x%lx)\n", sym, offset);
		else
			printf("-- no symbol name found\n");
	} while (unw_step(&cursor) > 0);

	_UPT_destroy(context);
	(void)ptrace(PTRACE_DETACH, pid, 0, 0);
}
