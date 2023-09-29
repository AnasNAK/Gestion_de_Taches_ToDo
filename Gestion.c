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
int numcoll;

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

void add_task(Task **tasks, int *numTasks)
{
    (*numTasks)++;
    *tasks = (Task *)realloc(*tasks, (*numTasks) * sizeof(Task));
    Task *task = &((*tasks)[(*numTasks) - 1]);
    task->id = ++lastTaskID;

    task->title = (char *)malloc(100);
    task->description = (char *)malloc(100);
    task->deadline = (char *)malloc(20);

    printf("Enter title: ");
    scanf(" %s", task->title);

    printf("Enter description: ");
    scanf(" %[^\n]", task->description);

    printf("Enter deadline (YYYY-MM-DD): ");
    scanf(" %[^\n]", task->deadline);

    // status options
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
        printf("How many collaborators do you want to add: ");
        scanf("%d", &numcoll);

        if (numcoll > 0)
        {
            task->collaboration = (Collaboration *)malloc(numcoll * sizeof(Collaboration));

            for (int i = 0; i < numcoll; i++)
            {
                task->collaboration[i].first_name = (char *)malloc(50);
                task->collaboration[i].last_name = (char *)malloc(50);
                task->collaboration[i].email = (char *)malloc(100);

                printf("Enter first name of collaborator[%d]: ", i + 1);
                scanf(" %49s", task->collaboration[i].first_name);

                printf("Enter last name of collaborator[%d]: ", i + 1);
                scanf(" %49s", task->collaboration[i].last_name);

                printf("Enter email of collaborator[%d]: ", i + 1);
                scanf(" %99s", task->collaboration[i].email);
            }
        }
        else
        {
            task->collaboration = NULL;
        }
    }
    else
    {
        task->collaboration = NULL;
    }
}

void Add_multiple_tasks(Task **tasks, int *numTasks)
{
    char response;

    do
    {
        add_task(tasks, numTasks);

        printf("Do you want to add another task? (y/n): ");
        scanf(" %c", &response);
    } while (response == 'y' || response == 'Y');
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
            for (int j = 0; j < numcoll; j++)
            {
                printf("Collaborator [%d]:  %s %s (%s)\n", j,
                       tasks[i].collaboration[j].first_name,
                       tasks[i].collaboration[j].last_name,
                       tasks[i].collaboration[j].email);
            }
        }
        printf("\n****************************************\n");
    }
}

int compare_titles(const void *a, const void *b)
{
    Task *task1 = (Task *)a;
    Task *task2 = (Task *)b;
    return strcmp(task1->title, task2->title);
}
int compare_deadline(const void *a, const void *b)
{
    const Task *taskA = (const Task *)a;
    const Task *taskB = (const Task *)b;

    // Assuming deadline is a string in the format "YYYY-MM-DD"
    return strcmp(taskA->deadline, taskB->deadline);
}

void display_tasks_sorted_by_title(Task *tasks, int numTasks)
{
    if (numTasks == 0)
    {
        printf("No tasks to display.\n");
        return;
    }

    qsort(tasks, numTasks, sizeof(Task), compare_titles);

    printf("Tasks sorted by title:\n");
    for (int i = 0; i < numTasks; i++)
    {
        printf("Task %d:\n", i + 1);
        printf("Task ID: %d\n", tasks[i].id);
        printf("Title: %s\n", tasks[i].title);
        printf("Description: %s\n", tasks[i].description);
        printf("Deadline: %s\n", tasks[i].deadline);
        printf("Status: %s\n", Status[tasks[i].StatusIndex]);
        printf("Creation Date: %d-%02d-%02d\n",
               tasks[i].localTime.tm_year + 1900,
               tasks[i].localTime.tm_mon + 1,
               tasks[i].localTime.tm_mday);

        if (tasks[i].collaboration != NULL)
        {
            for (int j = 0; j < numcoll; j++)
            {
                printf("Collaborator [%d]:  %s %s (%s)\n", j + 1,
                       tasks[i].collaboration[j].first_name,
                       tasks[i].collaboration[j].last_name,
                       tasks[i].collaboration[j].email);
            }
        }
        printf("\n****************************************\n");
    }
}

