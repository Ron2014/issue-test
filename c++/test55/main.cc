/*
 * @Date: 2021-06-24 18:06:28
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-05 10:56:24
 * @FilePath: \issue-test\c++\test55\main.cc
 * @Description: 
 */
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

char mBuffer[MAX_LEN_REDIS_KEY];

struct A
{
	virtual void foo0() = 0;
};

struct B : public A
{
};

class C
{
public:
	virtual void foo0() = 0;
};

class D : public C
{
};

class E
{
public:
	virtual void foo(int a)
	{
		cout << "E:foo." << a << endl;
	}
};

class F: public E
{
public:
	virtual void foo(int a, int b)
	{
		cout << "F:foo." << a << "." << b << endl;
	}
};

int main(int argc, char *argv[])
{
	for (int i=0; i<2000*60; i++)
	{
		int j = i;
	}

	F f;
	f.foo(1, 2);
	f.foo(1);

	return 0;
}