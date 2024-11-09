#include <bits/stdc++.h>
using namespace std;
const int MaxSize = 100;

int BF(char *major, char *key)
{
	int i = 0;
    int j = 0;
	while(i < strlen(major) && j < strlen(key))
	{
		if(major[i] == key[j])
		{
            //开始匹配，标记点移动
            i++;
            j++;
        }
		else 
		{
            //匹配中断，i回到上一轮匹配开始的位置+1，j置0
            i = i - j + 1;
            j = 0;
        }
	}
	if(j >= strlen(key))
        return i - j;  //匹配成功，返回开始匹配的初始位置
	else
        return -1;  //匹配不成功，返回-1
}

int main(int argc, char **argv)
{
    char major[MaxSize],key[MaxSize];

    cout << "请输入主串：";
    cin >> major;

    cout << "请输入需要匹配的子串：";
    cin >> key;

    int flag = BF(major,key);
    if(flag == -1)
        cout << "匹配失败\n";
    else
        cout << "匹配成功，匹配位置为：" << flag << endl;
    
    return 0;
}