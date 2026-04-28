/*
Problem Statement (Chit 2):
Implement the C program in which the main function accepts the integers to be sorted. Main function uses the FORK system call to create a new process called a child process. Parent process sorts the integers using a sorting algorithm and waits for the child process using WAIT system call to sort the integers using any sorting algorithm. Also demonstrate zombie and orphan states. . Use basic programming constructs only; do not use advanced inbuilt library functions
*/

/*
============================================================
Fork, Wait, Zombie, and Orphan Demonstration
------------------------------------------------------------
Idea:
- Main process accepts integers.
- fork() creates child process.
- Parent sorts one copy using Bubble Sort.
- Child sorts another copy using Selection Sort.
- wait() lets parent collect child exit status.
- Extra small demos show zombie and orphan behavior.

Detailed example:
- Input: 4 numbers -> 9 2 5 1
- Parent gets child PID from fork() and sorts: 1 2 5 9.
- Child gets return value 0 from fork() and sorts: 1 2 5 9.
- Parent calls wait(), which prevents the child from remaining zombie.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 50

/* Classic Bubble Sort: repeatedly move larger elements to the right. */
void bubbleSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
        }
    }
}

/* Classic Selection Sort: place one minimum element at each position. */
void selectionSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[min]) min = j;
        }
        int t = a[i];
        a[i] = a[min];
        a[min] = t;
    }
}

/* Print array in one line to keep parent/child output easy to compare. */
void printArray(int a[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
}

void sortingDemo(void) {
    /* `a` is parent copy, `b` is child copy (both start with same values). */
    int n, a[MAX], b[MAX];

    printf("Enter number of integers: ");
    scanf("%d", &n);

    printf("Enter integers:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        b[i] = a[i];
    }

    /*
    fork():
    - returns 0 inside child process
    - returns child PID inside parent process
    - returns -1 if process creation fails
    */
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        /* Child process branch: runs concurrently with parent. */
        printf("\n[Child] PID=%d Parent=%d\n", getpid(), getppid());
        selectionSort(b, n);
        printf("[Child] Selection sort result: ");
        printArray(b, n);
        exit(0);
    } else {
        /* Parent process branch: runs its own sorting and then waits. */
        int status;
        printf("\n[Parent] PID=%d Child=%d\n", getpid(), pid);
        bubbleSort(a, n);
        printf("[Parent] Bubble sort result: ");
        printArray(a, n);

        /*
        wait(&status):
        - blocks parent until child terminates
        - returns terminated child PID on success
        - fills status with child's exit information
        */
        wait(&status);
        printf("[Parent] Child completed, zombie avoided.\n");
    }
}

void zombieDemo(void) {
    /* Child exits quickly; parent intentionally delays wait(). */
    pid_t pid = fork();

    if (pid == 0) {
        printf("[Zombie demo child] Exiting now.\n");
        exit(0);
    } else {
        printf("[Zombie demo parent] Sleeping without wait for 10 seconds.\n");
        printf("During this time child may appear as zombie in process table.\n");
        sleep(10);
        wait(NULL);
    }
}

void orphanDemo(void) {
    /* Parent exits first so child gets adopted by init/system process. */
    pid_t pid = fork();

    if (pid == 0) {
        printf("[Orphan demo child] Old parent PID=%d\n", getppid());
        sleep(4);
        printf("[Orphan demo child] New parent PID=%d\n", getppid());
        exit(0);
    } else {
        printf("[Orphan demo parent] Exiting before child.\n");
        exit(0);
    }
}

int main(void) {
    int choice;

    /* Menu keeps single program usable for all required process demos. */
    printf("1. Fork sorting with wait\n");
    printf("2. Zombie demo\n");
    printf("3. Orphan demo\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1) sortingDemo();
    else if (choice == 2) zombieDemo();
    else if (choice == 3) orphanDemo();
    else printf("Invalid choice.\n");

    return 0;
}
