#include <spawn.h>

extern int checkResults(char *str, int rc);
    
int checkResults(char *str, int rc)
{
    if (rc == 0) {
        printf("%s", str);
    }
    return 0;
}

typedef struct inheritance {
    posix_spawnattr_t flags;
} inheritance_t;


