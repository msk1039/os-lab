/*
============================================================
Orphan Process Demonstration
------------------------------------------------------------
Idea:
- Parent exits before child.
- Child continues and gets adopted by init/system process.

Detailed example:
- Parent PID = 5000, child PID = 5001 (example values).
- Child prints original parent PID as 5000.
- Parent exits immediately without waiting.
- After a short sleep, child prints new parent PID (usually 1 or launchd PID).
- This confirms the child became an orphan and was re-parented.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    /*
    fork() return values:
    - 0  : currently running in child
    - >0 : currently running in parent (value is child PID)
    - -1 : failure to create process
    */
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        // CHILD PROCESS: This process will become an orphan
        printf("[CHILD] PID = %d, Initial Parent PID = %d\n", getpid(), getppid());

        sleep(5);

        // After sleep, the parent has exited
        printf("[CHILD] After parent exited, New Parent PID = %d\n", getppid());
        printf("[CHILD] Child process, now orphan, is finishing.\n");
        exit(0);
    } else {
        // PARENT PROCESS: Exits quickly and makes child an orphan
        printf("[PARENT] PID = %d: Child PID = %d\n", getpid(), pid);
        printf("[PARENT] Parent is exiting WITHOUT waiting for child.\n");
        printf("[PARENT] Child will become an ORPHAN process.\n");
        exit(0);
    }
}
