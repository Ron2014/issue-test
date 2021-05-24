/*
 * @Author: Ron
 * @LastEditors: Ron
 * @FilePath: \issue-test\c++\test45\main.cc
 */
#include <iostream>
using namespace std;

#include "define.h"

int main(int argc, char *argv[])
{
    printf("size of UnCompressMsgHeader = %zd\n", sizeof(UnCompressMsgHeader));
    printf("size of CompressMsgHeader = %zd\n", sizeof(CompressMsgHeader));

    BattleRecordDetailsShm arr[150];
    BattleRecordDetails data;
    BattleRecordDetailsShm tmp = {0};
    tmp = data;
    arr[150] = tmp;
    return 0;
}