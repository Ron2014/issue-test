#include <iostream>
#include <set>
#include <map>

using namespace std;

#define VIRTUAL_COPIES 1048576 //����ڵ�������ڵ�ĸ��Ʊ���
// #define VIRTUAL_COPIES 32 //����ڵ�������ڵ�ĸ��Ʊ���

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