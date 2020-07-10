#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

int main(int argc , char **argv)
{
	char policy[MAX_LENGTH_OF_POLICY] = {0};
	int number_of_process;
	Process *process;

	scanf("%s%d" , policy , &number_of_process);
	process = (Process *)malloc(number_of_process * sizeof(Process));
	for (int i = 0 ; i < number_of_process ; i++)
		scanf("%s%d%d" , process[i].process_name , &(process[i].ready_time) , &(process[i].execution_time));

	scheduler(number_of_process , process , policy);

	free(process);
	return 0;
}
