/*
============================================================
LRU (Least Recently Used) Page Replacement
------------------------------------------------------------
Idea:
- On page fault with full frames, replace the page that was used
  farthest in the past (least recently used).

Detailed example (3 frames):
- Reference string: 1, 2, 3, 1, 4
- After first 3 pages: frames [1,2,3]
- Access page 1: now recent order makes page 2 oldest usage.
- Page 4 fault occurs with full frames, so replace least recently used page 2.
- New frames become [1,4,3] (order in array may vary by index).
============================================================
*/

#include <stdio.h>
#include <stdbool.h>

#define FRAMES 3
#define REF_LEN 15

int refStr[REF_LEN] = {1, 2, 3, 1, 2, 3, 1, 2, 3, 4, 5, 4, 5, 4, 5};
int frames[FRAMES];
int lastUsed[FRAMES];

/* Return frame index of page if present, else -1 */
int findPage(int page, int count) {
    for (int i = 0; i < count; i++) {
        if (frames[i] == page) return i;
    }
    return -1;
}

/* Find frame index whose page was used least recently */
int findLRUIndex(void) {
    int idx = 0;
    for (int i = 1; i < FRAMES; i++) {
        /* Smaller timestamp means the page was used earlier (older) */
        if (lastUsed[i] < lastUsed[idx]) idx = i;
    }
    return idx;
}

int main(void) {
    int count = 0;
    int faults = 0;

    printf("%-6s %-8s %-20s %s\n", "Step", "Page", "Frames", "Status");
    printf("----------------------------------------------------------\n");

    for (int t = 0; t < REF_LEN; t++) {
        int page = refStr[t];
        bool fault = false;

        int idx = findPage(page, count);
        if (idx != -1) {
            /* Hit: only update last-used time */
            lastUsed[idx] = t;
        } else {
            fault = true;
            faults++;

            if (count < FRAMES) {
                /* Frame available: fill directly and mark last-used time */
                frames[count] = page;
                lastUsed[count] = t;
                count++;
            } else {
                /* Replace the frame whose page was touched farthest in the past */
                int replaceIdx = findLRUIndex();
                frames[replaceIdx] = page;
                lastUsed[replaceIdx] = t;
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
