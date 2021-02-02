/**
 * @author: https://kefeng.wang
 * @date: 2018-08-10 11:08
 **/
#include <iostream>
#include "ConsistentHashing.h"
using namespace std;

int main(int argc, char** args)
{
    CConsistentHashing* ch = new CConsistentHashing();

    cout << "------" << endl;

    // ��ʼ���
    ch->dumpObjectNodeMap("init info", 0, 65536);

    // ɾ������ڵ�
    ch->removePhysicalNode("192.168.1.103");
    ch->dumpObjectNodeMap("after remove", 0, 65536);

    // �������ڵ�
    ch->addPhysicalNode("192.168.1.108");
    ch->dumpObjectNodeMap("after add", 0, 65536);

    return 0;
}