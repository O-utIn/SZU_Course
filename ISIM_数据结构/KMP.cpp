#include <bits/stdc++.h>
using namespace std;
const int MaxSize = 100;

void get_next(char *key,int *next)
{

    next[0] = -1;
    int j = 0,k = -1;
    while(j < strlen(key))
    {
        if(k == -1 || key[j] ==key[k])
        {
            j++;
            k++;
            if(key[j] != key[k])
                next[j] = k;
            else
                next[j] = next[k];
        }
        else
            k = next[k];
    }
}

int KMP(char *major,char *key)
{
    int next[MaxSize];
    int i = 0,j = 0;
    get_next(key,next);
    while(i < strlen(major) && j < strlen(key))
    {
        if(j == -1 || major[i] == key[j])
        {
            i++;
            j++;
        }
        else
            j = next[j];
    }
    if(j >= strlen(key))
        return i - strlen(key);
    else
        return -1;

}


int main(int argc, char **argv)
{
    char major[MaxSize],key[MaxSize];

    cout << "请输入主串：";
    cin >> major;

    cout << "请输入需要匹配的子串：";
    cin >> key;

    int flag = KMP(major,key);
    if(flag == -1)
        cout << "匹配失败\n";
    else
        cout << "匹配成功，匹配位置为：" << flag << endl;
    
    return 0;
}