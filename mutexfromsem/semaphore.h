#include <sys/types.h>

int init(key_t semkey);
int lock(int sem_id);
int unlock(int sem_id);