void display_tasks_sorted_by_deadline(Task *tasks, int numTasks)
{
    if (numTasks == 0)
    {
        printf("No tasks to display.\n");
        return;
    }

    qsort(tasks, numTasks, sizeof(Task), compare_deadline);

    printf("Tasks sorted by deadline:\n");
    for (int i = 0; i < numTasks; i++)
    {
        printf("Task %d:\n", i + 1);
        printf("Task ID: %d\n", tasks[i].id);
        printf("Title: %s\n", tasks[i].title);
        printf("Description: %s\n", tasks[i].description);
        printf("Deadline: %s\n", tasks[i].deadline);
        printf("Status: %s\n", Status[tasks[i].StatusIndex]);
        printf("Creation Date: %d-%02d-%02d\n",
               tasks[i].localTime.tm_year + 1900,
               tasks[i].localTime.tm_mon + 1,
               tasks[i].localTime.tm_mday);

        if (tasks[i].collaboration != NULL)
        {
            for (int j = 0; j < numcoll; j++)
            {
                printf("Collaborator [%d]:  %s %s (%s)\n", j + 1,
                       tasks[i].collaboration[j].first_name,
                       tasks[i].collaboration[j].last_name,
                       tasks[i].collaboration[j].email);
            }
        }
        printf("\n****************************************\n");
    }
}

void display_tasks_with_near_deadlines(const Task *tasks, int numTasks)
{
    printf("Tasks with deadlines in 3 days or less:\n");
    for (int i = 0; i < numTasks; i++)
    {
        int yearDeadline = 0, monthDeadline = 0, dayDeadline = 0;
        int yearCreation = 0, monthCreation = 0, dayCreation = 0;

        if (sscanf(tasks[i].deadline, "%4d-%2d-%2d", &yearDeadline, &monthDeadline, &dayDeadline) != 3)
        {
            printf("Failed to parse deadline date: %s\n", tasks[i].deadline);
            continue;
        }

        yearCreation = tasks[i].localTime.tm_year + 1900;
        monthCreation = tasks[i].localTime.tm_mon + 1;
        dayCreation = tasks[i].localTime.tm_mday;

        int deadlineDays = yearDeadline * 365 + monthDeadline * 30 + dayDeadline;
        int creationDays = yearCreation * 365 + monthCreation * 30 + dayCreation;
        int diffDays = deadlineDays - creationDays;

        if (diffDays >= 0 && diffDays <= 3)
        {
            printf("Task ID: %d\n", tasks[i].id);
            printf("Title: %s\n", tasks[i].title);
            printf("Description: %s\n", tasks[i].description);
            printf("Deadline: %s\n", tasks[i].deadline);
            printf("Status: %s\n", Status[tasks[i].StatusIndex]);
            printf("Creation Date: %d-%02d-%02d\n", yearCreation, monthCreation, dayCreation);
            // printf("Parsed Deadline Date: %04d-%02d-%02d\n", yearDeadline, monthDeadline, dayDeadline);
            // printf("Difference in days: %d\n", diffDays);
            if (tasks[i].collaboration != NULL)
            {
                for (int j = 0; j < numcoll; j++)
                {
                    printf("Collaborator [%d]:  %s %s (%s)\n", j + 1,
                           tasks[i].collaboration[j].first_name,
                           tasks[i].collaboration[j].last_name,
                           tasks[i].collaboration[j].email);
                }
            }

            printf("This task is within 3 days or less\n");
            printf("\n");
        }
    }
}

Task *search_task_by_id(Task *tasks, int numTasks, int taskId)
{
    for (int i = 0; i < numTasks; i++)
    {
        if (tasks[i].id == taskId)
        {
            return &tasks[i];
        }
    }
    return NULL;
}

