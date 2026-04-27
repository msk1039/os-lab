/*
Problem Statement (Chit 8):
Implement the C/C++ program to simulate CPU Scheduling Algorithm priority (non-preemptive) with different arrival times. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
Non-Preemptive Priority CPU Scheduling
------------------------------------------------------------
Idea:
- Smaller priority number means higher priority.
- Among arrived processes, choose highest priority.
- Once selected, process runs until completion.

Detailed example:
- P1(AT=0,BT=4,PR=3), P2(AT=1,BT=2,PR=1)
- P1 starts first because only P1 is available at t=0.
- P2 has higher priority but waits until P1 completes.
============================================================
*/

#include <stdio.h>

#define MAX 30

int main(void) {
    int n, at[MAX], bt[MAX], pr[MAX], st[MAX], ct[MAX], wt[MAX], tat[MAX], done[MAX] = {0};
    int time = 0, completed = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("P%d arrival, burst, priority: ", i + 1);
        scanf("%d%d%d", &at[i], &bt[i], &pr[i]);
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

        st[p] = time;
        ct[p] = time + bt[p];
        tat[p] = ct[p] - at[p];
        wt[p] = tat[p] - bt[p];
        time = ct[p];
        done[p] = 1;
        completed++;
    }

    printf("\nPID\tAT\tBT\tPR\tST\tCT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], bt[i], pr[i], st[i], ct[i], wt[i], tat[i]);
    }

    return 0;
}

