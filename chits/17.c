/*
Problem Statement (Chit 17):
Implement the C/C++ program for SCAN disk scheduling with a given request queue (at least 15 requests) and initial disk head position. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
SCAN Disk Scheduling
------------------------------------------------------------
Idea:
- Move in one direction and serve requests on the way.
- Continue to physical disk end, then reverse.

Detailed example:
- Disk range 0-199, head=50, moving right.
- Serve right requests, go to 199, then serve left requests.
============================================================
*/

#include <stdio.h>

#define MAX 100

int absVal(int x) { return x < 0 ? -x : x; }

void sort(int a[], int n) {
    /* Sorting allows linear sweep in SCAN direction. */
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
            }
}

int main(void) {
    /* diskSize is required to model travel till physical end cylinder. */
    int n, head, diskSize, current, q[MAX], total = 0;

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
    printf("Enter disk size: ");
    scanf("%d", &diskSize);

    sort(q, n);
    int split = 0;
    /* Requests [0..split-1] are left of head; [split..n-1] are right side. */
    while (split < n && q[split] < head) split++;

    current = head;
    printf("\nCurrent\tNext\tSeek\n");
    /* Sweep right servicing all right-side requests first. */
    for (int i = split; i < n; i++) {
        int seek = absVal(q[i] - current);
        total += seek;
        printf("%d\t%d\t%d\n", current, q[i], seek);
        current = q[i];
    }

    /* SCAN must touch end cylinder before reversing direction. */
    int end = diskSize - 1;
    total += absVal(end - current);
    printf("%d\t%d\t%d\n", current, end, absVal(end - current));
    current = end;

    /* Reverse sweep to service remaining left-side requests. */
    for (int i = split - 1; i >= 0; i--) {
        int seek = absVal(q[i] - current);
        total += seek;
        printf("%d\t%d\t%d\n", current, q[i], seek);
        current = q[i];
    }

    printf("\nTotal seek distance: %d\n", total);
    printf("Average seek       : %.2f\n", (float)total / n);
    return 0;
}
