/*
 *  Michelle's basic thread test
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

int numthreads;

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
        char *str = (char*)"help ";
        //int i;

	(void)num;
        (void)junk;
       // for (i=0; i<5; i++) {
	lock_acquire(testlock);
                kprintf(str);
	lock_release(testlock);

       // }
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
threadfun(int nargs, char **args)
{
        (void)nargs; // KASSERT(nargs >= 1);
	//nunthreads = (int)*args[1] -'0';
        char *num = (char*)args[1];
	numthreads = 0;
	for(unsigned int i =0; i< strlen(num); i++) {
    		numthreads = 10 * numthreads + num[i] - '0';
		}


        init_items();
        kprintf("Starting thread test...\n");
        runthreads();
        kprintf("\nThread test done.\n");

        return 0;
}
