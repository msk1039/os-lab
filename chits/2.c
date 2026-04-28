
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 50


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


void printArray(int a[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
}

void sortingDemo(void) {

    int n, a[MAX], b[MAX];

    printf("Enter number of integers: ");
    scanf("%d", &n);

    printf("Enter integers:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        b[i] = a[i];
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {

        printf("\n[Child] PID=%d Parent=%d\n", getpid(), getppid());
        selectionSort(b, n);
        printf("[Child] Selection sort result: ");
        printArray(b, n);
        exit(0);
    } else {

        int status;
        printf("\n[Parent] PID=%d Child=%d\n", getpid(), pid);
        bubbleSort(a, n);
        printf("[Parent] Bubble sort result: ");
        printArray(a, n);


        wait(&status);
        printf("[Parent] Child completed, zombie avoided.\n");
    }
}

void zombieDemo(void) {

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
