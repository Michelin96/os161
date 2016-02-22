/*
 * Testing stub system calls.
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

int
main()
{
	int status;
	printf("Beginning of the Stubtest\n");
	pid_t id = getpid();
	pid_t wid = waitpid(getpid(), &status, 0);
	int pid = fork();
        printf("ID of this process is: %d\n", id);
        printf("Wait ID of this process is: %d\n", wid);
        printf("Fork process ID is: %d\n", pid);
	_exit(1); /* explicit call to exit */
}
