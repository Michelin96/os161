/* 
Test Program for the OS161 Menu
OS-Lab 4
*/

#include <types.h>
#include <lib.h>
#include <test.h>

int greettest(int nargs, char *args[])
{
	(void)nargs;
	(void)args;

	kprintf("You said: %s\n", args[1]);

	kprintf("Have you had your coffee yet?! -- Colonel Oz \n");

return 0;

}
