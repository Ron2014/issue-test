#include <iostream>
#include <set>
#include <map>

using namespace std;

// #define VIRTUAL_COPIES 1048576 //物理节点至虚拟节点的复制倍数
#define VIRTUAL_COPIES 16384 //物理节点至虚拟节点的复制倍数

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