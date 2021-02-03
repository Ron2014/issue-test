
//server.c
#include"comm.h"
 
int main()
{
	int shmid = CreateShm(4096);
 
	char *addr = shmat(shmid,NULL,0);	// 将共享内存段连接至当前进程地址空间

	sleep(2);
	int i = 0;
	while(i++ < 26)
	{
		printf("client# %s\n",addr);
		sleep(1);
	}

	shmdt(addr);						// 将共享内存段与当前进程脱离

	sleep(2);

	DestroyShm(shmid);

	return 0;
}