/*
Problem Statement (Chit 14):
Implement the C/C++ program for SSTF disk scheduling with a given request queue (at least 15 requests) and initial disk head position. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
SSTF Disk Scheduling
------------------------------------------------------------
Idea:
- From current head, choose nearest pending request.
- Repeat until all requests are served.
- Request queue must contain at least 15 requests.

Detailed example:
- Head=50, pending: 41, 92, 34
- Nearest from 50 is 41, then nearest from 41 is 34.
============================================================
*/

#include <stdio.h>

#define MAX 100

int absVal(int x) {
    return x < 0 ? -x : x;
}

int main(void) {
    int n, head, current, q[MAX], done[MAX] = {0}, total = 0;

    printf("Enter number of requests (at least 15): ");
    scanf("%d", &n);
    if (n < 15) {
        printf("Please enter at least 15 requests.\n");
        return 0;
    }

    printf("Enter request queue:\n");
    for (int i = 0; i < n; i++) scanf("%d", &q[i]);
    printf("Enter initial head position: ");
    scanf("%d", &head);

    current = head;
    printf("\nCurrent\tNext\tSeek\n");
    for (int count = 0; count < n; count++) {
        int pos = -1, best = 999999;

        for (int i = 0; i < n; i++) {
            if (!done[i] && absVal(q[i] - current) < best) {
                best = absVal(q[i] - current);
                pos = i;
            }
        }

        done[pos] = 1;
        total += best;
        printf("%d\t%d\t%d\n", current, q[pos], best);
        current = q[pos];
    }

    printf("\nTotal seek distance: %d\n", total);
    printf("Average seek       : %.2f\n", (float)total / n);
    return 0;
}

