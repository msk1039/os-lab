/*
Problem Statement (Chit 13):
Implement the C/C++ program for FCFS disk scheduling with a given request queue (at least 15 requests) and initial disk head position. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
FCFS Disk Scheduling
------------------------------------------------------------
Idea:
- Serve disk requests in the exact order entered by user.
- Request queue must contain at least 15 requests.

Detailed example:
- Head=50, queue: 82 170 43
- Movement: 50->82 (32), 82->170 (88), 170->43 (127).
- Total seek = 247.
============================================================
*/

#include <stdio.h>

#define MAX 100

int absVal(int x) {
    return x < 0 ? -x : x;
}

int main(void) {
    /* q[] stores request queue exactly in user-entered order. */
    int n, head, current, q[MAX], total = 0;

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

    /* Head begins at initial position provided by user. */
    current = head;
    printf("\nCurrent\tNext\tSeek\n");
    /* FCFS: serve requests in strict queue order. */
    for (int i = 0; i < n; i++) {
        int seek = absVal(q[i] - current);
        total += seek;
        printf("%d\t%d\t%d\n", current, q[i], seek);
        current = q[i];
    }

    printf("\nTotal seek distance: %d\n", total);
    printf("Average seek       : %.2f\n", (float)total / n);
    return 0;
}
