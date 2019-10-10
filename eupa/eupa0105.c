#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 2048

int main(void)
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        buf[strlen(buf) - 1] = 0;

        if ((pid = fork()) < 0)
            err(-1, "fork error");
        else if (pid == 0) {
            execlp(buf, buf, (char *) 0);
            err(-1, "couldn't execute: %s", buf);
        }

        if ((pid = waitpid(pid, &status, 0)) < 0)
            err(-1, "waitpid error");
        printf("%% ");
    }
    exit(0);
}

