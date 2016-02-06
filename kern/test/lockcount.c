/*
 *  Testing a shared counter that is protected by a lock
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

int numthreads;
unsigned long sharedcounter;
unsigned long maxcount;

static struct semaphore *tsem = NULL;
static struct lock* testlock; 
static
void
init_items(void)
{
        if (tsem==NULL) {
                tsem = sem_create("tsem", 0);
                if (tsem == NULL) {
                        panic("threadtest: sem_create failed\n");
                }
        }
	
	if (testlock==NULL) {
		testlock = lock_create("testlock");
		if (testlock == NULL) {
			panic("synchtest: lock_create failed\n");
		}
	}

}

static
void
mythreads(void *junk, unsigned long num)
{

	(void)num;
        (void)junk;

	for(unsigned long i=0; i<maxcount; i++){
		lock_acquire(testlock); 
			sharedcounter++;
		lock_release(testlock);
	}
	V(tsem);
}

/*
 */
 
static
void
runthreads()
{
        char name[16];
        int i, result;
	
        for (i=0; i<numthreads; i++) {
                snprintf(name, sizeof(name), "threadtest%d", i);
                result = thread_fork(name, NULL,
                                     mythreads,
                                     NULL, i);
                if (result) {
                        panic("threadtest: thread_fork failed %s)\n", 
             		 strerror(result));
                	}
	
	}
                

        for (i=0; i<numthreads; i++) {
                P(tsem);
        }

}


int
lockcount(int nargs, char **args)
{

	char *num = (char*)args[1];
	char *count = (char*)args[2];
	sharedcounter = 0;

	if (nargs == 1){
	   	   kprintf("Number of threads must be 1-49. Try again.\n Usage ctr2 threads counter\n");
	   	   return 0;
	}else
	{
	// making the user input an integer	
	numthreads = 0;
	for(unsigned int i =0; i< strlen(num); i++) {
    		numthreads = 10 * numthreads + num[i] - '0';
		}	
	}

	
	if (nargs == 3){
		maxcount = 0;
		for(unsigned int j=0; j<strlen(count); j++) {
			maxcount = 10 * maxcount + count[j] - '0';
		}
	}else
	{
		maxcount = 20000;
	}

	kprintf("Counter set to: %lu\n", maxcount);
        init_items();
        kprintf("Starting the thread counter using locks...\n");
        runthreads();
        kprintf("\nProcess is done.\n");

        return 0;
}