Task *search_task_by_title(Task *tasks, int numTasks, const char *title)
{
    for (int i = 0; i < numTasks; i++)
    {
        if (strcmp(tasks[i].title, title) == 0)
        {
            return &tasks[i];
        }
    }
    return NULL;
}

void modify_task_description(Task *task)
{
    printf("Enter the new description: ");
    scanf(" %[^\n]", task->description);
    printf("Description updated successfully.\n");
}

void modify_task_status(Task *task)
{
    int newStatus;
    printf("Enter the new status (1: pending, 2: in progress, 3: finished): ");
    scanf("%d", &newStatus);
    if (newStatus >= 1 && newStatus <= 3)
    {
        task->StatusIndex = newStatus - 1;
        printf("Status updated successfully.\n");
    }
    else
    {
        printf("Invalid status choice.\n");
    }
}

void modify_task_deadline(Task *task)
{
    printf("Enter the new deadline (YYYY-MM-DD): ");
    scanf(" %[^\n]", task->deadline);
    printf("Deadline updated successfully.\n");
}
void update_task_title(Task *task)
{
    printf("Enter the new title: ");
    scanf(" %[^\n]", task->title);
    printf("Title updated successfully.\n");
}

void add_collaborator(Task *task)
{
    if (task->collaboration == NULL)
    {
        task->collaboration = (Collaboration *)malloc(sizeof(Collaboration));
        numcoll = 1; // Update the number of collaborators
    }
    else
    {
        numcoll++; // Increment the number of collaborators
        task->collaboration = (Collaboration *)realloc(task->collaboration, numcoll * sizeof(Collaboration));
    }

    int newCollaboratorIndex = numcoll - 1; // Index for the new collaborator

    task->collaboration[newCollaboratorIndex].first_name = (char *)malloc(50);
    task->collaboration[newCollaboratorIndex].last_name = (char *)malloc(50);
    task->collaboration[newCollaboratorIndex].email = (char *)malloc(100);

    printf("Enter first name of collaborator: ");
    scanf(" %49s", task->collaboration[newCollaboratorIndex].first_name);

    printf("Enter last name of collaborator: ");
    scanf(" %49s", task->collaboration[newCollaboratorIndex].last_name);

    printf("Enter email of collaborator: ");
    scanf(" %99s", task->collaboration[newCollaboratorIndex].email);

    printf("Collaborator added successfully.\n");
}

void update_collaborator(Task *task)
{
    if (task->collaboration != NULL)
    {
        int collaboratorIndex;
        printf("Enter the index of the collaborator to update (1-%d): ", numcoll);
        scanf("%d", &collaboratorIndex);

        if (collaboratorIndex >= 1 && collaboratorIndex <= numcoll)
        {
            collaboratorIndex--; // Convert to 0-based index

            printf("Enter the new first name of collaborator: ");
            scanf(" %49s", task->collaboration[collaboratorIndex].first_name);

            printf("Enter the new last name of collaborator: ");
            scanf(" %49s", task->collaboration[collaboratorIndex].last_name);

            printf("Enter the new email of collaborator: ");
            scanf(" %99s", task->collaboration[collaboratorIndex].email);

            printf("Collaborator updated successfully.\n");
        }
        else
        {
            printf("Invalid collaborator index.\n");
        }
    }
    else
    {
        printf("No collaborators found for this task.\n");
    }
}

void delete_task(Task **tasks, int *numTasks, int taskId)
{
    int index = -1;

    // Find the index of the task with the given taskId
    for (int i = 0; i < *numTasks; i++)
    {
        if ((*tasks)[i].id == taskId)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        // Free the memory allocated for the task and its collaborators
        freeTask(&((*tasks)[index]));

        // Move the tasks after the deleted task one position forward in the array
        for (int i = index; i < *numTasks - 1; i++)
        {
            (*tasks)[i] = (*tasks)[i + 1];
        }

        // Decrease the number of tasks
        (*numTasks)--;

        // Reallocate memory to shrink the array
        *tasks = (Task *)realloc(*tasks, (*numTasks) * sizeof(Task));

        printf("Task with ID %d has been deleted.\n", taskId);
    }
    else
    {
        printf("Task with ID %d not found.\n", taskId);
    }
}

