#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include "scheduler.h"

int compare(const void *data_1 , const void *data_2)
{
	Process *process_1 = (Process *)data_1 , *process_2 = (Process *)data_2;
	return process_1->ready_time - process_2->ready_time;
}

void assign_cpu(pid_t pid , int core)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core , &mask);

	if (sched_setaffinity(pid , sizeof(mask) , &mask) < 0)
	{
		fprintf(stderr , "Fail to assign CPU.\n");
		exit(EXIT_FAILURE);
	}

	return;
}

Queue *initialize_queue(int size)
{
	Queue *queue = (Queue *)malloc(sizeof(Queue));
	queue->head = queue->tail = 0;
	queue->size = size + 1;
	queue->data = (int *)malloc((size + 1) * sizeof(int));
	return queue;
}

void delete_queue(Queue *queue)
{
	free(queue->data);
	free(queue);
	return;
}

void insert_queue(Queue *queue , int data)
{
	queue->data[queue->tail] = data;
	queue->tail = (queue->tail + 1) % queue->size;
	return;
}

int remove_queue(Queue *queue)
{
	if (queue->head == queue->tail)
		return -1;
	
	int data = queue->data[queue->head];
	queue->head = (queue->head + 1) % queue->size;
	return data;
}

int create_process(Process process)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		fprintf(stderr , "Fail to create process\n");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		for (int i = 0 ; i < process.execution_time ; i++)
			for (volatile unsigned long int i = 0 ; i < 1000000UL ; i++);
		exit(EXIT_SUCCESS);
	}

	assign_cpu(pid , 1);

	return pid;
}

void block_process(Process process)
{
	struct sched_param parameter;
	parameter.sched_priority = 0;
	sched_setscheduler(process.pid , SCHED_IDLE , &parameter);
	return;
}

void wakeup_process(Process process)
{
	struct sched_param parameter;
	parameter.sched_priority = 0;
	sched_setscheduler(process.pid , SCHED_OTHER , &parameter);
	return;
}

int select_process(int number_of_process , Process *process , char policy[MAX_LENGTH_OF_POLICY] , int execution , int index , int use , Queue *queue)
{
	if (strcmp(policy , "FIFO") == 0)
	{
		if (execution)
			return index;
		
		for (int i = 0 ; i < number_of_process && process[i].pid != -1 ; i++)
			if (process[i].execution_time > 0)
				return i;

		return -1;
	}
	else if (strcmp(policy , "RR") == 0)
	{
		if (!execution)
			return remove_queue(queue);
		else
		{
			if (use % 500 != 0)
				return index;
			else
			{
				insert_queue(queue , index);
				return remove_queue(queue);
			}
		}
	}
	else if (strcmp(policy , "SJF") == 0)
	{
		if (execution)
			return index;

		int min_execution_time = 2147483647 , min_index = -1;
		for (int i = 0 ; i < number_of_process && process[i].pid != -1 ; i++)
			if (process[i].execution_time > 0 && process[i].execution_time < min_execution_time)
			{
				min_execution_time = process[i].execution_time;
				min_index = i;
			}

		return min_index;
	}
	else if (strcmp(policy , "PSJF") == 0)
	{
		int min_execution_time = 2147483647 , min_index = -1;
		for (int i = 0 ; i < number_of_process && process[i].pid != -1 ; i++)
			if (process[i].execution_time > 0 && process[i].execution_time < min_execution_time)
			{
				min_execution_time = process[i].execution_time;
				min_index = i;
			}

		return min_index;
	}
	else
	{
		fprintf(stderr , "Invalid scheduler policy.\n");
		exit(EXIT_FAILURE);
	}
}

void scheduler(int number_of_process , Process *process , char policy[MAX_LENGTH_OF_POLICY])
{
	setvbuf(stdout , 0 , _IONBF , 0);

	struct sched_param parameter;
	parameter.sched_priority = 0;

	assign_cpu(getpid() , 0);
	sched_setscheduler(getpid() , SCHED_OTHER , &parameter);

	for (int i = 0 ; i < number_of_process ; i++)
		process[i].pid = -1;
	qsort(process , number_of_process , sizeof(Process) , compare);

	Queue *queue = initialize_queue(number_of_process);
	int time = 0 , execution = 0 , index = -1 , next_index , use , ready = 0 , finish = 0;
	char message[128] = {0};

	while (1)
	{
		if (execution && process[index].execution_time == 0)
		{
			#if DEBUG
				fprintf(stderr , "Finish %s at time %d.\n" , process[index].process_name , time);
			#endif
			waitpid(process[index].pid , 0 , 0);
			syscall(333 , &(process[index].end_time[0]) , &(process[index].end_time[1]));
			sprintf(message , "[Project1] %d %lu.%09lu %lu.%09lu" , process[index].pid , process[index].start_time[0] , process[index].start_time[1] , process[index].end_time[0] , process[index].end_time[1]);
			syscall(334 , message);
			execution = 0;
			index = -1;
			finish++;
			if (finish == number_of_process)
				break;
		}

		for ( ; ready < number_of_process && process[ready].ready_time == time ; ready++)
		{
			#if DEBUG
				fprintf(stderr , "Create %s at time %d.\n" , process[ready].process_name , time);
			#endif
			syscall(333 , &(process[ready].start_time[0]) , &(process[ready].start_time[1]));
			process[ready].pid = create_process(process[ready]);
			block_process(process[ready]);
			insert_queue(queue , ready);
			printf("%s %d\n" , process[ready].process_name , process[ready].pid);
		}

		next_index = select_process(number_of_process , process , policy , execution , index , use , queue);
		if (next_index != -1)
		{
			#if DEBUG
				if (index == -1)
					fprintf(stderr , "Start %s at time %d.\n" , process[next_index].process_name , time);
				if (next_index != index && index != -1)
					fprintf(stderr , "Context switch from %s to %s at time %d.\n" , process[index].process_name , process[next_index].process_name , time);
			#endif
			if (index != -1)
				block_process(process[index]);
			wakeup_process(process[next_index]);
			process[next_index].execution_time--;
			use = (next_index == index)? use + 1 : 1;
			execution = 1;
			index = next_index;
		}
		else
		{
			execution = 0;
			index = -1;
			use = 0;
		}

		for (volatile unsigned long int i = 0 ; i < 1000000UL ; i++);
		time++;
	}

	delete_queue(queue);
	return;
}
