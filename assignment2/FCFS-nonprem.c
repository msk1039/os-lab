/*
============================================================
FCFS (First Come First Served) - Non-Preemptive Scheduling
------------------------------------------------------------
Idea:
- Process that arrives first gets CPU first.
- Once a process starts, it runs until completion.

Detailed example:
- Input:
  P1(AT=0,BT=3), P2(AT=1,BT=2), P3(AT=2,BT=1)
- Step 1: time=0, run P1 from 0 to 3
- Step 2: both P2 and P3 are ready, but FCFS keeps arrival order
          so P2 runs from 3 to 5
- Step 3: P3 runs from 5 to 6
- Waiting Times: P1=0, P2=2, P3=3
- Turnaround Times: P1=3, P2=4, P3=4
============================================================
*/

#include <stdio.h>

#define N 7

/*
Sort processes by arrival time so that FCFS order becomes explicit.
We swap arrival time, burst time, and PID together to keep rows aligned.
*/
void sortByArrival(int at[], int bt[], int pid[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (at[j] > at[j + 1]) {
                int t;
                t = at[j];   at[j] = at[j + 1];   at[j + 1] = t;
                t = bt[j];   bt[j] = bt[j + 1];   bt[j + 1] = t;
                t = pid[j];  pid[j] = pid[j + 1]; pid[j + 1] = t;
            }
        }
    }
}

int main(void) {
    int at[N]  = {2, 9, 3, 7, 8, 4, 25};
    int bt[N]  = {1, 2, 2, 3, 4, 10, 1};
    int pid[N] = {1, 2, 3, 4, 5, 6, 7};

    int st[N], et[N], wt[N], tat[N];
    int time = 0;

    /* FCFS executes processes in non-decreasing arrival-time order */
    sortByArrival(at, bt, pid, N);

    for (int i = 0; i < N; i++) {
        if (time < at[i]) {
            time = at[i]; /* CPU was idle until this process arrived */
        }

        /* Standard scheduling metrics */
        st[i] = time;              /* Start Time */
        et[i] = st[i] + bt[i];
        tat[i] = et[i] - at[i];
        wt[i] = tat[i] - bt[i];

        time = et[i];
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
