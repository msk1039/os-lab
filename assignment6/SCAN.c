/*
============================================================
Disk Scheduling: SCAN (Elevator Algorithm)
------------------------------------------------------------
Idea:
- Head moves in one direction and serves requests on the way.
- It goes to disk end, then reverses direction and serves rest.

Detailed example:
- Head=50, direction=RIGHT, disk end=199.
- Sorted requests: 11,34,41,60,79,92,114,176
- Serve right side first: 60->79->92->114->176.
- Then continue to physical end 199 (even if no request there).
- Reverse and serve left side in reverse order: 41->34->11.
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

    /* split = first index whose request is >= head */
    int split = 0;
    while (split < REQUESTS && queue[split] < head) split++;

    printf("SCAN Disk Scheduling\n");
    printf("----------------------------------------------------------\n");
    printf("Initial Head Position : %d\n", head);
    printf("Direction             : RIGHT then LEFT\n");
    printf("Disk End              : %d\n\n", DISK_SIZE - 1);

    printf("%-20s %-20s %-15s\n", "Current Position", "Next Position", "Seek Distance");
    printf("----------------------------------------------------------\n");

    /* Move right: serve all requests >= head */
    for (int i = split; i < REQUESTS; i++) {
        int seek = abs(queue[i] - current);
        totalSeek += seek;
        printf("%-20d %-20d %-15d\n", current, queue[i], seek);
        current = queue[i];
    }

    /* Go to right end of disk before reversing */
    int rightEnd = DISK_SIZE - 1; /* SCAN explicitly touches disk boundary */
    int seekToEnd = abs(rightEnd - current);
    totalSeek += seekToEnd;
    printf("%-20d %-20d %-15d\n", current, rightEnd, seekToEnd);
    current = rightEnd;

    /* Move left: serve requests < head in reverse order */
    for (int i = split - 1; i >= 0; i--) {
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
