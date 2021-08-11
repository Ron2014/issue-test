/*
 * @Date: 2021-06-22 17:04:44
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-19 21:23:14
 * @FilePath: \issue-test\c++\test54\main.cc
 * @Description: 
 */

#include <iostream>
using namespace std;

enum ENUM1
{
	ENUM1_V1,
	ENUM1_V2,
	ENUM1_V3,
	ENUM1_V4,
	ENUM1_V5,
};

enum ENUM2
{
	ENUM2_V1 = 0,
	ENUM2_V2,
	ENUM2_V3,
	ENUM2_V4,
	ENUM2_V5,
};

enum ENUM3
{
	ENUM3_V1 = 0,
	ENUM3_V2,
	ENUM3_V3,
	ENUM3_V4 = 1000,
	ENUM3_V5,
};

enum ENUM4
{
	ENUM4_V1 = -10,
	ENUM4_V2,
	ENUM4_V3,
	ENUM4_V4 = 1000,
	ENUM4_V5,
};

enum ENUM5
{
	ENUM5_V1,
	ENUM5_V2 = -10,
	ENUM5_V3,
	ENUM5_V4 = 10,
	ENUM5_V5,
};

#define SHOW_ALL_ENUMS(idx) \
	printf("Enum%d(%zd): %d %d %d %d %d\n", idx, sizeof(enum ENUM##idx), ENUM##idx::ENUM##idx##_V1, ENUM##idx::ENUM##idx##_V2, ENUM##idx::ENUM##idx##_V3, ENUM##idx::ENUM##idx##_V4, ENUM##idx::ENUM##idx##_V5)

// #define IS_UNSIGNED(a) (a >= 0 && ~a >= 0)
#define IS_UNSIGNED(a) ((a |= (0x1 << (sizeof(a)*8 - 1))) > 0 ? 1 : 0)

int main(int argc, char *argv[])
{
	SHOW_ALL_ENUMS(1);
	SHOW_ALL_ENUMS(2);
	SHOW_ALL_ENUMS(3);
	SHOW_ALL_ENUMS(4);
	SHOW_ALL_ENUMS(5);

	enum ENUM1 val;
	int *ival = (int *)(&val);
	*ival = 0x1 << (sizeof(int)*8 - 1);
	printf("%d\n", *ival);
	
	int temp0 = 100;
	if (val > temp0)
	{
		printf("default unsigned\n");
	}
	else
	{
		printf("default signed\n");
	}
	
	unsigned int temp1 = 100;
	if (val > temp1)
	{
		printf("default unsigned\n");
	}
	else
	{
		printf("default signed\n");
	}
	return 0;
}