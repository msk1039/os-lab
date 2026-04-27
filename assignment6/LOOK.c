/*
============================================================
Disk Scheduling: LOOK
------------------------------------------------------------
Idea:
- Similar to SCAN, but head reverses at last pending request,
  not at physical disk end.
- Here direction is right first, then left.

Detailed example:
- Head=50, sorted queue: 11,34,41,60,79,92,114,176
- Move right and serve: 60->79->92->114->176
- LOOK now reverses immediately at 176 (last request on right),
  unlike SCAN which goes to disk end.
- Then serve left side in reverse: 41->34->11.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define REQUESTS 8

int cmp(const void *a, const void *b) {
    return (*(const int *)a - *(const int *)b);
}

int main(void) {
    int queue[REQUESTS] = {176, 79, 34, 60, 92, 11, 41, 114};
    int head = 50;
    int current = head;
    int totalSeek = 0;

    qsort(queue, REQUESTS, sizeof(int), cmp);

    /* split divides left-of-head and right-of-head request groups */
    int split = 0;
    while (split < REQUESTS && queue[split] < head) split++;

    printf("LOOK Disk Scheduling\n");
    printf("----------------------------------------------------------\n");
    printf("Initial Head Position : %d\n", head);
    printf("Direction             : RIGHT then LEFT\n\n");

    printf("%-20s %-20s %-15s\n", "Current Position", "Next Position", "Seek Distance");
    printf("----------------------------------------------------------\n");

    /* Move right to highest pending request */
    for (int i = split; i < REQUESTS; i++) {
        int seek = abs(queue[i] - current);
        totalSeek += seek;
        printf("%-20d %-20d %-15d\n", current, queue[i], seek);
        current = queue[i];
    }

    /* Reverse and move left for remaining requests */
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
