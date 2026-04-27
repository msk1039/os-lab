/*
============================================================
Zombie Process Demonstration
------------------------------------------------------------
Idea:
- Child exits quickly.
- Parent does not call wait() immediately.
- Child remains as zombie until parent exits or waits.

Detailed example:
- Parent forks child.
- Child prints message and exits immediately (becomes terminated process).
- Parent does not call `wait()` and sleeps for 15 seconds.
- During sleep interval, child entry remains in process table as zombie.
- Once parent exits, OS cleans up remaining process metadata.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    /*
    `fork()` creates child process.
    Return: 0 in child, child PID in parent, -1 on failure.
    */
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        // CHILD PROCESS
        printf("[CHILD] PID = %d: Child is running...\n", getpid());
        printf("[CHILD] Child is now exiting and may become a Zombie.\n");
        exit(0);
    } else {
        // PARENT PROCESS
        printf("[PARENT] PID = %d: Child PID = %d\n", getpid(), pid);
        printf("[PARENT] Parent is sleeping for 15 seconds...\n");


        /* Parent intentionally avoids wait() to demonstrate zombie state */
        sleep(15);

        printf("[PARENT] Parent woke up. Exiting WITHOUT calling wait().\n");
    }

    return 0;
}
