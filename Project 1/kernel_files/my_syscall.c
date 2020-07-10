#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

asmlinkage void get_time(unsigned long int time[2])
{
	struct timespec temp;
	getnstimeofday(&temp);
	time[0] = temp.tv_sec;
	time[1] = temp.tv_nsec;
	return;
}

asmlinkage void print_to_kernel(char *message)
{
	printk("%s\n" , message);
	return;
}