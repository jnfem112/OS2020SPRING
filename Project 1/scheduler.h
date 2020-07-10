#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <sys/types.h>

#define MAX_LENGTH_OF_POLICY 8
#define MAX_LENGTH_OF_PROCESS_NAME 32

struct Process
{
	char process_name[MAX_LENGTH_OF_PROCESS_NAME];
	int ready_time , execution_time;
	pid_t pid;
	unsigned long int start_time[2] , end_time[2];
};

struct Queue
{
	int head , tail , size , *data;
};

typedef struct Process Process;
typedef struct Queue Queue;

int compare(const void *data_1 , const void *data_2);
void assign_cpu(pid_t pid , int core);
Queue *initialize_queue(int size);
void delete_queue(Queue *queue);
void insert_queue(Queue *queue , int data);
int remove_queue(Queue *queue);
int create_process(Process process);
void block_process(Process process);
void wakeup_process(Process process);
int select_process(int number_of_process , Process *process , char policy[MAX_LENGTH_OF_POLICY] , int execution , int index , int use , Queue *queue);
void scheduler(int number_of_process , Process *process , char policy[MAX_LENGTH_OF_POLICY]);

#endif
