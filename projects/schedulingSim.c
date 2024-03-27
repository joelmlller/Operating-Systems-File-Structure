/*
Group HW2 by Laython Childers and Joel Miller
CPSC3220

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct task{
 int
 task_id, /* alphabetic tid can be obtained as 'A'+(task_counter++) */
 arrival_time,
 service_time,
 remaining_time,
 completion_time,
 response_time,
 wait_time;
 struct task *next;
};

struct queue{
 struct task *queuedTask;
 struct queue *next;
};

void enqueue(struct queue* readyQueue, struct task* procTask){
    struct queue* temp = readyQueue;
    while (temp->next != NULL){
        temp = temp->next;
    } 
    temp->next = malloc(sizeof(struct queue));
    temp = temp->next;
    temp->next = NULL;
    temp->queuedTask = procTask;
}

//Frees and returns new head of queue
struct queue* dequeue(struct queue* readyQueue){
    struct queue* temp = readyQueue->next;
    free(readyQueue);
    return temp;
}

//Inserts struct to front of queue. Returns head pointer
struct queue* insert(struct queue* readyQueue, struct task* procTask){
    struct queue* newHead = malloc(sizeof(struct queue*));
    newHead->next = readyQueue;
    newHead->queuedTask = procTask;
    return newHead;
}

//Injects struct in queue at index(starting at 0)
void inject(struct queue* readyQueue, struct task* procTask, int index){
    struct queue* temp = readyQueue;
    struct queue* newNode = malloc(sizeof(struct queue));
    for(int i = 0; i < (index-1); i++){
        temp = temp->next;
    }
    newNode->next = temp->next;
    temp->next = newNode;
    newNode->queuedTask = procTask;
}

bool find (struct queue* readyQueue, struct task* procTask){
    struct queue* temp = readyQueue;
    while(temp != NULL){
        if(temp->queuedTask->task_id == procTask->task_id){
            return true;
        }
        else temp = temp->next;
    }
    return false;
}

int queueLength(struct queue* readyQueue){
    struct queue* temp = readyQueue;
    int num = 0;
        while(temp != NULL){
            num++;
            temp = temp->next;
        }
        return num;
}

//Takes in a line containing an int pair and an int offset for alphabet char selection
struct task* initData(char currLine[], int numTasks){
    struct task *newTask = malloc(sizeof(struct task));
    newTask->task_id = 'A' + numTasks;

    int tmpIndex, index =  0;
    int maxLineLength = 10;
    char temp[maxLineLength];
    for(int iter = 0; iter < 2; iter++){
        tmpIndex = 0;
        do{
            temp[tmpIndex] = currLine[index];
            index++;
            tmpIndex++;
        }while(currLine[index] != ' ' && currLine[index] != '\n' && index < (int)strlen(currLine));

        index++;
        temp[tmpIndex] = '\0';

        if (iter == 0)
            newTask->arrival_time = atoi(temp);
        else{
            newTask->service_time = atoi(temp);
            newTask->remaining_time = newTask->service_time;
            newTask->completion_time = 0;
            newTask->response_time = 0;
            newTask->wait_time = 0;
        }
    }
    return newTask;
}

//Print format header for task trace
void printTraceHeader(){
    printf("time   cpu ready queue (tid/rst)\n");
    printf("----------------------------\n");
}

//Print format header for task summary
void printSummaryHeader(){
    printf("        arrival service completion response wait\n");
    printf("tid     time    time    time       time     time\n");
     printf("---------------------------------------------\n");
}

//Print summary data for time statistics
void printSummaryData(struct task* head){
    printSummaryHeader();
    struct task* currNode = head;
    while(currNode != 0x0){
        printf("%c%9d%9d%9d%9d%9d\n", currNode->task_id, currNode->arrival_time, currNode->service_time, currNode->completion_time, currNode->response_time, currNode->wait_time);
        currNode = currNode->next;
    }
}

//Print format header for service times
void printServiceTimeHeader(){
    printf("service wait\n");
    printf(" time   time\n");
    printf("------- ----\n");
}

