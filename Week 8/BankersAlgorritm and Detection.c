#include <stdio.h>
#include <stdbool.h>

#define MAX 10

int main() {
    int n, m;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter number of resources: ");
    scanf("%d", &m);

    int alloc[MAX][MAX], max[MAX][MAX], need[MAX][MAX];
    int available[MAX];

    printf("Enter Allocation Matrix:\n");
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter Maximum Demand Matrix:\n");
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    printf("Enter Available Resources:\n");
    for(int i = 0; i < m; i++)
        scanf("%d", &available[i]);

    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    int process;
    int request[MAX];

    printf("\nEnter process number making request: ");
    scanf("%d", &process);

    printf("Enter request vector:\n");
    for(int i = 0; i < m; i++)
        scanf("%d", &request[i]);

    for(int i = 0; i < m; i++) {
        if(request[i] > need[process][i]) {
            printf("\nError: Request exceeds maximum need.\n");
            return 0;
        }
    }

    for(int i = 0; i < m; i++) {
        if(request[i] > available[i]) {
            printf("\nResources not available. Process must wait.\n");
            return 0;
        }
    }

    for(int i = 0; i < m; i++) {
        available[i] -= request[i];
        alloc[process][i] += request[i];
        need[process][i] -= request[i];
    }

    int work[MAX], safeSeq[MAX];
    bool finish[MAX];

    for(int i = 0; i < m; i++)
        work[i] = available[i];

    for(int i = 0; i < n; i++)
        finish[i] = false;

    int count = 0;

    while(count < n) {
        bool found = false;

        for(int i = 0; i < n; i++) {
            if(!finish[i]) {
                bool possible = true;

                for(int j = 0; j < m; j++) {
                    if(need[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                }

                if(possible) {
                    for(int j = 0; j < m; j++)
                        work[j] += alloc[i][j];

                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if(!found) {
            printf("\nSystem will be UNSAFE after allocation. Request denied.\n");
            return 0;
        }
    }

    printf("\nRequest can be granted.\n");
    printf("System is in SAFE state.\nSafe sequence: ");

    for(int i = 0; i < n; i++) {
        printf("P%d", safeSeq[i]);
        if(i != n - 1)
            printf(" -> ");
    }

    printf("\n");

    return 0;
}
