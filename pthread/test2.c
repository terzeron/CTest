#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <spawn.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include "check.h"

typedef struct {
    int                 eventOccured;
    int                 numberWaiting;
    int                 wokeup;
    int                 reserved[1];
    pthread_cond_t      cond;
    pthread_mutex_t     mutex;        /* Protects this shared data and condition 
                                       */
} shared_data_t;

extern char      **environ;
shared_data_t     *sharedMem=NULL;
pid_t              childPid=0;
int                childStatus=-99;
int                shmid=0;

/* Change this path to be the path to where you create this example program */
#define MYPATH               "/QSYS.LIB/QP0WTEST.LIB/TPCOSP0.PGM"

#define NTHREADSTHISJOB      2
#define NTHREADSTOTAL        4

void parentSetup(void);
void childSetup(void);
void parentCleanup(void);
void childCleanup(void);

void *parentThreadFunc(void *parm)
{
    int           rc;

    rc = pthread_mutex_lock(&sharedMem->mutex);
    checkResults("pthread_mutex_lock()\n", rc);
    /* Under protection of the lock, increment the count */
    ++sharedMem->numberWaiting;

    while (!sharedMem->eventOccured) {
        printf("PARENT - Thread blocked\n");
        rc = pthread_cond_wait(&sharedMem->cond, &sharedMem->mutex);
        checkResults("pthread_cond_wait()\n", rc);
    }
    printf("PARENT - Thread awake!\n");

    /* Under protection of the lock, decrement the count */
    --sharedMem->numberWaiting;

    /* After incrementing the wokeup flage and unlocking the mutex */
    /* we no longer use the shared memory, the parent could destroy*/
    /* it. We indicate we're finished with it using the wokeup flag*/
    ++sharedMem->wokeup;
    rc = pthread_mutex_unlock(&sharedMem->mutex);
    checkResults("pthread_mutex_lock()\n", rc);
    return NULL;
}

void *childThreadFunc(void *parm)
{
    int           rc;

    rc = pthread_mutex_lock(&sharedMem->mutex);
    checkResults("pthread_mutex_lock()\n", rc);
    /* Under protection of the lock, increment the count */
    ++sharedMem->numberWaiting;

    while (!sharedMem->eventOccured) {
        printf("CHILD - Thread blocked\n");
        rc = pthread_cond_wait(&sharedMem->cond, &sharedMem->mutex);
        checkResults("pthread_cond_wait()\n", rc);
    }
    printf("CHILD - Thread awake!\n");

    /* Under protection of the lock, decrement the count */
    --sharedMem->numberWaiting;

    /* After incrementing the wokeup flage and unlocking the mutex */
    /* we no longer use the shared memory, the parent could destroy*/
    /* it. We indicate we're finished with it using the wokeup flag*/
    ++sharedMem->wokeup;
    rc = pthread_mutex_unlock(&sharedMem->mutex);
    checkResults("pthread_mutex_lock()\n", rc);
    return NULL;
}

