
//client.c
#include"comm.h"
 
int main()
{
	int shmid = GetShm(4096);
	sleep(1);

	char *addr = shmat(shmid,NULL,0);	// �������ڴ����������ǰ���̵�ַ�ռ�

	sleep(2);
	int i = 0;
	while(i < 26)
	{
		addr[i] = 'A' + i;
		i++;
		addr[i] = 0;
		sleep(1);
	}

	shmdt(addr);						// �������ڴ���뵱ǰ��������
	
	sleep(2);
	return 0;
}