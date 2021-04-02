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
    AddTeam_Success = 0,      //成功
    AddTeam_DisAgree = 3,     //不同意
    AddTeam_TargetInTeam = 4, //对方正在队伍中
    AddTeam_Full = 10005,
    AddTeam_NoTeam = 5,
    AddTeam_InMatching = 9,        //队伍正在匹配中了
    AddTeam_TargetInMatching = 10, //对方正在匹配中了
    AddTeam_InGame = 10009,        //队伍正在游戏中了
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