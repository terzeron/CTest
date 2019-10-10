#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <errno.h>

extern int errno;

#define SEMPERM 0600

int init(key_t semkey);
int lock(int sem_id);
int unlock(int sem_id);

