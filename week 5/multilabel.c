#include <stdio.h>
#include <windows.h>

#define MAX 100


#define RESET 7
#define GREEN 10
#define CYAN 11
#define YELLOW 14

typedef struct {
    int id, arrival, burst;
    int completion, turnaround, waiting;
    int type;
    int done;
} Process;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void sortByArrival(Process p[], int n) {
    Process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival > p[j].arrival) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

void displayQueues(Process p[], int n, int time) {
    setColor(CYAN);
    printf("\nTime %d\n", time);

    printf("System Queue: ");
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (!p[i].done && p[i].arrival <= time && p[i].type == 0) {
            printf("P%d ", p[i].id);
            found = 1;
        }
    }
    if (!found) printf("Empty");

    printf("\nUser Queue: ");
    found = 0;
    for (int i = 0; i < n; i++) {
        if (!p[i].done && p[i].arrival <= time && p[i].type == 1) {
            printf("P%d ", p[i].id);
            found = 1;
        }
    }
    if (!found) printf("Empty");

    printf("\n");
    setColor(RESET);
}

int main() {
    int n;
    Process p[MAX];

    setColor(GREEN);
    printf("===== CPU Scheduling Simulator =====\n");
    setColor(RESET);

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i);
        p[i].id = i;

        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);

        printf("Burst Time: ");
        scanf("%d", &p[i].burst);

        printf("Type (0 = System, 1 = User): ");
        scanf("%d", &p[i].type);

        p[i].done = 0;
    }

    sortByArrival(p, n);

    int time = 0, completed = 0;


    int gantt[MAX], gtime[MAX];
    int gindex = 0;
    gtime[gindex++] = 0;

    while (completed < n) {

        displayQueues(p, n, time);

        int idx = -1;


        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].arrival <= time && p[i].type == 0) {
                idx = i;
                break;
            }
        }


        if (idx == -1) {
            for (int i = 0; i < n; i++) {
                if (!p[i].done && p[i].arrival <= time && p[i].type == 1) {
                    idx = i;
                    break;
                }
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        setColor(YELLOW);
        printf("Selected: P%d\n", p[idx].id);
        setColor(RESET);


        gantt[gindex - 1] = p[idx].id;

        time += p[idx].burst;
        gtime[gindex++] = time;


        p[idx].completion = time;
        p[idx].turnaround = time - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        p[idx].done = 1;

        completed++;
    }


    setColor(GREEN);
    printf("\nGantt Chart:\n");
    setColor(RESET);

    for (int i = 0; i < gindex - 1; i++) {
        printf("|  P%d  ", gantt[i]);
    }
    printf("|\n");

    for (int i = 0; i < gindex; i++) {
        printf("%-6d", gtime[i]);
    }


    setColor(GREEN);
    printf("\n\nID\tType\tAT\tBT\tCT\tWT\tTAT\n");
    setColor(RESET);

    float totalWT = 0, totalTAT = 0;

    for (int i = 0; i < n; i++) {
        printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id,
               (p[i].type == 0) ? "System" : "User",
               p[i].arrival,
               p[i].burst,
               p[i].completion,
               p[i].waiting,
               p[i].turnaround);

        totalWT += p[i].waiting;
        totalTAT += p[i].turnaround;
    }

    printf("\nAverage Waiting Time: %.2f", totalWT / n);
    printf("\nAverage Turnaround Time: %.2f\n", totalTAT / n);

    setColor(CYAN);
    printf("\n===== Execution Complete =====\n");
    setColor(RESET);

    return 0;
}



