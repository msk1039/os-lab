

#include <stdio.h>

#define MAX 50

int main(void) {

    int n, f, ref[MAX], frame[MAX], last[MAX];
    int count = 0, faults = 0;

    printf("Enter number of frames: ");
    scanf("%d", &f);
    printf("Enter reference string length: ");
    scanf("%d", &n);
    printf("Enter reference string:\n");
    for (int i = 0; i < n; i++) scanf("%d", &ref[i]);


    for (int i = 0; i < f; i++) {
        frame[i] = -1;
        last[i] = -1;
    }

    printf("\nPage\tFrames\t\tStatus\n");

    for (int t = 0; t < n; t++) {
        int page = ref[t], pos = -1;


        for (int i = 0; i < f; i++) {
            if (frame[i] == page) pos = i;
        }

        if (pos != -1) {

            last[pos] = t;
            printf("%d\t", page);
        } else {
            faults++;

            if (count < f) {

                frame[count] = page;
                last[count] = t;
                count++;
            } else {

                int lru = 0;
                for (int i = 1; i < f; i++) {
                    if (last[i] < last[lru]) lru = i;
                }
                frame[lru] = page;
                last[lru] = t;
            }
            printf("%d\t", page);
        }

        for (int i = 0; i < f; i++) {
            if (frame[i] == -1) printf("- ");
            else printf("%d ", frame[i]);
        }
        printf("\t%s\n", pos == -1 ? "Fault" : "Hit");
    }

    printf("\nTotal page faults: %d\n", faults);
    printf("Total page hits  : %d\n", n - faults);
    return 0;
}