void display_completed_and_not_completed_tasks(Task *tasks, int numTasks)
{
    int completedTasks = 0;
    int notCompletedTasks = 0;

    for (int i = 0; i < numTasks; i++)
    {
        if (tasks[i].StatusIndex == 2)
        { // Assuming 2 represents "finished" status
            completedTasks++;
        }
        else
        {
            notCompletedTasks++;
        }
    }

    printf("Number of completed tasks: %d\n", completedTasks);
    printf("Number of not completed tasks: %d\n", notCompletedTasks);
}

void display_days_remaining_until_deadline(Task *tasks, int numTasks)
{
    struct tm currentTime;
    getLocalTime(&currentTime);

    for (int i = 0; i < numTasks; i++)
    {
        int yearDeadline = 0, monthDeadline = 0, dayDeadline = 0;
        int yearCreation = 0, monthCreation = 0, dayCreation = 0;

        if (sscanf(tasks[i].deadline, "%4d-%2d-%2d", &yearDeadline, &monthDeadline, &dayDeadline) != 3)
        {
            printf("Failed to parse deadline date for Task %d.\n", tasks[i].id);
            continue;
        }

        yearCreation = tasks[i].localTime.tm_year + 1900;
        monthCreation = tasks[i].localTime.tm_mon + 1;
        dayCreation = tasks[i].localTime.tm_mday;

        int deadlineDays = yearDeadline * 365 + monthDeadline * 30 + dayDeadline;
        int creationDays = yearCreation * 365 + monthCreation * 30 + dayCreation;
        int diffDays = deadlineDays - creationDays;

        printf("Task ID: %d\n", tasks[i].id);
        printf("Title: %s\n", tasks[i].title);
        printf("Days remaining until the deadline: %d\n", diffDays);
        printf("\n");
    }
}

