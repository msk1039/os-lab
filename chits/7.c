/*
Problem Statement (Chit 7):
Implement the C/C++ program to simulate CPU Scheduling Algorithm shortest job first (non-preemptive) with different arrival times. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
Shortest Job First CPU Scheduling
------------------------------------------------------------
Idea:
- Non-preemptive scheduling.
- Among arrived processes, choose the smallest burst time.

Detailed example:
- P1(AT=0,BT=7), P2(AT=2,BT=4), P3(AT=4,BT=1)
- P1 runs first because it is the only arrived process.
- At time 7, P2 and P3 are ready, so P3 runs before P2.
============================================================
*/

#include <stdio.h>

#define MAX 30

int main(void) {
    /* done[] marks processes already completed in previous iterations. */
    int n, at[MAX], bt[MAX], st[MAX], ct[MAX], wt[MAX], tat[MAX], done[MAX] = {0};
    int time = 0, completed = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("P%d arrival and burst: ", i + 1);
        scanf("%d%d", &at[i], &bt[i]);
    }

    /* Repeat selecting shortest available job until all jobs are done. */
    while (completed < n) {
        int p = -1, best = 999999;

        /* Candidate set: arrived and unfinished processes only. */
        for (int i = 0; i < n; i++) {
            if (!done[i] && at[i] <= time && bt[i] < best) {
                best = bt[i];
                p = i;
            }
        }

        /* If no job is ready at this time, advance to next time unit. */
        if (p == -1) {
            time++;
            continue;
        }

        /* Non-preemptive execution: selected job runs until completion. */
        st[p] = time;
        ct[p] = time + bt[p];
        tat[p] = ct[p] - at[p];
        wt[p] = tat[p] - bt[p];
        time = ct[p];
        done[p] = 1;
        completed++;
    }

    printf("\nPID\tAT\tBT\tST\tCT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], bt[i], st[i], ct[i], wt[i], tat[i]);
    }

    return 0;
}
