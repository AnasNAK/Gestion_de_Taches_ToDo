#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char *Status[] = {
    "pending",
    "in progress",
    "finished"};

typedef struct
{
    char *first_name;
    char *last_name;
    char *email;
} Collaboration;

typedef struct
{
    int id;
    char *title;
    char *description;
    char *deadline;
    struct tm localTime;
    int StatusIndex;
    Collaboration *collaboration;
} Task;

Task task = {.id = 0};
int lastTaskID = 0;

void getLocalTime(struct tm *localTime)
{
    time_t currentTime;
    time(&currentTime);
    *localTime = *localtime(&currentTime);
}

void freeTask(Task *task)
{
    free(task->title);
    free(task->description);
    free(task->deadline);
    if (task->collaboration)
    {
        free(task->collaboration->first_name);
        free(task->collaboration->last_name);
        free(task->collaboration->email);
        free(task->collaboration);
    }
}

int readLastTaskID(const char *filename)
{
    int lastID = 0;
    FILE *file = fopen(filename, "r");
    if (file)
    {
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            int taskID;
            char firstName[100], lastName[100], email[100], status[100];
            int year, month, day;

            if (sscanf(line, "%d %s %s %d %s %d %d %d %s %s %s",
                       &taskID, firstName, lastName, &year, status, &year, &month, &day, firstName, lastName, email) == 11)
            {
                // Found a valid task line in the file, update lastID
                lastID = taskID;
            }
        }
        fclose(file);
    }
    return lastID;
}

int getStatusIndex(const char *statusToFind, const char *Status[], int numStatus)
{
    for (int i = 0; i < numStatus; i++)
    {
        if (strcmp(statusToFind, Status[i]) == 0)
        {
            return i;
        }
    }
    // Return a default index (e.g., -1) to indicate not found or handle errors as needed.
    return -1;
}

int readTaskCount(const char *filename)
{
    int taskCount = 0;
    FILE *file = fopen(filename, "r");
    if (file)
    {
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (strstr(line, "****************************************") != NULL)
            {
                taskCount++;
            }
        }
        fclose(file);
    }
    return taskCount;
}

void read_tasks_from_file(Task **tasks, int *numTasks, const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char line[1024];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        Task task;
        task.title = NULL;
        task.description = NULL;
        task.deadline = NULL;
        task.collaboration = NULL;

        int taskID, year, month, day;
        char status[20], firstName[50], lastName[50], email[100];

       if (sscanf(line, "%d %s %s %s %s %d %d %d %s %s %s",
           &task.id, task.title, task.description, task.deadline, status,
           &task.localTime.tm_year, &task.localTime.tm_mon, &task.localTime.tm_mday,
           task.collaboration->first_name, task.collaboration->last_name, task.collaboration->email) == 11)
        {


            (*numTasks)++;
            *tasks = (Task *)realloc(*tasks, (*numTasks) * sizeof(Task));
            (*tasks)[(*numTasks) - 1] = task;
        }
        else
        {
            // Handle invalid data format if needed
        }
    }

    fclose(file);
}

void save_to_file(Task *tasks, int numTasks, const char *filename)
{
    FILE *file = fopen(filename, "a");

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < numTasks; i++)
    {
        fprintf(file, "%d", tasks[i].id);
        fprintf(file, " %s", tasks[i].title);
        fprintf(file, " %s", tasks[i].description);
        fprintf(file, " %s", tasks[i].deadline);
        fprintf(file, " %s", Status[tasks[i].StatusIndex]);
        fprintf(file, " %d %02d %02d",
                tasks[i].localTime.tm_year + 1900,
                tasks[i].localTime.tm_mon + 1,
                tasks[i].localTime.tm_mday);

        if (tasks[i].collaboration != NULL)
        {
            fprintf(file, " %s %s %s\n",
                    tasks[i].collaboration->first_name,
                    tasks[i].collaboration->last_name,
                    tasks[i].collaboration->email);
        }
    }

    fclose(file);
}

