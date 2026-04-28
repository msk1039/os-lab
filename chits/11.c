/*
Problem Statement (Chit 11):
Implement the C/C++ program for Optimal Page Replacement Algorithm with n frames as an input. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
Optimal Page Replacement
------------------------------------------------------------
Idea:
- User enters number of frames and reference string.
- On page fault, replace page whose next use is farthest in future.
- If a page is never used again, replace it immediately.

Detailed example:
- Frames=3, reference: 1 2 3 4 1 2
- Frames before 4 are 1,2,3.
- Looking ahead, 3 is never used again, so replace 3 with 4.
============================================================
*/

#include <stdio.h>

#define MAX 50

int main(void) {
    /* `frame[]` stores currently loaded pages. */
    int n, f, ref[MAX], frame[MAX];
    int count = 0, faults = 0;

    printf("Enter number of frames: ");
    scanf("%d", &f);
    printf("Enter reference string length: ");
    scanf("%d", &n);
    printf("Enter reference string:\n");
    for (int i = 0; i < n; i++) scanf("%d", &ref[i]);

    /* Initialize all frames as empty slots. */
    for (int i = 0; i < f; i++) frame[i] = -1;

    printf("\nPage\tFrames\t\tStatus\n");
    for (int t = 0; t < n; t++) {
        int page = ref[t], found = 0;

        /* Hit test: check if page already exists in any frame. */
        for (int i = 0; i < f; i++) {
            if (frame[i] == page) found = 1;
        }

        if (!found) {
            faults++;

            if (count < f) {
                /* If free frame exists, no replacement is needed yet. */
                frame[count++] = page;
            } else {
                /* Otherwise choose victim with farthest future use. */
                int victim = 0, farthest = -1;

                for (int i = 0; i < f; i++) {
                    int next = -1;
                    /* Scan future references to find next use of frame[i]. */
                    for (int j = t + 1; j < n; j++) {
                        if (ref[j] == frame[i]) {
                            next = j;
                            break;
                        }
                    }

                    if (next == -1) {
                        /* Never used again: best page to replace. */
                        victim = i;
                        break;
                    }
                    if (next > farthest) {
                        farthest = next;
                        victim = i;
                    }
                }
                frame[victim] = page;
            }
        }

        printf("%d\t", page);
        for (int i = 0; i < f; i++) {
            if (frame[i] == -1) printf("- ");
            else printf("%d ", frame[i]);
        }
        printf("\t%s\n", found ? "Hit" : "Fault");
    }

    printf("\nTotal page faults: %d\n", faults);
    printf("Total page hits  : %d\n", n - faults);
    return 0;
}
