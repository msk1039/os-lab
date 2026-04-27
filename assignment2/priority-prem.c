/*
============================================================
Priority Scheduling - Preemptive
------------------------------------------------------------
Idea:
- Every time unit, run arrived process with highest priority
  (smallest priority number).
- If a higher-priority process arrives, current one is preempted.

Detailed example:
- Input:
  P1(AT=0,BT=5,PR=3), P2(AT=1,BT=2,PR=1), P3(AT=2,BT=3,PR=2)
- t=0: run P1.
- t=1: P2 arrives with higher priority (1 < 3), so preempt P1.
- t=2: P3 arrives, but P2 still has higher priority, so P2 continues.
- After P2 completes, scheduler compares remaining ready processes again.
============================================================
*/

#include <stdio.h>

#define N 7

int main(void) {
    int at[N]   = {0, 1, 2, 3, 4, 5, 6};
    int bt[N]   = {4, 3, 5, 2, 4, 6, 3};
    int pr[N]   = {3, 1, 4, 2, 5, 2, 1}; /* lower value = higher priority */
    int pid[N]  = {1, 2, 3, 4, 5, 6, 7};

    int rem[N], st[N], et[N], wt[N], tat[N];
    int done[N] = {0};
    int started[N] = {0};

    for (int i = 0; i < N; i++) {
        rem[i] = bt[i];
        st[i] = -1;
    }

    int completed = 0;
    int time = 0;

    /* Preemptive simulation: decide best process every time unit */
    while (completed < N) {
        int idx = -1;
        int bestPr = 1000000;

        /* Select highest-priority process among arrived */
        for (int i = 0; i < N; i++) {
            if (!done[i] && at[i] <= time) {
                if (pr[i] < bestPr) {
                    bestPr = pr[i];
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            time++; /* CPU idle */
            continue;
        }

        if (!started[idx]) {
            st[idx] = time;
            started[idx] = 1;
        }

        rem[idx]--; /* Run selected process for one CPU tick */
        time++;

        if (rem[idx] == 0) {
            done[idx] = 1;
            et[idx] = time;
            completed++;
        }
    }

    float avgWT = 0.0f, avgTAT = 0.0f;
    printf("PID\tAT\tBT\tPR\tST\tET\tWT\tTAT\n");
    for (int i = 0; i < N; i++) {
        tat[i] = et[i] - at[i];
        wt[i] = tat[i] - bt[i];

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               pid[i], at[i], bt[i], pr[i], st[i], et[i], wt[i], tat[i]);
        avgWT += wt[i];
        avgTAT += tat[i];
    }

    printf("Avg WT  = %.2f\n", avgWT / N);
    printf("Avg TAT = %.2f\n", avgTAT / N);
    return 0;
}