void add_task(Task **tasks, int *numTasks)
{

    (*numTasks)++;
    *tasks = (Task *)realloc(*tasks, (*numTasks) * sizeof(Task));
    Task *task = &((*tasks)[(*numTasks) - 1]);
    task->id = ++lastTaskID;
    task->title = (char *)malloc(sizeof(char));
    task->description = (char *)malloc(sizeof(char));
    task->deadline = (char *)malloc(sizeof(char));

    printf("Enter title: ");
    scanf(" %99s", task->title);

    printf("Enter description: ");
    scanf(" %99s", task->description);

    printf("Enter deadline (YYYY-MM-DD): ");
    scanf(" %19s", task->deadline);
    // Display status options
    printf("Select a status:\n");
    for (int i = 0; i < sizeof(Status) / sizeof(Status[0]); i++)
    {
        printf("%d: %s\n", i + 1, Status[i]);
    }

    int statusChoice;
    do
    {
        printf("Enter the status number (1-%d): ", sizeof(Status) / sizeof(Status[0]));
        scanf("%d", &statusChoice);
    } while (statusChoice < 1 || statusChoice > sizeof(Status) / sizeof(Status[0]));

    task->StatusIndex = statusChoice - 1;

    getLocalTime(&(task->localTime));

    char response;

    do
    {
        printf("Do you want to add collaboration? (y/n): ");
        scanf(" %c", &response);
    } while (response != 'y' && response != 'Y' && response != 'n' && response != 'N');

    if (response == 'y' || response == 'Y')
    {
        task->collaboration = malloc(sizeof(Collaboration));

        task->collaboration->first_name = malloc(50);
        task->collaboration->last_name = malloc(50);
        task->collaboration->email = malloc(100);

        printf("Enter first name of collaborator: ");
        scanf("%s", task->collaboration->first_name);

        printf("Enter last name of collaborator: ");
        scanf("%s", task->collaboration->last_name);

        printf("Enter email of collaborator: ");
        scanf("%s", task->collaboration->email);

        // display
        printf("\n****************************************\n");
        printf("Task ID: %d\n", task->id);
        printf("Title: %s\n", task->title);
        printf("Description: %s\n", task->description);
        printf("Deadline: %s\n", task->deadline);
        printf("Status: %s\n", Status[task->StatusIndex]);
        printf("Creation Date: %d-%02d-%02d\n",
               task->localTime.tm_year + 1900,
               task->localTime.tm_mon + 1,
               task->localTime.tm_mday);

        if (task->collaboration != NULL)
        {
            printf("Collaborator: %s %s (%s)\n",
                   task->collaboration->first_name,
                   task->collaboration->last_name,
                   task->collaboration->email);
        }
        printf("\n****************************************\n");
    }
    else
    {
        task->collaboration = NULL;
    }
}

void Add_multiple_tasks()
{
}
void display_tasks(Task *tasks, int numTasks)
{
    for (int i = 0; i < numTasks; i++)
    {
        printf("Task %d:\n", i + 1);
        printf("Task ID: %d\n", tasks[i].id);
        printf("Title: %s\n", tasks[i].title);
        printf("Description: %s\n", tasks[i].description);
        printf("Deadline: %s\n", tasks[i].deadline);
        printf("Status: %s\n", Status[tasks[i].StatusIndex]);
        printf("Creation Date: %d-%02d-%02d\n",
               tasks[i].localTime.tm_year,
               tasks[i].localTime.tm_mon,
               tasks[i].localTime.tm_mday);

        if (tasks[i].collaboration != NULL)
        {
            printf("Collaborator: %s %s (%s)\n",
                   tasks[i].collaboration->first_name,
                   tasks[i].collaboration->last_name,
                   tasks[i].collaboration->email);
        }
        printf("\n****************************************\n");
    }
}

void sort_tasks(Task *task)
{
    // Implement the code to sort tasks by title here
    printf("Sorting tasks by title...\n");
}

void sort_tasks_by_deadline(Task *task)
{
    // Implement the code to sort tasks by deadline here
    printf("Sorting tasks by deadline...\n");
}

void display_near_deadline_tasks(Task *task)
{
    // Implement the code to display tasks with deadlines in 3 days or less here
    printf("Displaying tasks with deadlines in 3 days or less...\n");
}

void modify_task_description(Task *task)
{
    // Implement the code to modify task description here
    printf("Modifying task description...\n");
}

void modify_task_status(Task *task)
{
    // Implement the code to modify task status here
    printf("Modifying task status...\n");
}

void modify_task_deadline(Task *task)
{
    // Implement the code to modify task deadline here
    printf("Modifying task deadline...\n");
}

void delete_task(Task *task)
{
    // Implement the code to delete a task by its identifier here
    printf("Deleting a task by identifier...\n");
}

