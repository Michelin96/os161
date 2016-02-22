/*
 * Print out the process ID
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

int
main()
{
	int id = getpid();
	printf("ID of this process is: %d\n", id);
	return 0; /* avoid compiler warnings */
}
