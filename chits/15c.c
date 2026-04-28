/*
Problem Statement (Chit 15):
Implement the C/C++ program for LOOK disk scheduling with a given request queue (at least 15 requests) and initial disk head position. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
LOOK Disk Scheduling
------------------------------------------------------------
Idea:
- Sort requests.
- Move in one direction and serve requests.
- Reverse at last request in that direction, not at disk end.

Detailed example:
- Head=50, sorted queue: 11 34 41 60 79 92
- Moving right: serve 60,79,92 then reverse to 41,34,11.
============================================================
*/

#include <stdio.h>

#define MAX 100

int absVal(int x) { return x < 0 ? -x : x; }

void sort(int a[], int n) {
    /* Simple bubble sort to order requests from low to high cylinder number. */
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
            }
}

int main(void) {
    /* `split` divides left-side and right-side requests around head. */
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

    sort(q, n);
    int split = 0;
    /* First index whose cylinder is >= current head. */
    while (split < n && q[split] < head) split++;

    current = head;
    printf("\nCurrent\tNext\tSeek\n");
    /* Move right first and service all right-side requests in ascending order. */
    for (int i = split; i < n; i++) {
        int seek = absVal(q[i] - current);
        total += seek;
        printf("%d\t%d\t%d\n", current, q[i], seek);
        current = q[i];
    }
    /* Reverse direction and service left-side requests in descending order. */
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
