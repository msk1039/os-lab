/*
============================================================
Disk Scheduling: SSTF (Shortest Seek Time First)
------------------------------------------------------------
Idea:
- From current head position, choose the pending request with
  the smallest seek distance.
- Repeat until all requests are served.

Detailed example:
- Head=50, pending requests: 176,79,34,60,92,11,41,114
- Nearest request from 50 is 41 (distance 9), so serve 41 first.
- From 41, nearest might be 34 (distance 7), then 60 (distance 26), etc.
- At each step, scheduler recomputes nearest from CURRENT head position.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define REQUESTS 8

int main(void) {
    int queue[REQUESTS] = {176, 79, 34, 60, 92, 11, 41, 114};
    int visited[REQUESTS] = {0};

    int head = 50;
    int current = head;
    int totalSeek = 0;

    printf("SSTF Disk Scheduling\n");
    printf("----------------------------------------------------------\n");
    printf("Initial Head Position : %d\n\n", head);

    printf("%-20s %-20s %-15s\n", "Current Position", "Next Position", "Seek Distance");
    printf("----------------------------------------------------------\n");

    /* Repeat REQUESTS times; each iteration serves one unvisited request */
    for (int count = 0; count < REQUESTS; count++) {
        int idx = -1;
        int minSeek = 1000000;

        /* Find unvisited request nearest to current head */
        for (int i = 0; i < REQUESTS; i++) {
            if (!visited[i]) {
                int seek = abs(queue[i] - current);
                if (seek < minSeek) {
                    minSeek = seek;
                    idx = i;
                }
            }
        }

        visited[idx] = 1; /* Mark request as serviced so it is not chosen again */
        totalSeek += minSeek;
        printf("%-20d %-20d %-15d\n", current, queue[idx], minSeek);
        current = queue[idx];
    }

    printf("----------------------------------------------------------\n");
    printf("Total Seek Distance   : %d cylinders\n", totalSeek);
    printf("Average Seek Distance : %.2f cylinders/request\n", (float)totalSeek / REQUESTS);
    return 0;
}