void search_task_by_id(Task *task)
{
}

void search_task_by_title(Task *task)
{
    // Implement the code to search a task by its title here
    printf("Searching a task by title...\n");
}

void display_statistics(Task *task)
{
    // Implement the code to display statistics here
    printf("Displaying statistics...\n");
}

int main()
{
    const char *filename = "C:\\Users\\ANAS_NAK\\Desktop\\Gestion_de_Taches_ToDo\\dataSaS.txt";
    // const char *filename = "C:\\Users\\adm\\OneDrive\\Desktop\\Gestion_de_Taches_ToDo\\dataSaS.txt";
    int choice, choice2, choice3, choice4, choice5;
    Task *tasks = NULL;
    int numTasks = 0;
    lastTaskID = readLastTaskID(filename);

    while (1)
    {
        printf("\n\t\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
        printf("\t\t\t\t\t\t\t1. Add a new task\n");
        printf("\t\t\t\t\t\t\t2. Add multiple new tasks\n");
        printf("\t\t\t\t\t\t\t3. Display all tasks\n");
        printf("\t\t\t\t\t\t\t4. Modify a task\n");
        printf("\t\t\t\t\t\t\t5. Delete a task \n");
        printf("\t\t\t\t\t\t\t6. Search for task\n");
        printf("\t\t\t\t\t\t\t7. Statistics\n");
        printf("\t\t\t\t\t\t\t8. Exit\n");
        printf("\n\t\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            add_task(&tasks, &numTasks);
            save_to_file(tasks, numTasks, filename);
            break;
        case 2:

            break;
        case 3:

            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ list of all tasks Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("\t\t\t\t\t1. Sort tasks by alphabetical order\n");
            printf("\t\t\t\t\t2. Sort tasks by deadline\n");
            printf("\t\t\t\t\t3. Display tasks with a deadline in 3 days or less\n");
            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ list of all tasks Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("Enter your choice (1/2/3): ");
            scanf("%d", &choice2);

            switch (choice2)
            {
            case 1:

                read_tasks_from_file(&tasks, &numTasks, filename);
                display_tasks(tasks, numTasks);
                break;
            case 2:

                printf("Enter your2");
                break;
            case 3:
                printf("Enter your3");
                break;
            default:
                printf("Invalid choice.\n");
                break;
            }

            break;

        case 4:
            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ list of modifiying task Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("\t\t\t\t\t\t\t1. modify a task description\n");
            printf("\t\t\t\t\t\t\t2. modify a task status\n");
            printf("\t\t\t\t\t\t\t3. modify a task deadline\n");
            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ list of modifiying task Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("Enter your choice (1/2/3): ");
            scanf("%d", &choice3);

            switch (choice3)
            {
            case 1:

                printf("md1");
                break;
            case 2:

                printf("md2");
                break;
            case 3:
                printf("md3");
                break;
            default:
                printf("Invalid choice.\n");
                break;
            }
            break;
        case 5:
            delete_task(tasks);
            break;
        case 6:
            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ list of search task Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("\t\t\t\t\t\t\t1. search for task by ID\n");
            printf("\t\t\t\t\t\t\t2. search for task by title\n");
            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ list of search task Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("Enter your choice (1/2): ");
            scanf("%d", &choice4);

            switch (choice4)
            {
            case 1:

                printf("sch1");
                break;
            case 2:

                printf("sch2");
                break;
            default:
                printf("Invalid choice.\n");
                break;
            }
            break;
        case 7:
            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ statistic Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("\t\t\t\t\t1. total number of tasks\n");
            printf("\t\t\t\t\t2. number of tasks complet and not complet\n");
            printf("\t\t\t\t\t3. the number of days remaining until the deadline of each task\n");
            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ statistic Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("Enter your choice (1/2/3): ");
            scanf("%d", &choice5);

            switch (choice5)
            {
            case 1:

                printf("st1");
                break;
            case 2:

                printf("st2");
                break;
            case 3:
                printf("st3");
                break;
            default:
                printf("Invalid choice.\n");
                break;
            }
            break;
        case 8:
            printf("don't worry about all this is reserved on the database ");
            for (int i = 0; i < numTasks; i++)
            {
                freeTask(&tasks[i]);
            }
            free(tasks); // Free the tasks array
            return 0;
        default:
            printf("Invalid choice, please try again.\n");
        }
    }
}
