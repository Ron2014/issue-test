
//server.c
#include"comm.h"
 
int main()
{
	int shmid = CreateShm(4096);
 
	char *addr = shmat(shmid,NULL,0);	// �������ڴ����������ǰ���̵�ַ�ռ�

	sleep(2);
	int i = 0;
	while(i++ < 26)
	{
		printf("client# %s\n",addr);
		sleep(1);
	}

	shmdt(addr);						// �������ڴ���뵱ǰ��������

	sleep(2);

	DestroyShm(shmid);

	return 0;
}