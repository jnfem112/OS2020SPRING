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

int main(int argc, char *argv[])
{
	char buf[BUF_SIZE];
	int dev_fd, file_fd, num_of_file = atoi(argv[1]); // the fd for the device and the fd for the input file
	size_t ret, file_size = 0, total_file_size = 0, data_size = -1, offset;
	char file_name[num_of_file][50];
	char method[20];
	char ip[20];
	struct timeval start;
	struct timeval end;
	double trans_time, total_trans_time = 0; //calulate the time between the device is opened and it is closed
	void *src, *dst;
	char *kernel_address, *file_address;

	for (int i = 0; i < num_of_file; i++)
	{
		strcpy(file_name[i], argv[i + 2]);
	}
	strcpy(method, argv[num_of_file + 2]);
	strcpy(ip, argv[num_of_file + 3]);

	for (int i = 0; i < num_of_file; i++)
	{
		if ((dev_fd = open("/dev/slave_device", O_RDWR)) < 0) //should be O_RDWR for PROT_WRITE when mmap()
		{
			perror("failed to open /dev/slave_device\n");
			return 1;
		}

		gettimeofday(&start ,NULL);

		if ((file_fd = open(file_name[i], O_RDWR | O_CREAT | O_TRUNC)) < 0)
		{
			fprintf(stderr, "failed to open input file\n");
			return 1;
		}

		if (ioctl(dev_fd, 0x12345677, ip) == -1)	//0x12345677 : connect to master in the device
		{
			perror("ioclt create slave socket error\n");
			return 1;
		}

		//write(1, "ioctl success\n", 14);

		file_size = 0;
		switch (method[0])
		{
			case 'f'://fcntl : read()/write()
				do
				{
					ret = read(dev_fd, buf, sizeof(buf)); //read from the the device
					write(file_fd, buf, ret); //write to the input file
					file_size += ret;
				} while(ret > 0);
				break;

			case 'm':
				src = mmap(NULL, MAP_SIZE, PROT_READ, MAP_SHARED, dev_fd, 0);
				while (1)
				{
					while ((ret = ioctl(dev_fd, 0x12345678)) < 0 && errno == EAGAIN);

					if (ret < 0)
					{
						perror("ioctl error\n");
						return 1;
					}
					else if (ret == 0)
						break;
					else
					{
						offset = (size_t)(file_size / PAGE_SIZE) * PAGE_SIZE;

						posix_fallocate(file_fd, file_size, ret);
						dst = mmap(NULL, ret, PROT_WRITE, MAP_SHARED, file_fd, offset);
						memcpy(dst, src, ret);
						munmap(dst, ret);

						file_size += ret;
					}
				}
				ftruncate(file_fd, file_size);

				if (ioctl(dev_fd, 0, src) == -1)
				{
					perror("ioctl error\n");
					return 1;
				}

				munmap(src, MAP_SIZE);
				break;
		}

		if (ioctl(dev_fd, 0x12345679) == -1)// end receiving data, close the connection
		{
			perror("ioclt client exits error\n");
			return 1;
		}

		gettimeofday(&end, NULL);
		trans_time = (end.tv_usec - start.tv_usec) * 0.0001 + (end.tv_sec - start.tv_sec) * 1000;
		total_trans_time += trans_time;
		total_file_size += file_size;

		close(file_fd);
		close(dev_fd);
	}

	printf("Transmission time: %lf ms, File size: %ld bytes\n", total_trans_time, total_file_size);
	return 0;
}
