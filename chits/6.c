/*
Problem Statement (Chit 6):
Implement the C/C++ program to simulate CPU Scheduling Algorithm FCFS (non-preemptive) with different arrival times. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
FCFS CPU Scheduling
------------------------------------------------------------
Idea:
- Process with earliest arrival time gets CPU first.
- Non-preemptive: once started, process completes before next begins.

Detailed example:
- P1(AT=0,BT=3), P2(AT=1,BT=2), P3(AT=2,BT=1)
- Order is P1 -> P2 -> P3.
- P2 waits until P1 completes at time 3.
============================================================
*/

#include <stdio.h>

#define MAX 30

int main(void) {
    /* pid[] lets us keep original process ID even after sorting by arrival. */
    int n, at[MAX], bt[MAX], pid[MAX], st[MAX], ct[MAX], wt[MAX], tat[MAX];
    int time = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        pid[i] = i + 1;
        printf("P%d arrival and burst: ", i + 1);
        scanf("%d%d", &at[i], &bt[i]);
    }

    /* Sort by arrival time because FCFS always respects arrival order. */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (at[j] > at[j + 1]) {
                int t;
                t = at[j]; at[j] = at[j + 1]; at[j + 1] = t;
                t = bt[j]; bt[j] = bt[j + 1]; bt[j + 1] = t;
                t = pid[j]; pid[j] = pid[j + 1]; pid[j + 1] = t;
            }
        }
    }

    /* Compute schedule timeline from left to right. */
    for (int i = 0; i < n; i++) {
        /* If next process has not arrived yet, CPU stays idle until arrival. */
        if (time < at[i]) time = at[i];

        /* Standard schedule metrics. */
        st[i] = time;
        ct[i] = time + bt[i];
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        time = ct[i];
    }

    printf("\nPID\tAT\tBT\tST\tCT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], st[i], ct[i], wt[i], tat[i]);
    }

    return 0;
}
