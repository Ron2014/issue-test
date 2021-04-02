/*
 * @Author: Ron
 * @LastEditors: Ron
 * @FilePath: \issue-test\c++\test40\test.cc
 */
#include <iostream>
using namespace std;

typedef unsigned char BOOL;

enum AddTeam_Code
{
    AddTeam_Success = 0,      //�ɹ�
    AddTeam_DisAgree = 3,     //��ͬ��
    AddTeam_TargetInTeam = 4, //�Է����ڶ�����
    AddTeam_Full = 10005,
    AddTeam_NoTeam = 5,
    AddTeam_InMatching = 9,        //��������ƥ������
    AddTeam_TargetInMatching = 10, //�Է�����ƥ������
    AddTeam_InGame = 10009,        //����������Ϸ����
    AddTeam_Exception = 10010,
};

int main(int argc, char *argv[])
{
    int a = 2147483647;
    cout << a + 1 << endl;
    printf("%d\n", a + 1);

    cout << sizeof(BOOL) << endl;
    cout << sizeof(bool) << endl;
    cout << sizeof(AddTeam_Code) << endl;

    return 0;
}