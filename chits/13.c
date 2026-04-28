

#include <stdio.h>

#define MAX 100

int absVal(int x) {
    return x < 0 ? -x : x;
}

int main(void) {

    int n, head, current, q[MAX], total = 0;

    printf("Enter number of requests (at least 15): ");
    scanf("%d", &n);
    if (n < 15) {
        printf("Please enter at least 15 requests.\n");
        return 0;
    }

    printf("Enter request queue:\n");
    for (int i = 0; i < n; i++) scanf("%d", &q[i]);
    printf("Enter initial head position: ");
    scanf("%d", &head);


    current = head;
    printf("\nCurrent\tNext\tSeek\n");

    for (int i = 0; i < n; i++) {
        int seek = absVal(q[i] - current);
        total += seek;
        printf("%d\t%d\t%d\n", current, q[i], seek);
        current = q[i];
    }

    printf("\nTotal seek distance: %d\n", total);
    printf("Average seek       : %.2f\n", (float)total / n);
    return 0;
}
