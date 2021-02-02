#include <iostream>
#include <set>
#include <map>

using namespace std;

#define VIRTUAL_COPIES 1048576 //物理节点至虚拟节点的复制倍数
// #define VIRTUAL_COPIES 32 //物理节点至虚拟节点的复制倍数

class CConsistentHashing {
private:
    static set<string> physicalNodes;
    map<long, string> virtualNodes;

public:
    CConsistentHashing();

    static long FNVHash(string key);
    static long FNVHash(const unsigned char* p, size_t size);

    void addPhysicalNode(string nodeIp);
    void removePhysicalNode(string nodeIp);
    string getObjectNode(string object);
    void dumpObjectNodeMap(string label, int objectMin, int objectMax);
};