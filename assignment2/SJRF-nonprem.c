/*
============================================================
SJF (Shortest Job First) - Non-Preemptive Scheduling
------------------------------------------------------------
Idea:
- At each decision point, choose among arrived processes
  the one with the smallest burst time.
- Once selected, process runs to completion.

Detailed example:
- Input:
  P1(AT=0,BT=7), P2(AT=2,BT=4), P3(AT=4,BT=1)
- Step 1: t=0, only P1 is available, so P1 runs fully (0->7)
- Step 2: at t=7, both P2 and P3 are waiting; choose shortest BT (P3)
- Step 3: run P3 (7->8), then P2 (8->12)
- This reduces average waiting time compared to FCFS for many workloads.
============================================================
*/

#include <stdio.h>

#define N 7

int main(void) {
    int at[N]  = {2, 9, 3, 7, 8, 4, 25};
    int bt[N]  = {1, 2, 2, 3, 4, 10, 1};
    int pid[N] = {1, 2, 3, 4, 5, 6, 7};

    int st[N], et[N], wt[N], tat[N];
    int done[N] = {0};

    int completed = 0;
    int time = 0;

    /* Repeat until every process has completed once (non-preemptive) */
    while (completed < N) {
        int idx = -1;
        int bestBT = 1000000;

        /* Find arrived process with shortest burst time */
        for (int i = 0; i < N; i++) {
            if (!done[i] && at[i] <= time) {
                if (bt[i] < bestBT) {
                    bestBT = bt[i];
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            time++; /* CPU idle: no arrived process yet */
            continue;
        }

        /* Execute selected process fully because SJF here is non-preemptive */
        st[idx] = time;
        et[idx] = time + bt[idx];
        tat[idx] = et[idx] - at[idx];
        wt[idx] = tat[idx] - bt[idx];

        time = et[idx];
        done[idx] = 1;
        completed++;
    }

    float avgWT = 0.0f, avgTAT = 0.0f;
    printf("PID\tAT\tBT\tST\tET\tWT\tTAT\n");
    for (int i = 0; i < N; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               pid[i], at[i], bt[i], st[i], et[i], wt[i], tat[i]);
        avgWT += wt[i];
        avgTAT += tat[i];
    }

    printf("Avg WT  = %.2f\n", avgWT / N);
    printf("Avg TAT = %.2f\n", avgTAT / N);
    return 0;
}
