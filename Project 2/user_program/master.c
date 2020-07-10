#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define PAGE_SIZE 4096
#define MAP_SIZE (1 * PAGE_SIZE)
#define BUF_SIZE 512
size_t get_filesize(const char *filename);//get the size of the input file

int main(int argc, char *argv[])
{
	char buf[BUF_SIZE];
	int dev_fd, file_fd; // the fd for the device and the fd for the input file
	size_t ret, file_size, total_file_size = 0, offset = 0, tmp;
	int number_of_file = atoi(argv[1]);
	char file_name[number_of_file][50] , method[20];
	size_t mmap_size;
	void *src, *dst;
	char *kernel_address = NULL, *file_address = NULL;
	struct timeval start;
	struct timeval end;
	double trans_time, total_trans_time = 0; //calulate the time between the device is opened and it is closed

	for (int i = 0; i < number_of_file; i++)
		strcpy(file_name[i], argv[i + 2]);
	strcpy(method , argv[number_of_file + 2]);

	for (int i = 0; i < number_of_file; i++)
	{
		if ((dev_fd = open("/dev/master_device", O_RDWR)) < 0)
		{
			perror("failed to open /dev/master_device\n");
			return 1;
		}

		gettimeofday(&start, NULL);

		if ((file_fd = open(file_name[i], O_RDWR)) < 0)
		{
			perror("failed to open input file\n");
			return 1;
		}

		if ((file_size = get_filesize(file_name[i])) < 0)
		{
			perror("failed to get filesize\n");
			return 1;
		}

		if (ioctl(dev_fd, 0x12345677) == -1) //0x12345677 : create socket and accept the connection from the slave
		{
			perror("ioclt server create socket error\n");
			return 1;
		}

		switch (method[0])
		{
			case 'f': //fcntl : read()/write()
				do
				{
					ret = read(file_fd, buf, sizeof(buf)); // read from the input file
					write(dev_fd, buf, ret); //write to the the device
				} while (ret > 0);
				break;

			case 'm':
				dst = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, 0);
				for (int j = 0; j * MAP_SIZE < file_size; j++)
				{
					// map memory of size MAP_SIZE every loop
					// if remaining file size is less than MAP_SIZE, then let mmap_size be the remaining file size
					mmap_size = (file_size - j * MAP_SIZE > MAP_SIZE)? MAP_SIZE : file_size - j * MAP_SIZE;
					src = mmap(NULL, mmap_size, PROT_READ, MAP_SHARED, file_fd, j * MAP_SIZE);
					memcpy(dst, src, mmap_size);
					munmap(src, mmap_size);
					while (ioctl(dev_fd, 0x12345678, mmap_size) < 0 && errno == EAGAIN);
				}

				if (ioctl(dev_fd, 0, dst) == -1)
				{
					perror("ioclt server error\n");
					return 1;
				}

				munmap(dst, MAP_SIZE);
				break;
			}

		if(ioctl(dev_fd, 0x12345679) == -1) // end sending data, close the connection
		{
			perror("ioclt server exits error\n");
			return 1;
		}
		gettimeofday(&end, NULL);
		trans_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) * 0.0001;
		total_trans_time += trans_time;
		total_file_size += file_size;

		close(file_fd);
		close(dev_fd);
	}

	printf("Transmission time: %lf ms, File size: %lu bytes\n", total_trans_time, total_file_size);
	return 0;
}

size_t get_filesize(const char *filename)
{
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}
