/*
 * Print out the process ID
 */

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int
main()
{
	int id = getpid();
	printf("ID of this process is: %d\n", id);
	return 0; /* avoid compiler warnings */
}
