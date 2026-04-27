/*
============================================================
Fork Demo: Parent + Child Sorting
------------------------------------------------------------
Idea:
- Parent and child both sort the same input independently after fork().
- Parent uses Bubble Sort, child uses Selection Sort.
- Parent waits for child to avoid zombie process.

Detailed example:
- Input array: 9 4 7 1
- `fork()` creates two processes with separate address spaces.
- Child sorts its copy using Selection Sort -> 1 4 7 9.
- Parent sorts its own copy using Bubble Sort -> 1 4 7 9.
- Parent calls `wait()` so child exit status can be collected cleanly.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


// Function to perform Bubble Sort (Used by Parent)
void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function to perform Selection Sort (Used by Child)
void selectionSort(int arr[], int n) {
    int i, j, minIdx, temp;
    for (i = 0; i < n - 1; i++) {
        minIdx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        temp = arr[minIdx];
        arr[minIdx] = arr[i];
        arr[i] = temp;
    }
}

// Utility function to print the array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int n;

    printf("Enter number of integers: ");
    if (scanf("%d", &n) != 1) return 1;

    int arr[n];
    int arr_copy[n];

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        printf(" Element[%d]: ", i);
        scanf("%d", &arr[i]);
        arr_copy[i] = arr[i]; // Make a copy for the child process
    }

    printf("\n[MAIN] Original Array: ");
    printArray(arr, n);

    /*
    fork():
    - Returns 0 in child process.
    - Returns child's PID (>0) in parent process.
    - Returns -1 on failure.
    */
    pid_t pid = fork(); // Create child process

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // --- CHILD PROCESS ---
        printf("\n[CHILD] PID = %d, Parent PID = %d\n", getpid(), getppid());
        printf("[CHILD] Sorting using Selection Sort...\n");
        selectionSort(arr_copy, n);

        printf("[CHILD] Sorted Array (Selection Sort): ");
        printArray(arr_copy, n);

        printf("[CHILD] Child process finished.\n");
        exit(0);
    }
    else {
        // --- PARENT PROCESS ---
        printf("\n[PARENT] PID = %d, Child PID = %d\n", getpid(), pid);
        printf("[PARENT] Sorting using Bubble Sort...\n");
        bubbleSort(arr, n);

        printf("[PARENT] Sorted Array (Bubble Sort): ");
        printArray(arr, n);

        // Wait for child to finish to prevent Zombie state
        int status;
        /*
        wait(&status):
        - Returns PID of terminated child on success.
        - Returns -1 on error.
        - Fills `status` with exit information.
        */
        wait(&status);

        if (WIFEXITED(status)) {
            printf("\n[PARENT] Child exited with status: %d\n", WEXITSTATUS(status));
        }

        printf("[PARENT] Parent process finished.\n");
    }

    return 0;
}
