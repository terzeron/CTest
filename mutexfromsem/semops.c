#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int init(key_t semkey)
{
    int status = 0, sem_id;
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short int *array;
    } ctl_arg;

    sem_id = semget(semkey, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (sem_id < 0) {
        if (errno == EEXIST) {
            sem_id = semget(semkey, 1, 0);
        }
    } else {			
        ctl_arg.val = 1;  
        status = semctl(sem_id, 0, SETVAL, ctl_arg); 
    }

    if (sem_id == -1 || status == -1) {
        errx(1, "can't initialize lock");
    }

    return sem_id;
}

    
int lock(int sem_id)
{
    struct sembuf buf;
    
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;

    if (semop(sem_id, &buf, 1) == -1) {
        errx(1, "can't lock");
    }

    return 0;
}
   

int unlock(int sem_id)
{
    struct sembuf buf;

    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;

    if (semop(sem_id, &buf, 1) == -1) {
        errx(1, "can't unlock");
    }

    return 0;
}
