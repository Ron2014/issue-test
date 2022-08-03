#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

void swap(vector<string> &vecWords, int i, int j)
{
    if (i == j)
    {
        return;
    }
    string tmp = vecWords[i];
    vecWords[i] = vecWords[j];
    vecWords[j] = tmp;
}

void show_permutation(vector<string> &vecWords)
{
    ostringstream oss;
    for (string &str : vecWords)
    {
        oss << str << " ";
    }
    cout << oss.str() << endl;
}

void full_permutation(vector<string> &vecWords, int iCount)
{
    if (iCount <= 0)
    {
        return show_permutation(vecWords);
    }

    for (int i = 0; i < iCount; i++)
    {
        swap(vecWords, i, iCount - 1);
        full_permutation(vecWords, iCount - 1);
        swap(vecWords, i, iCount - 1);
    }
}

int main(int argc, const char *argv[])
{
    string szWord;
    vector<string> vecWords;

    while (getline(cin, szWord)) {
        if (szWord[szWord.length()-1] < 0x20) {
            szWord = szWord.substr(0, szWord.length()-1); // remove CR
        }
        if (szWord.empty())
        {
            break;
        }
        vecWords.push_back(szWord);
    }
    full_permutation(vecWords, vecWords.size());
    return 0;
}