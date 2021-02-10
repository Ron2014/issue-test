#include <iostream>
#include <vector>
#include <map>
using namespace std;

#define UINT unsigned int
typedef vector< vector<UINT> > PartitionList;
typedef vector< map<UINT, UINT> > PartitionMap;

class CIntegerPartition
{
public:
    UINT Value;
    PartitionMap Result;
    void Update(UINT value);
    void ShowResult();
};