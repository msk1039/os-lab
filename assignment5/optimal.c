/*
============================================================
Optimal Page Replacement
------------------------------------------------------------
Idea:
- On page fault with full frames, replace the page whose next use
  is farthest in the future (or never used again).
- This gives minimum possible page faults for a given reference string.

Detailed example (3 frames):
- Reference string: 1, 2, 3, 4, 1, 2
- Frames before page 4: [1,2,3]
- Look ahead:
  next use of 1 is near, next use of 2 is near, 3 is never used again.
- Best replacement is 3, so load 4 by replacing 3.
- This look-ahead choice minimizes future faults for known reference string.
============================================================
*/

#include <stdio.h>
#include <stdbool.h>

#define FRAMES 3
#define REF_LEN 15

int refStr[REF_LEN] = {1, 2, 3, 1, 2, 3, 1, 2, 3, 4, 5, 4, 5, 4, 5};
int frames[FRAMES];

/* Return true if page is currently in loaded frames */
bool inFrames(int page, int count) {
    for (int i = 0; i < count; i++) {
        if (frames[i] == page) return true;
    }
    return false;
}

/* Choose frame to replace using Optimal look-ahead */
int chooseOptimalVictim(int currentPos) {
    int victim = 0;
    int farthestNextUse = -1;

    for (int i = 0; i < FRAMES; i++) {
        int nextUse = -1;

        /* Search future reference string for next use of each frame page */
        for (int j = currentPos + 1; j < REF_LEN; j++) {
            if (refStr[j] == frames[i]) {
                nextUse = j;
                break;
            }
        }

        /* Best victim: page never needed again */
        if (nextUse == -1) return i;

        if (nextUse > farthestNextUse) {
            farthestNextUse = nextUse;
            victim = i;
        }
    }

    return victim;
}

int main(void) {
    int count = 0;
    int faults = 0;

    printf("%-6s %-8s %-20s %s\n", "Step", "Page", "Frames", "Status");
    printf("----------------------------------------------------------\n");

    for (int t = 0; t < REF_LEN; t++) {
        int page = refStr[t];
        bool fault = false;

        if (!inFrames(page, count)) {
            fault = true;
            faults++;

            if (count < FRAMES) {
                frames[count++] = page;
            } else {
                int idx = chooseOptimalVictim(t); /* choose farthest-future page */
                frames[idx] = page;
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
