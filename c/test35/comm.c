/*
 * @Date: 2021-01-15 15:56:23
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-08-11 10:27:12
 * @Description: 
 */

//comm.c
#include"comm.h"
 
static int CommShm(int size, int flags)
{
	key_t key = ftok(PATHNAME, PROJ_ID);		// 将一个已存在的路径名和一个整数标识符转换成一个key_t值，称为IPC键值 key
	if(key < 0)
	{
		perror("ftok");
		return -1;
	}

	int shmid = 0;
	if((shmid = shmget(key, size, flags)) < 0)	// 给semget、msgget、shmget传入key值，它们返回的都是相应的IPC对象标识符 shmid
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

指定key为IPC_PRIVATE操作系统保证创建一个唯一的IPC对象。
设置flag参数的IPC_CREAT位但不设置它的IPC_EXCL位时，如果所指定key键的IPC对象不存在，那就是创建一个新的对象；否则返回该对象。
同时设置flag的IPC_CREAT和IPC_EXCL位时，如果所指定key键的IPC对象不存在，那就创建一个新的对象；否则返回一个EEXIST错误，因为该对象已存在。

0666 此 IPC 的读写权限

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