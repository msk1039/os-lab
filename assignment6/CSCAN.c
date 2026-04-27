/*
============================================================
Disk Scheduling: C-SCAN (Circular SCAN)
------------------------------------------------------------
Idea:
- Head moves in one direction only (here: right).
- After reaching disk end, it jumps to start (0) and continues right.
- Requests are served in a circular manner.

Detailed example:
- Head=50, direction only RIGHT.
- Serve right-side requests in ascending order (>=50).
- Continue to disk end (199), then perform circular jump 199->0.
- Resume rightward scan from 0 to service previously left-side requests.
- This gives more uniform wait time than SCAN for cyclic workloads.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define REQUESTS 8
#define DISK_SIZE 200

int cmp(const void *a, const void *b) {
    return (*(const int *)a - *(const int *)b);
}

int main(void) {
    int queue[REQUESTS] = {176, 79, 34, 60, 92, 11, 41, 114};
    int head = 50;
    int current = head;
    int totalSeek = 0;

    qsort(queue, REQUESTS, sizeof(int), cmp);

    /* split = first request not less than head */
    int split = 0;
    while (split < REQUESTS && queue[split] < head) split++;

    printf("C-SCAN Disk Scheduling\n");
    printf("----------------------------------------------------------\n");
    printf("Initial Head Position : %d\n", head);
    printf("Direction             : RIGHT (circular)\n");
    printf("Disk Range            : 0 to %d\n\n", DISK_SIZE - 1);

    printf("%-20s %-20s %-15s\n", "Current Position", "Next Position", "Seek Distance");
    printf("----------------------------------------------------------\n");

    /* Serve right-side requests */
    for (int i = split; i < REQUESTS; i++) {
        int seek = abs(queue[i] - current);
        totalSeek += seek;
        printf("%-20d %-20d %-15d\n", current, queue[i], seek);
        current = queue[i];
    }

    /* Move to disk end */
    int rightEnd = DISK_SIZE - 1;
    int seekToEnd = abs(rightEnd - current);
    totalSeek += seekToEnd;
    printf("%-20d %-20d %-15d\n", current, rightEnd, seekToEnd);
    current = rightEnd;

    /* Circular jump to start */
    /* Circular jump cost is counted as head movement in this simulation */
    int jumpSeek = abs(current - 0);
    totalSeek += jumpSeek;
    printf("%-20d %-20d %-15d\n", current, 0, jumpSeek);
    current = 0;

    /* Serve left-side requests in ascending order */
    for (int i = 0; i < split; i++) {
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
