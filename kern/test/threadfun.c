/*
 *  Michelle's basic thread test
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

#define NTHREADS 10

static struct semaphore *tsem = NULL;

static
void
init_sem(void)
{
        if (tsem==NULL) {
                tsem = sem_create("tsem", 0);
                if (tsem == NULL) {
                        panic("threadtest: sem_create failed\n");
                }
        }
}

static
void
mythreads(void *junk, unsigned long num)
{
        int ch = '0' + num;
        int i;

        (void)junk;

        for (i=0; i<120; i++) {
                putch(ch);
        }
        V(tsem);
}

/*
 * The idea with this is that you should see
 *
 *   0123456789 <pause> 0123456789
 *
 * (possibly with the numbers in different orders)
 *
 * The delay loop is supposed to be long enough that it should be clear
 * if either timeslicing or the scheduler is not working right.
 */
 
static
void
runthreads()
{
        char name[16];
        int i, result;

        for (i=0; i<NTHREADS; i++) {
                snprintf(name, sizeof(name), "threadtest%d", i);
                result = thread_fork(name, NULL,
                                     mythreads,
                                     NULL, i);
                if (result) {
                        panic("threadtest: thread_fork failed %s)\n", 
                              strerror(result));
                }
        }

        for (i=0; i<NTHREADS; i++) {
                P(tsem);
        }
}


int
threadfun(int nargs, char **args)
{
        (void)nargs;
        (void)args;
        init_sem();
        kprintf("Starting thread test...\n");
        runthreads();
        kprintf("\nThread test done.\n");

        return 0;
}
