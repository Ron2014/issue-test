#include "ConsistentHashing.h"
#include <sstream>
#include <math.h>

set<string> CConsistentHashing::physicalNodes {
    "193.168.1.101",
    "192.168.1.102",
    "192.168.1.103",
    "192.168.1.104",
};

long
CConsistentHashing::FNVHash(string key)
{
    static int p = 16777619;

    long hash = 2166136261L;
    for (int idx = 0, num = key.length(); idx < num; ++idx)
    {
        hash = (hash ^ key[idx]) * p;
    }
    hash += hash << 13;
    hash ^= hash >> 7;
    hash += hash << 3;
    hash ^= hash >> 17;
    hash += hash << 5;

    if (hash < 0)
    {
        hash = abs(hash);
    }
    return hash;
}

CConsistentHashing::CConsistentHashing() {
    for (string nodeIp : physicalNodes)
    {
        addPhysicalNode(nodeIp);
        cout << "====== init node " <<  nodeIp << endl;
    }
}

void
CConsistentHashing::addPhysicalNode(string nodeIp)
{
    for (int idx = 0; idx < VIRTUAL_COPIES; ++idx)
    {
        long hash = FNVHash(nodeIp + "#" + to_string(idx));
        virtualNodes.insert(make_pair(hash, nodeIp));
    }
}

void
CConsistentHashing::removePhysicalNode(string nodeIp)
{
    for (int idx = 0; idx < VIRTUAL_COPIES; ++idx)
    {
        long hash = FNVHash(nodeIp + "#" + to_string(idx));
        virtualNodes.erase(hash);
    }
}

// ���Ҷ���ӳ��Ľڵ�
string
CConsistentHashing::getObjectNode(string object)
{
    long hash = FNVHash(object);

    // map ����˳��Ĭ�� key ����
    map<long, string>::iterator tailMap = virtualNodes.upper_bound(hash); // �����׸����� key �ĵ�����

    if (tailMap == virtualNodes.end())
    {
        return virtualNodes.begin()->second;
    }

    return tailMap->second;
}

// ͳ�ƶ�����ڵ��ӳ���ϵ
void
CConsistentHashing::dumpObjectNodeMap(string label, int objectMin, int objectMax)
{
    // ͳ��
    map<string, int> objectNodeMap; // IP => COUNT
    for (int object = objectMin; object <= objectMax; ++object)
    {
        string nodeIp = getObjectNode(to_string(object));
        // cout << nodeIp << endl;
        int count = objectNodeMap[nodeIp];
        objectNodeMap[nodeIp] = count + 1;
    }

    // ��ӡ
    double totalCount = objectMax - objectMin + 1;
    cout << "======== " << label << " ========" << endl;

    for (map<string, int>::iterator entry = objectNodeMap.begin(); entry != objectNodeMap.end(); entry ++)
    {
        double percent = (100.0 * entry->second / totalCount);
        cout << "IP=" << entry->first << ": RATE=" << percent << "%" << endl;
    }
}