//Print format header for service times
void printServiceTimeData(struct task* head){
   printServiceTimeHeader();
    struct task* currNode = head;
    struct queue* ServiceTimeAscending = malloc(sizeof(struct queue));
    ServiceTimeAscending->next = NULL;
    ServiceTimeAscending->queuedTask = head;
    struct queue* temp;
    int index = 0;
    while(currNode->next != 0x0){
        currNode = currNode->next;
        temp = ServiceTimeAscending;
        while(temp != 0x0){
            if (currNode->service_time > temp->queuedTask->service_time){
                inject(ServiceTimeAscending, currNode,(index + 1));
                break;
            }
            else if (currNode->service_time < temp->queuedTask->service_time){
                ServiceTimeAscending = insert(ServiceTimeAscending, currNode);
                break;
            }
            else{
                inject(ServiceTimeAscending, currNode,(index + 1));
                break;
            }
        }  
        temp = temp->next;
    }

    temp = ServiceTimeAscending;
    while(temp != 0x0){
        printf("%4d%6d\n", temp->queuedTask->service_time, temp->queuedTask->wait_time);
        temp = temp->next;
    }
        
        
}

//RR Scheduler
void RRScheduler(struct task* head, int numTasks) {
int tasksCompleted = 0;
    int clock = 0;
    struct task* currTask = head;
    struct queue* readyQueue;
    struct task* temp;
    char emptyQueue = '-';
    struct queue* tempQueue;
    int prevRunTID = -1;
    while(tasksCompleted < numTasks){
        if(currTask->arrival_time <= clock){
            //If currTask requires more processing time
            if(currTask->remaining_time > 0){
                //branch for checking future tasks into ready queue
                temp = currTask->next;
                while(temp != NULL && temp->arrival_time <= clock && temp->remaining_time != 0){
                    if (readyQueue == NULL){
                        readyQueue =  malloc(sizeof(struct queue));
                        readyQueue->next = NULL;
                        readyQueue->queuedTask = temp;
                    }
                    else{
                        if (!find(readyQueue, temp)){
                            readyQueue = insert(readyQueue, temp);
                        }
                    }
                    if (temp != NULL){
                        temp = temp->next;
                    }      
                }

                if(prevRunTID == currTask->task_id && readyQueue != NULL){
                    enqueue(readyQueue, currTask);
                    currTask = readyQueue->queuedTask;
                    readyQueue = dequeue(readyQueue);
                }
                prevRunTID = currTask->task_id;
                //If readyQueue contains tasks
                if(readyQueue != NULL){



                    printf("%3d%5c%d     ", clock, currTask->task_id, currTask->remaining_time);
                    tempQueue = readyQueue;
                    int num = 0;
                    while(tempQueue != NULL){
                        if(num == queueLength(readyQueue) - 1){
                            printf("%c%c\n", tempQueue->queuedTask->task_id, (tempQueue->queuedTask->remaining_time + 48));
                        }
                        else{ 
                            printf("%c%c, ", tempQueue->queuedTask->task_id, (tempQueue->queuedTask->remaining_time + 48));
                        }
                        num++;
                        tempQueue->queuedTask->wait_time++;
                        tempQueue = tempQueue->next;
                    }
                    currTask->remaining_time--;
                    clock++;

                    if(currTask->remaining_time == 0){
                        currTask->response_time = currTask->wait_time + currTask->service_time;
                        currTask->completion_time = currTask->arrival_time + currTask->response_time;

                    }
                    else{
                        enqueue(readyQueue, currTask);
                    }
                currTask = readyQueue->queuedTask;
                readyQueue = dequeue(readyQueue);
                }
                //readyQueue is empty
                else{
                    printf("%3d%5c%d%6c%c\n", clock, currTask->task_id, currTask->remaining_time, emptyQueue, emptyQueue);
                    currTask->remaining_time--;
                    clock++; 
                }
            }
            //Else, currTask requires no more processing time. Update to next process
            else{
                currTask->response_time = currTask->wait_time + currTask->service_time;
                currTask->completion_time = currTask->arrival_time + currTask->response_time;
                currTask = currTask->next;
                if(readyQueue != NULL){
                    readyQueue = dequeue(readyQueue);
                }
                tasksCompleted++;
            }
        }
        //Else no tasks have arrived at current clock cycle
        else{
            if (readyQueue != NULL){
                printf("%3d%12c%c\n", clock, readyQueue->queuedTask->task_id, (readyQueue->queuedTask->remaining_time + 48));
            }
            else{
                printf("%3d%12c%c\n", clock, emptyQueue, emptyQueue);
            }
            clock++;
            prevRunTID = -1;
        }
    }

}


