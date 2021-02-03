
//comm.c
#include"comm.h"
 
static int CommShm(int size, int flags)
{
	key_t key = ftok(PATHNAME, PROJ_ID);		// ��һ���Ѵ��ڵ�·������һ��������ʶ��ת����һ��key_tֵ����ΪIPC��ֵ key
	if(key < 0)
	{
		perror("ftok");
		return -1;
	}

	int shmid = 0;
	if((shmid = shmget(key, size, flags)) < 0)	// ��semget��msgget��shmget����keyֵ�����Ƿ��صĶ�����Ӧ��IPC�����ʶ�� shmid
	{
		perror("shmget");
		return -2;
	}

	return shmid;
}

int DestroyShm(int shmid)
{
	if(shmctl(shmid, IPC_RMID, NULL) < 0)
	{
		perror("shmctl");
		return -1;
	}
	return 0;
}

/**
 
IPC_CREAT	// Create key if key does not exist.
IPC_EXCL   	// Fail if key exists.

ָ��keyΪIPC_PRIVATE����ϵͳ��֤����һ��Ψһ��IPC����
����flag������IPC_CREATλ������������IPC_EXCLλʱ�������ָ��key����IPC���󲻴��ڣ��Ǿ��Ǵ���һ���µĶ��󣻷��򷵻ظö���
ͬʱ����flag��IPC_CREAT��IPC_EXCLλʱ�������ָ��key����IPC���󲻴��ڣ��Ǿʹ���һ���µĶ��󣻷��򷵻�һ��EEXIST������Ϊ�ö����Ѵ��ڡ�

0666 �� IPC �Ķ�дȨ��

*/

int CreateShm(int size)
{
	return CommShm(size, IPC_CREAT | IPC_EXCL | 0666);
	//return CommShm(size, IPC_CREAT | 0666);
}

int GetShm(int size)
{
	return CommShm(size, IPC_CREAT);
}