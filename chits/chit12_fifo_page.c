/*
Problem Statement (Chit 12):
Implement the C/C++ program for FIFO Page Replacement Algorithm with n frames as an input. Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
FIFO Page Replacement
------------------------------------------------------------
Idea:
- User enters number of frames and reference string.
- On page fault with full memory, replace page loaded earliest.
- A circular pointer tracks the next frame to replace.

Detailed example:
- Frames=3, reference: 1 2 3 4 1
- 1,2,3 fill frames.
- 4 replaces 1 because 1 entered first.
- 1 later replaces 2 because 2 is now oldest.
============================================================
*/

#include <stdio.h>

#define MAX 50

int main(void) {
    int n, f, ref[MAX], frame[MAX];
    int count = 0, faults = 0, ptr = 0;

    printf("Enter number of frames: ");
    scanf("%d", &f);
    printf("Enter reference string length: ");
    scanf("%d", &n);
    printf("Enter reference string:\n");
    for (int i = 0; i < n; i++) scanf("%d", &ref[i]);

    for (int i = 0; i < f; i++) frame[i] = -1;

    printf("\nPage\tFrames\t\tStatus\n");
    for (int t = 0; t < n; t++) {
        int page = ref[t], found = 0;

        for (int i = 0; i < f; i++) {
            if (frame[i] == page) found = 1;
        }

        if (!found) {
            faults++;
            if (count < f) {
                frame[count++] = page;
            } else {
                frame[ptr] = page;
                ptr = (ptr + 1) % f;
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