// Given pointer to linked list of tasks and num of tasks, runs sim of SJF scheduling
void SJFScheduler(struct task* head, int numTasks) {
    int tasksCompleted = 0;
    int clock = 0;
    struct task* currTask = head;
    struct queue* readyQueue;
    struct task* temp;
    char emptyQueue = '-';
    struct queue* tempQueue;

    while(tasksCompleted < numTasks){
        //If currTask arrived at or before current clock cycle
        if(currTask->arrival_time <= clock){
            //If currTask requires more processing time
            if(currTask->remaining_time > 0){
                //branch for checking future tasks into ready queue
                temp = currTask->next;
                while(temp != NULL && temp->arrival_time <= clock && temp->remaining_time != 0){
                    if (readyQueue == NULL){
                        readyQueue =  malloc(sizeof(struct queue));
                        readyQueue->next = NULL;
                        readyQueue->queuedTask = temp;

                    }
                    else{
                        if (!find(readyQueue, temp)){
                            readyQueue = insert(readyQueue, temp);
                        }
                    }
                    if (temp != NULL){
                        temp = temp->next;
                    }      
                }
                //If readyQueue contains tasks
                if(readyQueue != NULL){
                    tempQueue = readyQueue;
                    while(tempQueue != NULL){
                        temp = currTask;
                        if(tempQueue->queuedTask->remaining_time < currTask->remaining_time){
                            currTask = tempQueue->queuedTask;
                            readyQueue = dequeue(readyQueue);
                            readyQueue = insert(readyQueue, temp);   
                        }
                        if(tempQueue != NULL){
                        tempQueue = tempQueue->next;
                        }
                    }


                    printf("%3d%5c%d     ", clock, currTask->task_id, currTask->remaining_time);
                    tempQueue = readyQueue;
                    int num = 0;
                    while(tempQueue != NULL){
                        if(num == queueLength(readyQueue) - 1){
                            printf("%c%c\n", tempQueue->queuedTask->task_id, (tempQueue->queuedTask->remaining_time + 48));
                        }
                        else{ 
                            printf("%c%c, ", tempQueue->queuedTask->task_id, (tempQueue->queuedTask->remaining_time + 48));
                        }
                        num++;
                        tempQueue->queuedTask->wait_time++;
                        tempQueue = tempQueue->next;
                    }
                }
                //readyQueue is empty
                else{
                    printf("%3d%5c%d%6c%c\n", clock, currTask->task_id, currTask->remaining_time, emptyQueue, emptyQueue);
                }
                
                currTask->remaining_time--;
                clock++;
            }
            //Else, currTask requires no more processing time. Update to next process
            else{
                currTask->response_time = currTask->wait_time + currTask->service_time;
                currTask->completion_time = currTask->arrival_time + currTask->response_time;
                if(readyQueue != NULL){
                    currTask = readyQueue->queuedTask;
                }
                else{
                    currTask = currTask->next;
                }
                if(readyQueue != NULL){
                    readyQueue = dequeue(readyQueue);
                }
                tasksCompleted++;
            }
        }
        //Else no tasks have arrived at current clock cycle
        else{
            if (readyQueue != NULL){
                printf("%3d%12c%c\n", clock, readyQueue->queuedTask->task_id, (readyQueue->queuedTask->remaining_time + 48));
            }
            else{
                printf("%3d%12c%c\n", clock, emptyQueue, emptyQueue);
            }
            clock++;
        }
    }
}



