#include <iostream>
using namespace std;

void shell_sort(int arr[], int length)
{
        cout << "shell_sort------------------------------" << length << endl;
        
        int h = 1;
        while(h < length / 3) 
        {
                h = 3 * h + 1;      // 1-5:1, 6-14:4, 15-51:13, ...
        }
        
        while (h >= 1) 
        {
                for (int i = h; i < length; i++)
                {
                        int tmp = arr[i];       // target: minimum

                        int j;                  // find a position to insert
                        for (j = i; j >= h && tmp < arr[j-h]; j -= h) {
                                arr[j] = arr[j - h];        // move bigger backward
                        }
                        arr[j] = tmp;
                }
                
                h /= 3;
                
                for(int len = 0; len < length; len++)
                        cout << arr[len] << ",";
                cout << endl;
        }
}

int main()
{
        int arr[] = {8, 3, 9, 5, 1, 4, 7, 6, 2, 0};
        cout << "Hello, world!" << endl;
        shell_sort(arr, 10);
        
        for(int len = 0; len < 10; len++)
                cout << arr[len] << ",";
        cout << endl;
        
    return 0;
}