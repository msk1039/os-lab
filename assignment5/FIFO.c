/*
============================================================
FIFO Page Replacement
------------------------------------------------------------
Idea:
- Keep pages in memory frames.
- On page fault with full frames, remove the oldest loaded page
  (the one that came first).

Detailed example (3 frames):
- Reference string: 1, 2, 3, 4, 1
- Load 1 -> [1, -, -]
- Load 2 -> [1, 2, -]
- Load 3 -> [1, 2, 3]
- Page 4 causes fault with full frames, replace oldest page (1) -> [4, 2, 3]
- Page 1 again causes fault, next oldest now is 2 -> [4, 1, 3]
============================================================
*/

#include <stdio.h>
#include <stdbool.h>

#define FRAMES 3
#define REF_LEN 15

int refStr[REF_LEN] = {1, 2, 3, 1, 2, 3, 1, 2, 3, 4, 5, 4, 5, 4, 5};
int frames[FRAMES];

/* Linear lookup for current page among active frame count */
bool inFrames(int page, int count) {
    for (int i = 0; i < count; i++) {
        if (frames[i] == page) return true;
    }
    return false;
}

int main(void) {
    int count = 0;
    int faults = 0;
    int replacePtr = 0; /* Points to oldest frame for FIFO replacement */

    printf("%-6s %-8s %-20s %s\n", "Step", "Page", "Frames", "Status");
    printf("----------------------------------------------------------\n");

    for (int t = 0; t < REF_LEN; t++) {
        int page = refStr[t];
        bool fault = false;

        if (!inFrames(page, count)) {
            fault = true;
            faults++;

            if (count < FRAMES) {
                /* Free frame available */
                frames[count++] = page;
            } else {
                /* Replace oldest frame */
                frames[replacePtr] = page;
                /* Move pointer circularly to next oldest frame */
                replacePtr = (replacePtr + 1) % FRAMES;
            }
        }

        printf("%-6d %-8d [ ", t + 1, page);
        for (int i = 0; i < count; i++) printf("%d ", frames[i]);
        for (int i = count; i < FRAMES; i++) printf("- ");
        printf("]   %s\n", fault ? "PAGE FAULT" : "HIT");
    }

    printf("----------------------------------------------------------\n");
    printf("Total Page Faults : %d\n", faults);
    printf("Total Page Hits   : %d\n", REF_LEN - faults);
    return 0;
}