int main()
{
    // const char *filename = "C:\\Users\\ANAS_NAK\\Desktop\\Gestion_de_Taches_ToDo\\dataSaS.txt";
    // const char *filename = "C:\\Users\\adm\\OneDrive\\Desktop\\Gestion_de_Taches_ToDo\\dataSaS.txt";
    int choice, choice2, choice3, choice4, choice5;
    Task *tasks = NULL;
    int numTasks = 0;

    // lastTaskID = readLastTaskID(filename);

    while (1)
    {
    start:

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

        while (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number: ");
            while (getchar() != '\n')
                ;
        }
        switch (choice)
        {
        case 1:
            add_task(&tasks, &numTasks);
            // save_to_file(tasks, numTasks, filename);
            break;
        case 2:
            Add_multiple_tasks(&tasks, &numTasks);
            break;
        case 3:
            while (1)
            {

                printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ list of all tasks Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                printf("\t\t\t\t\t1. Sort tasks by alphabetical order\n");
                printf("\t\t\t\t\t2. Sort tasks by deadline\n");
                printf("\t\t\t\t\t3. Display tasks with a deadline in 3 days or less\n");
                printf("\t\t\t\t\t4. Return\n");
                printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ list of all tasks Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                printf("Enter your choice (1/2/3/4): ");
                while (scanf("%d", &choice2) != 1)
                {
                    printf("Invalid input. Please enter a number: ");
                    while (getchar() != '\n')
                        ;
                }

                switch (choice2)
                {
                case 1:
                    display_tasks_sorted_by_title(tasks, numTasks);
                    break;

                case 2:

                    display_tasks_sorted_by_deadline(tasks, numTasks);
                    break;
                case 3:
                    display_tasks_with_near_deadlines(tasks, numTasks);
                    break;
                case 4:
                    goto start;
                    break;
                default:
                    printf("Invalid choice.\n");
                    break;
                }
            }

            break;

        case 4:
        {
            int taskId;
            printf("Enter the task ID to modify: ");
            scanf("%d", &taskId);

            Task *foundTask = search_task_by_id(tasks, numTasks, taskId);

            if (foundTask != NULL)
            {

                printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ Options ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                printf("\t\t\t\t\t1. Modify description\n");
                printf("\t\t\t\t\t2. Modify status\n");
                printf("\t\t\t\t\t3. Modify deadline\n");
                printf("\t\t\t\t\t4. Modify title\n");
                printf("\t\t\t\t\t5. Add collaborator\n");
                printf("\t\t\t\t\t6. Update collaborator\n");
                printf("\t\t\t\t\t7. Return to Menu \n");
                printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ Options ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                printf("Enter your choice: ");
                while (scanf("%d", &choice3) != 1)
                {
                    printf("Invalid input. Please enter a number: ");
                    while (getchar() != '\n')
                        ;
                }
                switch (choice3)
                {
                case 1:
                    modify_task_description(foundTask);
                    break;
                case 2:
                    modify_task_status(foundTask);
                    break;
                case 3:
                    modify_task_deadline(foundTask);
                    break;
                case 4:
                    update_task_title(foundTask);
                    break;
                case 5:
                    add_collaborator(foundTask);
                    break;
                case 6:
                    update_collaborator(foundTask);
                    break;
                case 7:
                    goto start;
                    break;
                default:
                    printf("Invalid choice.\n");
                    break;
                }
            }
            else
            {
                printf("Task not found.\n");
            }
            break;
        }
        break;
        case 5:
        {
            int deleteTaskId;
            printf("Enter the ID of the task to delete: ");
            scanf("%d", &deleteTaskId);
            delete_task(&tasks, &numTasks, deleteTaskId);
        }
        break;
        case 6:
            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ list of search task Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("\t\t\t\t\t\t\t1. Search for task by ID\n");
            printf("\t\t\t\t\t\t\t2. Search for task by title\n");
            printf("\t\t\t\t\t\t\t3. Return to menu\n");
            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ list of search task Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("Enter your choice (1/2/3): ");
            while (scanf("%d", &choice4) != 1)
            {
                printf("Invalid input. Please enter a number: ");
                while (getchar() != '\n')
                    ;
            }
            switch (choice4)
            {
            case 1:
            {
                int searchId;
                printf("Enter the task ID to search for: ");
                scanf("%d", &searchId);

                Task *foundTaskById = search_task_by_id(tasks, numTasks, searchId);

                if (foundTaskById != NULL)
                {
                    printf("Task found by ID:\n");
                    display_tasks(foundTaskById, 1);
                }
                else
                {
                    printf("Task with ID %d not found.\n", searchId);
                }
                break;
            }
            case 2:
            {
                char searchTitle[100];
                printf("Enter the task title to search for: ");
                scanf(" %[^\n]", searchTitle);

                Task *foundTaskByTitle = search_task_by_title(tasks, numTasks, searchTitle);

                if (foundTaskByTitle != NULL)
                {
                    printf("Task found by title:\n");
                    display_tasks(foundTaskByTitle, 1);
                }
                else
                {
                    printf("Task with title '%s' not found.\n", searchTitle);
                }
                break;
            }
            case 3:
                goto start;
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
            printf("\t\t\t\t\t4. Return to Menu\n");
            printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~ statistic Menu ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            printf("Enter your choice (1/2/3/4): ");
            while (scanf("%d", &choice5) != 1)
            {
                printf("Invalid input. Please enter a number: ");
                while (getchar() != '\n')
                    ;
            }
            switch (choice5)
            {
            case 1:
                printf("Total number of tasks: %d\n", numTasks);
                break;
            case 2:

                display_completed_and_not_completed_tasks(tasks, numTasks);
                break;
            case 3:
                display_days_remaining_until_deadline(tasks, numTasks);
                break;
            case 4:
                goto start;
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
            free(tasks);
            return 0;
        default:
            printf("Invalid choice, please try again.\n");
        }
    }
}