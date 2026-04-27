/*
============================================================
SRTF (Shortest Remaining Time First) - Preemptive Scheduling
------------------------------------------------------------
Idea:
- At every time unit, run the arrived process that has the
  smallest remaining burst time.
- If a shorter job arrives, current process is preempted.

Detailed example:
- Input:
  P1(AT=0,BT=8), P2(AT=1,BT=4), P3(AT=2,BT=2)
- t=0: run P1 (remaining 7)
- t=1: P2 arrives, 4 < 7, preempt P1 and run P2
- t=2: P3 arrives, 2 < P2 remaining(3), preempt P2 and run P3
- Scheduler always chooses smallest remaining time at each unit.
============================================================
*/

#include <stdio.h>

#define N 7

int main(void) {
    int at[N]  = {2, 0, 3, 7, 9, 5, 10};
    int bt[N]  = {3, 5, 1, 6, 1, 5, 1};
    int pid[N] = {1, 2, 3, 4, 5, 6, 7};

    int rem[N], st[N], et[N], wt[N], tat[N];
    int done[N] = {0};
    int started[N] = {0};

    for (int i = 0; i < N; i++) {
        rem[i] = bt[i];
        st[i] = -1;
    }

    int completed = 0;
    int time = 0;

    /* Simulate CPU execution one time unit at a time */
    while (completed < N) {
        int idx = -1;
        int bestRem = 1000000;

        /* Pick arrived process with smallest remaining time */
        for (int i = 0; i < N; i++) {
            if (!done[i] && at[i] <= time && rem[i] < bestRem) {
                bestRem = rem[i];
                idx = i;
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

        rem[idx]--; /* Execute selected process for exactly 1 unit */
        time++;

        if (rem[idx] == 0) {
            done[idx] = 1;
            et[idx] = time;
            completed++;
        }
    }

    float avgWT = 0.0f, avgTAT = 0.0f;
    printf("PID\tAT\tBT\tST\tET\tWT\tTAT\n");
    for (int i = 0; i < N; i++) {
        /* Standard post-processing after completion times are known */
        tat[i] = et[i] - at[i];
        wt[i] = tat[i] - bt[i];

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               pid[i], at[i], bt[i], st[i], et[i], wt[i], tat[i]);
        avgWT += wt[i];
        avgTAT += tat[i];
    }

    printf("Avg WT  = %.2f\n", avgWT / N);
    printf("Avg TAT = %.2f\n", avgTAT / N);
    return 0;
}
