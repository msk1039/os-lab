/*
Problem Statement (Chit 18):
Implement the C/C++ program for CSCAN disk scheduling with a given request queue (at least 15 requests) and initial disk head position. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
C-SCAN Disk Scheduling
------------------------------------------------------------
Idea:
- Move in one direction only.
- After reaching disk end, jump to start and continue.

Detailed example:
- Disk range 0-199, head=50, moving right.
- Serve right requests, go to 199, jump to 0, serve remaining requests.
============================================================
*/

#include <stdio.h>

#define MAX 100

int absVal(int x) { return x < 0 ? -x : x; }

void sort(int a[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
            }
}

int main(void) {
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
    while (split < n && q[split] < head) split++;

    current = head;
    printf("\nCurrent\tNext\tSeek\n");
    for (int i = split; i < n; i++) {
        int seek = absVal(q[i] - current);
        total += seek;
        printf("%d\t%d\t%d\n", current, q[i], seek);
        current = q[i];
    }

    int end = diskSize - 1;
    total += absVal(end - current);
    printf("%d\t%d\t%d\n", current, end, absVal(end - current));
    current = end;

    total += current;
    printf("%d\t%d\t%d\n", current, 0, current);
    current = 0;

    for (int i = 0; i < split; i++) {
        int seek = absVal(q[i] - current);
        total += seek;
        printf("%d\t%d\t%d\n", current, q[i], seek);
        current = q[i];
    }

    printf("\nTotal seek distance: %d\n", total);
    printf("Average seek       : %.2f\n", (float)total / n);
    return 0;
}

