#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Nak.h"

const char *Status[] = {
    "pending",
    "in progress",
    "finished"

};

typedef struct
{
    char *first_name;
    char *last_name;
    char *email;
} Collaboratin;

typedef struct
{

    int id;
    char *title;
    char *description;
    char *deadline;
    int date_Created[20];
    int StatusIndex;
    Collaboratin *collaboratin;

} Task;

struct tm localTime;
getLocalTime(localTime);

void add_task(Task *task)
{

    scanf("%s", &task->title);
    scanf("%s", &task->deadline);
    scanf("%s", &task->description);
    scanf("%d-%02d-%02d", localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday = task->date_Created);
}

int main()
{
    Task *task;
    add_task(task);
    printf("%s\n", task->title);
    printf("%s\n", task->description);
    printf("%d-%02d%02d\n", task->date_Created);

    return 1;
}

// getLocalTime(&localTime);

// printf("Local Time: %d-%02d-%02d %02d:%02d:%02d\n",
// localTime.tm_year + 1900,
// localTime.tm_mon + 1,
// localTime.tm_mday,
//     localTime.tm_hour,
//     localTime.tm_min,
//     localTime.tm_sec
// );