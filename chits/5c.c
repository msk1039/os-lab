/*
Problem Statement (Chit 5):
Implement the C/C++ program to simulate CPU Scheduling Algorithm Shortest Job Remaining First (preemptive) with different arrival times. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
Shortest Remaining Time First CPU Scheduling
------------------------------------------------------------
Idea:
- Preemptive version of Shortest Job First.
- At every time unit, choose the arrived process with least remaining time.

Detailed example:
- P1(AT=0,BT=8), P2(AT=1,BT=4), P3(AT=2,BT=2)
- P1 starts first.
- P2 arrives with smaller remaining time and preempts P1.
- P3 arrives with still smaller time and preempts P2.
============================================================
*/

#include <stdio.h>

#define MAX 30

int main(void) {
    /* All process attributes are maintained in parallel arrays by index. */
    int n, at[MAX], bt[MAX], rem[MAX], st[MAX], ct[MAX], wt[MAX], tat[MAX];
    int started[MAX] = {0}, done[MAX] = {0};
    int time = 0, completed = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("P%d arrival and burst: ", i + 1);
        scanf("%d%d", &at[i], &bt[i]);
        rem[i] = bt[i];
        st[i] = -1;
    }

    /* One-step CPU simulation to allow switching process every unit time. */
    while (completed < n) {
        int p = -1, shortest = 999999;

        /* Choose arrived process with minimum remaining execution time. */
        for (int i = 0; i < n; i++) {
            if (!done[i] && at[i] <= time && rem[i] < shortest) {
                shortest = rem[i];
                p = i;
            }
        }

        /* If no process has arrived yet, advance time without execution. */
        if (p == -1) {
            time++;
            continue;
        }

        if (!started[p]) {
            st[p] = time;
            started[p] = 1;
        }

        /* Execute selected process for one time unit only. */
        rem[p]--;
        time++;

        if (rem[p] == 0) {
            /* Mark completion and freeze completion time for metrics. */
            done[p] = 1;
            ct[p] = time;
            completed++;
        }
    }

    printf("\nPID\tAT\tBT\tST\tCT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        /* Turnaround and waiting are derived from completion values. */
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], bt[i], st[i], ct[i], wt[i], tat[i]);
    }

    return 0;
}