int main(int argc, char **argv)
{
    int                   rc=0;
    int                   i;
    pthread_t             threadid[NTHREADSTHISJOB];
    int                   parentJob=0;

    /* If we run this from the QSHELL interpreter on the AS/400, we want      */
    /* it to be line buffered even if we run it in batch so the output between*/
    /* parent and child is intermixed.                                        */
    setvbuf(stdout,NULL,_IOLBF,4096);
    /* Determine if we're running in the parent or child */
    if (argc != 1 && argc != 2) {
        printf("Incorrect usage\n");
        printf("Pass no parameters to run as the parent testcase\n");
        printf("Pass one parameter `ASCHILD' to run as the child testcase\n");
        exit(1);
    }

    if (argc == 1) {
        parentJob = 1;
    } else {
        if (strcmp(argv[1], "ASCHILD")) {
            printf("Incorrect usage\n");
            printf("Pass no parameters to run as the parent testcase\n");
            printf("Pass one parameter `ASCHILD' to run as the child testcase\n");
            exit(1);
        }
        parentJob = 0;
    }

    /* PARENT *****************************************************************/
    if (parentJob) {
        printf("PARENT - Enter Testcase - %s\n", argv[0]);
        parentSetup();

        printf("PARENT - Create %d threads\n", NTHREADSTHISJOB);
        for (i=0; i<NTHREADSTHISJOB; ++i) {
            rc = pthread_create(&threadid[i], NULL, parentThreadFunc, NULL);
            checkResults("pthread_create()\n", rc);
        }

        rc = pthread_mutex_lock(&sharedMem->mutex);
        checkResults("pthread_mutex_lock()\n", rc);
        while (sharedMem->numberWaiting != NTHREADSTOTAL) {
            printf("PARENT - Waiting for %d threads to wait, "
                   "currently %d waiting\n",
                   NTHREADSTOTAL, sharedMem->numberWaiting);

            rc = pthread_mutex_unlock(&sharedMem->mutex);
            checkResults("pthread_mutex_unlock()\n", rc);
            sleep(1);
            rc = pthread_mutex_lock(&sharedMem->mutex);
            checkResults("pthread_mutex_lock()\n", rc);
        }

        printf("PARENT - wake up all of the waiters...\n");
        sharedMem->eventOccured = 1;
        rc = pthread_cond_broadcast(&sharedMem->cond);
        checkResults("pthread_cond_signal()\n", rc);

        printf("PARENT - Wait for waking threads and cleanup\n");
        while (sharedMem->wokeup != NTHREADSTOTAL) {
            printf("PARENT - Waiting for %d threads to wake, "
                   "currently %d wokeup\n",
                   NTHREADSTOTAL, sharedMem->wokeup);

            rc = pthread_mutex_unlock(&sharedMem->mutex);
            checkResults("pthread_mutex_unlock()\n", rc);
            sleep(1);
            rc = pthread_mutex_lock(&sharedMem->mutex);
            checkResults("pthread_mutex_lock()\n", rc);
        }

        parentCleanup();
        printf("PARENT - Main completed\n");
        exit(0);
    }

    /* CHILD *****************************************************************/
    {
        void *status=NULL;

        printf("CHILD - Enter Testcase - %s\n", argv[0]);
        childSetup();

        printf("CHILD - Create %d threads\n", NTHREADSTHISJOB);
        for (i=0; i<NTHREADSTHISJOB; ++i) {
            rc = pthread_create(&threadid[i], NULL, childThreadFunc, NULL);
            checkResults("pthread_create()\n", rc);
        }
        /* The parent will wake up all of these threads using the */
        /* pshared condition variable. We'll just join to them... */
        printf("CHILD - Joining to all threads\n");

        for (i=0; i<NTHREADSTHISJOB; ++i) {
            rc = pthread_join(threadid[i], &status);
            checkResults("pthread_join()\n", rc);
            if (status != NULL) {
                //printf("CHILD - Got a bad status from a thread, %.8x %.8x %.8x %.8x\n", status);
                printf("CHILD %d\n", *(int *) status);
                exit(1);
            }
        }
        /* After all the threads are awake, the parent will destroy */
        /* the condition and mutex. Don't use it anymore            */
        childCleanup();
        printf("CHILD - Main completed\n");
    }
    return 0;
}


/***************************************************************/
/* This function initializes the shared memory for the job,    */
/* sets up the environment variable indicating where the shared*/
/* memory is, and spawns the child job.                        */
/*                                                             */
/* It creates and initializes the shared memory segment, and   */
/* It initializes the following global variables in this       */
/* job.                                                        */
/*    sharedMem                                                */
/*    childPid                                                 */
/*    shmid                                                    */
/*                                                             */
/* If any of this setup/initialization fails, it will exit(1)  */
/* and terminate the test.                                     */
/*                                                             */
/***************************************************************/
void parentSetup(void)
{
    int rc;

    /***************************************************************/
    /* Create shared memory for shared_data_t above                */
    /* attach the shared memory                                    */
    /* set the static/global sharedMem pointer to it               */
    /***************************************************************/
    printf("PARENT - Create the shared memory segment\n");
    rc = shmget(IPC_PRIVATE, sizeof(shared_data_t), 0666);
    if (rc == -1) {
        printf("PARENT - Failed to create a shared memory segment\n");
        exit(1);
    }
    shmid = rc;

    printf("PARENT - Attach the shared memory\n");
    sharedMem = shmat(shmid, NULL, 0);
    if (sharedMem == NULL) {
        shmctl(shmid, IPC_RMID, NULL);
        printf("PARENT - Failed to attach shared memory\n");
        exit(1);
    }
    /***************************************************************/
    /* Initialize the mutex/condition and other shared memory data */
    /***************************************************************/
    {
        pthread_mutexattr_t              mattr;
        pthread_condattr_t               cattr;

        printf("PARENT - Init shared memory mutex/cond\n");
        memset(sharedMem, 0, sizeof(shared_data_t));

        /* Process Shared Mutex */
        rc = pthread_mutexattr_init(&mattr);
        checkResults("pthread_mutexattr_init()\n", rc);

        rc = pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
        checkResults("pthread_mutexattr_setpshared()\n", rc);

        rc = pthread_mutex_init(&sharedMem->mutex, &mattr);
        checkResults("pthread_mutex_init()\n", rc);

        /* Process Shared Condition */
        rc = pthread_condattr_init(&cattr);
        checkResults("pthread_condattr_init()\n", rc);

        rc = pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
        checkResults("pthread_condattr_setpshared()\n", rc);

        rc = pthread_cond_init(&sharedMem->cond, &cattr);
        checkResults("pthread_cond_init()\n", rc);
    }
    /**************************************************************/
    /* Set and environment variable so that the child can inherit */
    /* it and know the shared memory ID                           */
    /**************************************************************/
    {
        char        shmIdEnvVar[128];
        sprintf(shmIdEnvVar, "TPCOSP0_SHMID=%d\n", shmid);
        rc = putenv(shmIdEnvVar);
        if (rc) {
            printf("PARENT - Failed to store env var %s, errno=%d\n",
                   shmIdEnvVar, errno);
            exit(1);
        }
        printf("PARENT - Stored shared memory id of %d\n", shmid);
    }

    /**************************************************/
    /* Spawn the child job                            */
    /**************************************************/
    {
        inheritance_t  in;
        char          *av[3] = {NULL, NULL, NULL};

        /* Allow thread creation in the spawned child          */
        memset(&in, 0, sizeof(in));
        in.flags.__flags = POSIX_SPAWN_SETPGROUP;

        /* Set up the arguments to pass to spawn based on the  */
        /* arguments passed in                                 */
        av[0] = MYPATH;
        av[1] = "ASCHILD";
        av[2] = NULL;

        /* Spawn the child that was specified, inheriting all  */
        /* of the environment variables.                       */
        childPid = posix_spawn(MYPATH, 0, NULL, &in, av, environ);
        if (childPid == -1) {
            /* spawn failure */
            printf("PARENT - spawn() failed, errno=%d\n", errno);
            exit(1);
        }

        printf("PARENT - spawn() success, [PID=%d]\n", childPid);
    }

    return;
}

