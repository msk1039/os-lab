/*
============================================================
Priority Scheduling - Non-Preemptive
------------------------------------------------------------
Idea:
- Each process has a priority value.
- Smaller number means higher priority.
- Among arrived processes, pick highest priority.
- Once selected, process runs till completion.

Detailed example:
- Input:
  P1(AT=0,BT=4,PR=3), P2(AT=1,BT=2,PR=1), P3(AT=2,BT=3,PR=2)
- t=0: only P1 has arrived, so run P1 fully (0->4)
- t=4: P2 and P3 are ready; smaller priority number wins, so pick P2
- then run P3.
- Since this is non-preemptive, arriving high-priority jobs wait
  until currently running job completes.
============================================================
*/

#include <stdio.h>

#define N 7

int main(void) {
    int at[N]   = {0, 1, 2, 3, 4, 5, 6};
    int bt[N]   = {4, 3, 5, 2, 4, 6, 3};
    int pr[N]   = {3, 1, 4, 2, 5, 2, 1}; /* lower value = higher priority */
    int pid[N]  = {1, 2, 3, 4, 5, 6, 7};

    int st[N], et[N], wt[N], tat[N];
    int done[N] = {0};

    int completed = 0;
    int time = 0;

    /* Keep selecting next highest-priority arrived process */
    while (completed < N) {
        int idx = -1;
        int bestPr = 1000000;

        /* Find highest-priority process among arrived */
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

        /* Non-preemptive: selected process runs until burst finishes */
        st[idx] = time;
        et[idx] = time + bt[idx];
        tat[idx] = et[idx] - at[idx];
        wt[idx] = tat[idx] - bt[idx];

        time = et[idx];
        done[idx] = 1;
        completed++;
    }

    float avgWT = 0.0f, avgTAT = 0.0f;
    printf("PID\tAT\tBT\tPR\tST\tET\tWT\tTAT\n");
    for (int i = 0; i < N; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               pid[i], at[i], bt[i], pr[i], st[i], et[i], wt[i], tat[i]);
        avgWT += wt[i];
        avgTAT += tat[i];
    }

    printf("Avg WT  = %.2f\n", avgWT / N);
    printf("Avg TAT = %.2f\n", avgTAT / N);
    return 0;
}
