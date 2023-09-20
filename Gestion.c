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
            if (sscanf(line, "Task ID: %d", &taskID) == 1)
            {
                // Found a Task ID in the file, update lastID
                lastID = taskID;
            }
        }
        fclose(file);
    }
    return lastID;
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
        fprintf(file, "Task ID: %d\n", tasks[i].id);
        fprintf(file, "Title: %s\n", tasks[i].title);
        fprintf(file, "Description: %s\n", tasks[i].description);
        fprintf(file, "Deadline: %s\n", tasks[i].deadline);
        fprintf(file, "Status: %s\n", Status[tasks[i].StatusIndex]);
        fprintf(file, "Creation Date: %d-%02d-%02d\n",
                tasks[i].localTime.tm_year + 1900,
                tasks[i].localTime.tm_mon + 1,
                tasks[i].localTime.tm_mday);

        if (tasks[i].collaboration != NULL)
        {
            fprintf(file, "Collaborator: %s %s (%s)\n",
                    tasks[i].collaboration->first_name,
                    tasks[i].collaboration->last_name,
                    tasks[i].collaboration->email);
        }

        fprintf(file, "****************************************\n");
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

    // Set the status index, subtracting 1 to match the array index
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
    printf("Displaying tasks...\n");
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
    int choice;
    Task *tasks = NULL;
    int numTasks = 0;

    lastTaskID = readLastTaskID(filename);

    while (1)
    {
        printf("\n\t\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~Menu~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
        printf("\t\t\t\t\t1. Add a new task\n");
        printf("\t\t\t\t\t2. Add multiple new tasks\n");
        printf("\t\t\t\t\t3. Display the list of all tasks\n");
        printf("\t\t\t\t\t4. Sort tasks in alphabetical order\n");
        printf("\t\t\t\t\t5. Sort tasks by deadline\n");
        printf("\t\t\t\t\t6. Display tasks with a deadline in 3 days or less\n");
        printf("\t\t\t\t\t7. Modify a task\n");
        printf("\t\t\t\t\t8. Delete a task by ID\n");
        printf("\t\t\t\t\t9. Search for tasks\n");
        printf("\t\t\t\t\t10. Statistics\n");
        printf("\t\t\t\t\t11. Exit\n");
        printf("\n\t\t\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~Menu~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
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
            display_tasks(tasks, numTasks);
            break;
        case 4:
            sort_tasks(tasks); // Pass tasks to the sort_tasks function
            break;
        case 5:
            sort_tasks_by_deadline(tasks); // Pass tasks to the sort_tasks_by_deadline function
            break;
        case 6:
            display_near_deadline_tasks(tasks); // Pass tasks to the display_near_deadline_tasks function
            break;
        case 7:
            // Modify a task
            break;
        case 8:
            delete_task(tasks); // Pass tasks to the delete_task function
            break;
        case 9:
            // Search tasks
            break;
        case 10:
            display_statistics(tasks); // Pass tasks to the display_statistics function
            break;
        case 11:
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
