#include "IntegerPartition.h"
#include <assert.h>

void CIntegerPartition::ShowResult()
{
    cout << Value << " = " << endl;
    for (auto &map: Result)
    {
        for (auto &it : map)
            cout << " " << it.first << ":" << it.second << " ";
        cout << endl;
    }
}

void CIntegerPartition::Update(UINT value)
{
    assert(value);

    UINT iCountOfApple = value;
    UINT iCountOfBasket = value;
    vector<UINT> result;

    PartitionList **aStoreWays = new PartitionList*[iCountOfApple+1];
    for (UINT i=0; i<=iCountOfApple; i++)
        aStoreWays[i] = new PartitionList[iCountOfBasket+1];

    for (UINT i=1; i<=iCountOfApple; i++)
    {
        for (UINT j=1; j<=iCountOfBasket; j++)
        {
            if ( i==1 )
            {
                // 1个苹果放在任意一个盘子
                result.clear();
                result.push_back(1);
                aStoreWays[i][j].push_back(result);
            }
            else if ( j==1 )
            {
                // 任意苹果放在一个盘子里
                result.clear();
                result.push_back(i);
                aStoreWays[i][j].push_back(result);
            }
            else if (j > i)
            {
                // 盘子比苹果多
                aStoreWays[i][j] = aStoreWays[i][j-1];
            }
            else if (j == i)
            {
                // 至少有1个盘子为空的情况
                for (auto &node: aStoreWays[i][j-1])
                    aStoreWays[i][j].push_back(node);

                // 没有空盘子，那么都是 1
                result.clear();
                for(UINT k=0; k<j; k++)
                    result.push_back(1);
                aStoreWays[i][j].push_back(result);
            }
            else // j < i
            {
                // 至少有1个盘子为空的情况
                for (auto &node: aStoreWays[i][j-1])
                    aStoreWays[i][j].push_back(node);

                // 所有盘子都不为空（至少1个苹果的情况）
                // 即，先铺满一层，消耗了 j 个苹果；再对剩下的 i-j 个苹果进行拆分
                for (auto &node: aStoreWays[i-j][j])
                {
                    result.clear();
                    for (auto &it: node)
                        result.push_back(it+1);

                    UINT left = j-node.size();      // i - ( i - j + node.size() )
                    for (UINT k = 0; k < left; k++)
                        result.push_back(1);

                    aStoreWays[i][j].push_back(result);
                }
            }
        }
    }

    Value = value;
    Result.clear();

    cout << Value << " = " << endl;

    map<UINT, UINT> solution;
    for (auto &list: aStoreWays[value][value])
    {
        solution.clear();
        for (auto &index: list)
        {
            UINT count = solution[index];
            solution[index] = count + 1;
            cout << " " << index;
        }
        Result.push_back(solution);
        cout << endl;
    }

    for (int i = 0; i <= iCountOfApple; i++)
        delete[] aStoreWays[i];
    delete[] aStoreWays;
}