/***************************************************************/
/* This function attaches the shared memory for the child job, */
/* It uses the environment variable indicating where the shared*/
/* memory is.                                                  */
/*                                                             */
/* If any of this setup/initialization fails, it will exit(1)  */
/* and terminate the test.                                     */
/*                                                             */
/* It initializes the following global variables:              */
/*    sharedMem                                                */
/*    shmid                                                    */
/***************************************************************/
void childSetup(void)
{
    //int rc;

    printf("CHILD - Child setup\n");
    /**************************************************************/
    /* Set and environment variable so that the child can inherit */
    /* it and know the shared memory ID                           */
    /**************************************************************/
    {
        char        *shmIdEnvVar;
        shmIdEnvVar = getenv("TPCOSP0_SHMID");
        if (shmIdEnvVar == NULL) {
            printf("CHILD - Failed to get env var \"TPCOSP0_SHMID\", errno=%d\n",
                   errno);
            exit(1);
        }
        shmid = atoi(shmIdEnvVar);
        printf("CHILD - Got shared memory id of %d\n", shmid);
    }
    /***************************************************************/
    /* Create shared memory for shared_data_t above                */
    /* attach the shared memory                                    */
    /* set the static/global sharedMem pointer to it               */
    /***************************************************************/
    printf("CHILD - Attach the shared memory\n");
    sharedMem = shmat(shmid, NULL, 0);
    if (sharedMem == NULL) {
        shmctl(shmid, IPC_RMID, NULL);
        printf("CHILD - Failed to attach shared memory\n");
        exit(1);
    }
    return;
}

/***************************************************************/
/* wait for child to complete and get return code.             */
/* Destroy mutex and condition in shared memory                */
/* detach and remove shared memory                             */
/* set the child's return code in global storage               */
/*                                                             */
/* If this function fails, it will call exit(1)                */
/*                                                             */
/* This function sets the following global variables:          */
/*    sharedMem                                                */
/*    childStatus                                              */
/*    shmid                                                    */
/***************************************************************/
void parentCleanup(void)
{
    int         status=0;
    int         rc;
    int         waitedPid = 0;

    /* Even though there's no thread left in the child using the   */
    /* contents of the shared memory, before we destroy the mutex  */
    /* and condition in that shared memory, we'll wait for the     */
    /* child job to complete, we know for 100% certainty that no   */
    /* threads in the child are using it then.                     */
    printf("PARENT - Parent cleanup\n");
    /* Wait for the child to complete */
    waitedPid = waitpid(childPid, &status, 0);
    if (rc == -1) {
        printf("PARENT - waitpid failed, errno=%d\n", errno);
        exit(1);
    }
    childStatus = status;

    /* Cleanup resources */
    rc = pthread_mutex_destroy(&sharedMem->mutex);
    checkResults("pthread_mutex_destroy()\n", rc);

    rc = pthread_cond_destroy(&sharedMem->cond);
    checkResults("pthread_cond_destroy()\n", rc);

    /* Detach/Remove shared memory */
    rc = shmdt(sharedMem);
    if (rc) {
        printf("PARENT - Failed to detach shared memory, errno=%d\n", errno);
        exit(1);
    }
    rc = shmctl(shmid, IPC_RMID, NULL);
    if (rc) {
        printf("PARENT - Failed to remove shared memory id=%d, errno=%d\n",
               shmid, errno);
        exit(1);
    }
    shmid = 0;
    return;
}

/***************************************************************/
/* Detach the shared memory                                    */
/* At this point, there is no serialization, so the contents   */
/* of the shared memory should not be used.                    */
/*                                                             */
/* If this function fails, it will call exit(1)                */
/*                                                             */
/* This function sets the following global variables:          */
/*    sharedMem                                                */
/***************************************************************/
void childCleanup(void)
{
    int rc;

    printf("CHILD - Child cleanup\n");
    rc = shmdt(sharedMem);
    sharedMem = NULL;
    if (rc) {
        printf("CHILD - Failed to detach shared memory, errno=%d\n", errno);
        exit(1);
    }
    return;
}

