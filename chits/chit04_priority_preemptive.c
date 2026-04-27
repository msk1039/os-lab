/*
Problem Statement (Chit 4):
Implement the C/C++ program to simulate CPU Scheduling Algorithm Priority (preemptive) with different arrival times. . Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
Preemptive Priority CPU Scheduling
------------------------------------------------------------
Idea:
- Smaller priority number means higher priority.
- At every time unit, choose the arrived process with highest priority.
- If a higher-priority process arrives, current process is preempted.

Detailed example:
- P1(AT=0,BT=5,PR=3), P2(AT=1,BT=2,PR=1)
- P1 starts at t=0.
- At t=1, P2 arrives and preempts P1 because 1 is higher priority than 3.
============================================================
*/

#include <stdio.h>

#define MAX 30

int main(void) {
    int n, at[MAX], bt[MAX], pr[MAX], rem[MAX], st[MAX], ct[MAX], wt[MAX], tat[MAX];
    int started[MAX] = {0}, done[MAX] = {0};
    int time = 0, completed = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("P%d arrival, burst, priority: ", i + 1);
        scanf("%d%d%d", &at[i], &bt[i], &pr[i]);
        rem[i] = bt[i];
        st[i] = -1;
    }

    while (completed < n) {
        int p = -1, best = 999999;

        for (int i = 0; i < n; i++) {
            if (!done[i] && at[i] <= time && pr[i] < best) {
                best = pr[i];
                p = i;
            }
        }

        if (p == -1) {
            time++;
            continue;
        }

        if (!started[p]) {
            st[p] = time;
            started[p] = 1;
        }

        rem[p]--;
        time++;

        if (rem[p] == 0) {
            done[p] = 1;
            ct[p] = time;
            completed++;
        }
    }

    printf("\nPID\tAT\tBT\tPR\tST\tCT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], bt[i], pr[i], st[i], ct[i], wt[i], tat[i]);
    }

    return 0;
}

