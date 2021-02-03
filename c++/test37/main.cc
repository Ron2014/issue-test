#include <iostream>
#include "ConsistentHashing.h"

using namespace std;

int main(int argc, char** args)
{
    CConsistentHashing* ch = new CConsistentHashing();

    cout << "------ init for " << VIRTUAL_COPIES << " times " << endl;

    ch->addPhysicalNode("192.168.1.101");
    ch->addPhysicalNode("192.168.1.102");
    ch->addPhysicalNode("192.168.1.103");
    ch->addPhysicalNode("192.168.1.104");
    ch->addPhysicalNode("192.168.1.105");
    ch->addPhysicalNode("192.168.1.106");

    // ��ʼ���
    ch->dumpObjectNodeMap("origin info1", 0, 65536);
    ch->dumpObjectNodeMap("origin info2", 0, 65536);

    // ɾ������ڵ�
    ch->removePhysicalNode("192.168.1.103");
    ch->dumpObjectNodeMap("remove 192.168.1.103", 0, 65536);

    // �������ڵ�
    ch->addPhysicalNode("192.168.1.103");
    ch->dumpObjectNodeMap("add 192.168.1.103", 0, 65536);

    // �������ڵ�
    ch->addPhysicalNode("192.168.1.107");
    ch->dumpObjectNodeMap("add 192.168.1.107", 0, 65536);

    cout << "press any key to exit" << endl;
    getchar();

    return 0;
}