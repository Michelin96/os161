/*
 *  Michelle's basic thread test
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>
#include <spinlock.h>

int numthreads;
unsigned long sharedcounter;
unsigned long truecount;
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
        char *str = (char*)"thread ";

	(void)num;
        (void)junk;

	//thread printout is locked so it will finish before a new one starts
	lock_acquire(testlock);
                kprintf(str);
	lock_release(testlock);
	for(unsigned long i=0; i<maxcount; i++){
		spinlock_acquire(&testlock->spl_lock); 
			sharedcounter++;
		spinlock_release(&testlock->spl_lock);
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
	truecount = maxcount * numthreads;

}


int
spinlockcount(int nargs, char **args)
{
        (void)nargs; 
	char *num = (char*)args[1];
	char *count = (char*)args[2];
	sharedcounter = 0;

	if (count == NULL){
		maxcount = 20000;
	}else{
	maxcount = 0;
		for(unsigned int j=0; j<strlen(count); j++) {
		maxcount = 10 * maxcount + count[j] - '0';
		}
	}


	// making the user input an integer	
	numthreads = 0;
	for(unsigned int i =0; i< strlen(num); i++) {
    		numthreads = 10 * numthreads + num[i] - '0';
	}	

	//check there is at least one thread
	if (numthreads == 0 || num == NULL){
	   	   kprintf("Number of threads must be 1-49. Try again.\n Usage ctr3 threads counter\n");
	   	   return 0;
	}

	kprintf("Number of threads : %d\n", numthreads);
	kprintf("Counter set to: %lu\n", maxcount);
        init_items();
        kprintf("Starting the thread counter using spinlocks...\n");
        runthreads();
        kprintf("\nProcess is done.\n");

        return 0;
}
