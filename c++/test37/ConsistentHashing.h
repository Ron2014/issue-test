#include <iostream>
#include <set>
#include <map>

using namespace std;

// #define VIRTUAL_COPIES 1048576 //����ڵ�������ڵ�ĸ��Ʊ���
#define VIRTUAL_COPIES 16384 //����ڵ�������ڵ�ĸ��Ʊ���

class CConsistentHashing {
private:
    map<long, string> virtualNodes;
    map<int, string> nodesRecords;

public:
    static long FNVHash(string key);

    void addPhysicalNode(string nodeIp);
    void removePhysicalNode(string nodeIp);
    string getObjectNode(string object);
    void dumpObjectNodeMap(string label, int objectMin, int objectMax);
};