/*
============================================================
Round Robin (RR) Scheduling
------------------------------------------------------------
Idea:
- CPU time is divided into equal time quantum slices.
- Ready processes execute in circular queue order.
- If a process is not finished in one quantum, it goes to queue end.

Detailed example (q=2):
- Input:
  P1(AT=0,BT=5), P2(AT=0,BT=3), P3(AT=1,BT=2)
- Queue evolution:
  t=0: [P1,P2]
  run P1 for 2 -> rem=3, queue [P2,P3,P1]
  run P2 for 2 -> rem=1, queue [P3,P1,P2]
  run P3 for 2 -> completes, queue [P1,P2]
  run P1 for 2, then P2 for 1, then P1 for 1.
- Every process gets CPU repeatedly, which improves response fairness.
============================================================
*/

#include <stdio.h>

#define N 7
#define Q 2
#define MAXQ 100

int main(void) {
    int at[N]  = {0, 1, 2, 3, 4, 5, 6};
    int bt[N]  = {5, 4, 2, 1, 6, 3, 2};
    int pid[N] = {1, 2, 3, 4, 5, 6, 7};

    int rem[N], st[N], et[N], wt[N], tat[N];
    int started[N] = {0};
    int inQueue[N] = {0};

    int queue[MAXQ]; /* Simple linear queue with front/rear indices */
    int front = 0, rear = 0;

    for (int i = 0; i < N; i++) {
        rem[i] = bt[i];
        st[i] = -1;
    }

    int completed = 0;
    int time = 0;

    /* Add processes that arrive at time 0 */
    for (int i = 0; i < N; i++) {
        if (at[i] == 0) {
            queue[rear++] = i;
            inQueue[i] = 1;
        }
    }

    /* Continue cycles until all processes complete */
    while (completed < N) {
        /* If queue is empty, CPU is idle; jump to next time */
        if (front == rear) {
            time++;
            for (int i = 0; i < N; i++) {
                if (!inQueue[i] && rem[i] > 0 && at[i] <= time) {
                    queue[rear++] = i;
                    inQueue[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++];
        inQueue[idx] = 0;

        if (!started[idx]) {
            st[idx] = time;
            started[idx] = 1;
        }

        /* Process can execute at most one quantum in each turn */
        int run = (rem[idx] < Q) ? rem[idx] : Q;
        rem[idx] -= run;
        time += run;

        /* Add newly arrived processes to queue */
        for (int i = 0; i < N; i++) {
            if (!inQueue[i] && rem[i] > 0 && at[i] <= time) {
                queue[rear++] = i;
                inQueue[i] = 1;
            }
        }

        if (rem[idx] > 0) {
            /* Process not finished, add back to queue end */
            queue[rear++] = idx;
            inQueue[idx] = 1;
        } else {
            et[idx] = time;
            completed++;
        }
    }

    float avgWT = 0.0f, avgTAT = 0.0f;
    printf("PID\tAT\tBT\tST\tET\tWT\tTAT\n");
    for (int i = 0; i < N; i++) {
        tat[i] = et[i] - at[i]; /* Turnaround = completion - arrival */
        wt[i] = tat[i] - bt[i];

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               pid[i], at[i], bt[i], st[i], et[i], wt[i], tat[i]);
        avgWT += wt[i];
        avgTAT += tat[i];
    }

    printf("Time Quantum = %d\n", Q);
    printf("Avg WT  = %.2f\n", avgWT / N);
    printf("Avg TAT = %.2f\n", avgTAT / N);
    return 0;
}