//Given pointer to linked list of tasks and num of tasks, runs sim of fifo scheduling
void FIFOScheduler(struct task* head, int numTasks){
    int tasksCompleted = 0;
    int clock = 0;
    struct task* currTask = head;
    struct queue* readyQueue;
    struct task* temp;
    char emptyQueue = '-';
    struct queue* tempQueue;

    while(tasksCompleted < numTasks){
        //If currTask arrived at or before current clock cycle
        if(currTask->arrival_time <= clock){
            //If currTask requires more processing time
            if(currTask->remaining_time > 0){
                //branch for checking future tasks into ready queue
                temp = currTask->next;
                while(temp != NULL && temp->arrival_time <= clock){
                    if (readyQueue == NULL){
                        readyQueue =  malloc(sizeof(struct queue));
                        readyQueue->next = NULL;
                        readyQueue->queuedTask = temp;

                    }
                    else{
                        if (!find(readyQueue, temp)){
                            enqueue(readyQueue, temp);
                        }
                    }
                    if (temp != NULL){
                        temp = temp->next;
                    }      
                }
                //If readyQueue contains tasks
                if(readyQueue != NULL){
                    printf("%3d%5c%d     ", clock, currTask->task_id, currTask->remaining_time);
                    tempQueue = readyQueue;
                    int num = 0;
                    while(tempQueue != NULL){
                        if(num == queueLength(readyQueue) - 1){
                            printf("%c%c\n", tempQueue->queuedTask->task_id, (tempQueue->queuedTask->remaining_time + 48));
                        }
                        else{ 
                            printf("%c%c, ", tempQueue->queuedTask->task_id, (tempQueue->queuedTask->remaining_time + 48));
                        }
                        num++;
                        tempQueue->queuedTask->wait_time++;
                        tempQueue = tempQueue->next;
                    }
                }
                //readyQueue is empty
                else{
                    printf("%3d%5c%d%6c%c\n", clock, currTask->task_id, currTask->remaining_time, emptyQueue, emptyQueue);
                }
                
                currTask->remaining_time--;
                clock++;
            }
            //Else, currTask requires no more processing time. Update to next process
            else{
                currTask->response_time = currTask->wait_time + currTask->service_time;
                currTask->completion_time = currTask->arrival_time + currTask->response_time;
                currTask = currTask->next;
                if(readyQueue != NULL){
                    readyQueue = dequeue(readyQueue);
                }
                tasksCompleted++;
            }
        }
        //Else no tasks have arrived at current clock cycle
        else{
            if (readyQueue != NULL){
                printf("%3d%12c%c\n", clock, readyQueue->queuedTask->task_id, (readyQueue->queuedTask->remaining_time + 48));
            }
            else{
                printf("%3d%12c%c\n", clock, emptyQueue, emptyQueue);
            }
            clock++;
        }
    }
}

int main(int argc, char *argv[]) {

    //Check for correct number of args
    if (argc != 2) {
        printf("Incorrect number of arguments\n");
        return(1);
    }

    int numTasks = 0;
    struct task* head;
    struct task* currTask;

    int maxLineLength = 10;
    char currLine[maxLineLength]; 

    while(fgets(currLine, maxLineLength, stdin) != NULL) {
        if (numTasks != 0){ //create new task and append
            currTask->next = initData(currLine, numTasks);
            currTask = currTask->next;
            currTask->next = NULL;
            numTasks++;
        }
        else{ //init head node of linked list.
            head = initData(currLine, numTasks);
            head->next = NULL;
            currTask = head;
            numTasks++;
        }   
    }
    
    if(strcmp(argv[1], "-fifo")==0){
        printf("FIFO scheduling results\n\n");
        printTraceHeader();
        FIFOScheduler(head, numTasks);
        printSummaryData(head);
        printServiceTimeData(head);
    }
    else if(strcmp(argv[1], "-sjf")==0){
        printf("SJF(preemptive) scheduling results\n\n");
        printTraceHeader();
        SJFScheduler(head, numTasks);
        printSummaryData(head);
        printServiceTimeData(head);
    }
    else if(strcmp(argv[1], "-rr")==0){
        printf("RR scheduling results (time slice is 1)\n\n");
        printTraceHeader();
        RRScheduler(head, numTasks);
        printSummaryData(head);
        printServiceTimeData(head);
    }
    else{
        printf("Invalid scheduling policy selection\n");
        return 1;
    }

    return 0;
}