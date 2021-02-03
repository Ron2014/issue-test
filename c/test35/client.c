
//client.c
#include"comm.h"
 
int main()
{
	int shmid = GetShm(4096);
	sleep(1);

	char *addr = shmat(shmid,NULL,0);	// 将共享内存段连接至当前进程地址空间

	sleep(2);
	int i = 0;
	while(i < 26)
	{
		addr[i] = 'A' + i;
		i++;
		addr[i] = 0;
		sleep(1);
	}

	shmdt(addr);						// 将共享内存段与当前进程脱离
	
	sleep(2);
	return 0;
}