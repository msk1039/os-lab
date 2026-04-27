/*
============================================================
Disk Scheduling: FCFS (First Come First Served)
------------------------------------------------------------
Idea:
- Serve disk requests in the exact order they arrive.
- No reordering is done.

Detailed example:
- Head starts at 50, queue order is [82, 170, 43].
- Move 50->82 (seek 32), then 82->170 (seek 88),
  then 170->43 (seek 127).
- Total seek = 32 + 88 + 127 = 247.
- FCFS is simple and fair by arrival order, but not always efficient.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define REQUESTS 8

int main(void) {
    int queue[REQUESTS] = {176, 79, 34, 60, 92, 11, 41, 114};
    int head = 50;
    int totalSeek = 0;
    int current = head;

    printf("FCFS Disk Scheduling\n");
    printf("----------------------------------------------------------\n");
    printf("Initial Head Position : %d\n\n", head);

    printf("%-20s %-20s %-15s\n", "Current Position", "Next Position", "Seek Distance");
    printf("----------------------------------------------------------\n");

    /* Traverse requests exactly in arrival sequence */
    for (int i = 0; i < REQUESTS; i++) {
        int seek = abs(queue[i] - current);
        totalSeek += seek;
        printf("%-20d %-20d %-15d\n", current, queue[i], seek);
        current = queue[i];
    }

    printf("----------------------------------------------------------\n");
    printf("Total Seek Distance   : %d cylinders\n", totalSeek);
    printf("Average Seek Distance : %.2f cylinders/request\n", (float)totalSeek / REQUESTS);
    return 0;
}
