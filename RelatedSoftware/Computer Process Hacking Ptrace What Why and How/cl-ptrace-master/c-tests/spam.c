/* print some spam - used to test singlestepping for example */

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

#include "util.h"
#include "util.c"
// Found it! The characters are layout out, :little-endian,
// style on the machine. So "Find" is in memory "dniF" so to speak
// Also each char maps to good old ASCI, so we can use cl:char-int
// to querry the letters and finally each char needs just a byte
// to be represented. So find in memory is: 
// (reverse ;; because :little-endian
//     (mapcar #'char-int (coerce "Find" 'list))) */
char string[] = "Find this String in memory";

int x = 0;
int flag = 0xabcd;

int main(int argc, char **argv) {
  /* flag will be found in the RAX register! */
	print_endianness();
  while(flag == 0xABCD) {
    x++;
  }
  printf("Left the endless loop, because flag = %d!\n", flag);
  printf("Secret string is: %s\n", string);
  return x;
}
