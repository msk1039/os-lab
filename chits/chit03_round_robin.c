/*
Problem Statement (Chit 3):
Implement the C/C++ program to simulate CPU Scheduling Algorithm round robin (preemptive) with different arrival times. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
Round Robin CPU Scheduling
------------------------------------------------------------
Idea:
- Each process receives CPU for a fixed time quantum.
- If not complete, it goes back to the ready queue.
- Different arrival times are handled by adding arrived processes to queue.

Detailed example:
- Quantum=2, P1(AT=0,BT=5), P2(AT=1,BT=3)
- P1 runs 0-2, P2 runs 2-4, P1 runs 4-6,
  P2 runs 6-7, P1 runs 7-8.
============================================================
*/

#include <stdio.h>

#define MAX 30
#define QMAX 200

int main(void) {
    int n, quantum;
    int at[MAX], bt[MAX], rem[MAX], st[MAX], ct[MAX], wt[MAX], tat[MAX];
    int started[MAX] = {0}, inQueue[MAX] = {0};
    int q[QMAX], front = 0, rear = 0;
    int time = 0, completed = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    for (int i = 0; i < n; i++) {
        printf("P%d arrival time and burst time: ", i + 1);
        scanf("%d%d", &at[i], &bt[i]);
        rem[i] = bt[i];
        st[i] = -1;
    }

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && rem[i] > 0 && at[i] <= time) {
                q[rear++] = i;
                inQueue[i] = 1;
            }
        }

        if (front == rear) {
            time++;
            continue;
        }

        int p = q[front++];
        inQueue[p] = 0;

        if (!started[p]) {
            st[p] = time;
            started[p] = 1;
        }

        int run = (rem[p] < quantum) ? rem[p] : quantum;
        rem[p] -= run;
        time += run;

        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && rem[i] > 0 && at[i] <= time && i != p) {
                q[rear++] = i;
                inQueue[i] = 1;
            }
        }

        if (rem[p] > 0) {
            q[rear++] = p;
            inQueue[p] = 1;
        } else {
            ct[p] = time;
            completed++;
        }
    }

    printf("\nPID\tAT\tBT\tST\tCT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], bt[i], st[i], ct[i], wt[i], tat[i]);
    }

    return 0;
}

