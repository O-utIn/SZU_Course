#include<stdio.h>
#include <string.h>
#define N 20

int main()
{
	char country[N][12] = {"美国","日本","韩国","德国","越南","澳大利亚","马来西亚","巴西","俄罗斯","泰国","印度","印度尼西亚","荷兰","英国","新加坡","沙特阿拉伯","墨西哥","法国","菲律宾","加拿大"};
	int import[N] = {17970081,20550277,21344326,11991430,9231942,16352908,9830508,11000670,7960907,6183263,2814551,6392283,1400494,2567686,3881973,5696861,1913928,3910500,2476131,3048978};
	int exports[N] = {57607523,16581426,14880548,11517540,13789507,6638009,7865488,5361123,6755117,6935458,9750751,6064749,10243171,8702212,5510346,3032124,6743974,4593066,5151026,5151026};
	float IERatio[N];
	
	int i,j,temp;
	char tempCountry[12];
    float tempRatio;
    
	for(i = 0; i < N; i++)
    	IERatio[i] = (float)import[i] / exports[i];

//	// 打印出每个国家的进出口交换比率
//	for (i = 0; i < N; i++)
//    	printf("%2d %12s 的进出口交换比率为: %f\n",i+1, country[i], IERatio[i]);

	
    	
    	
    // 冒泡排序
    for (i = 0; i < N-1; i++)
	{
        for (j = 0; j < N-i-1; j++)
		{
            if (import[j] < import[j+1])
			{
                temp = import[j];
                import[j] = import[j+1];
                import[j+1] = temp;

                strcpy(tempCountry, country[j]);
                strcpy(country[j], country[j+1]);
                strcpy(country[j+1], tempCountry);

                temp = exports[j];
                exports[j] = exports[j+1];
                exports[j+1] = temp;

                tempRatio = IERatio[j];
                IERatio[j] = IERatio[j+1];
                IERatio[j+1] = tempRatio;
            }
        }
    }
    
    // 打印冒泡排序后的结果
    printf("按进口额从多到少进行冒泡排序后的数据:\n");
    printf("序号\t    国家\t 进口额\t\t出口额\t\t进出口交换比率\n");
    for (i = 0; i < N; i++)
        printf("[%2d]%12s\t%-9d\t%-9d\t%f\n", i+1, country[i], import[i], exports[i], IERatio[i]);
    
    // 打印进口额冒泡排序后的棒图
    int maxImport = import[0];
    for (i = 1; i < N; i++)
	{
        if (import[i] > maxImport)
            maxImport = import[i];
    }
    
    printf("\n进口额从多到少进行冒泡排序后的棒图:\n");
    for (i = 0; i < N; i++)
	{
        int numStars = (int)(import[i] * 40.0 / maxImport + 0.5);
        printf("[%2d]%12s\t ",i+1, country[i]);
        for (j = 0; j < numStars; j++)
            printf("*");
        printf("\n");
    }
	
	
	// 等待键盘输入回车键继续 
    printf("\n\n等待键盘输入回车键进行选择排序\n\n");
    while (getchar() != '\n');
	
	
	// 选择排序
    for (i = 0; i < N-1; i++)
	{
        int maxIndex = i;
        for (j = i+1; j < N; j++)
		{
            if (exports[j] > exports[maxIndex])
                maxIndex = j;
        }
        if (maxIndex != i)
		{
            temp = exports[i];
            exports[i] = exports[maxIndex];
            exports[maxIndex] = temp;

            temp = import[i];
            import[i] = import[maxIndex];
            import[maxIndex] = temp;

            strcpy(tempCountry, country[i]);
            strcpy(country[i], country[maxIndex]);
            strcpy(country[maxIndex], tempCountry);

            tempRatio = IERatio[i];
            IERatio[i] = IERatio[maxIndex];
            IERatio[maxIndex] = tempRatio;
        }
    }

    // 打印选择排序后的结果
    printf("按出口额从多到少进行选择排序后的数据:\n");
    printf("序号\t    国家\t 进口额\t\t出口额\t\t进出口交换比率\n");
    for (i = 0; i < N; i++)
        printf("[%2d]%12s\t%-9d\t%-9d\t%f\n", i+1, country[i], import[i], exports[i], IERatio[i]);

        // 打印出口额排序后的棒图
    int maxExport = exports[0];
    for (i = 1; i < N; i++) {
        if (exports[i] > maxExport) {
            maxExport = exports[i];
        }
    }

    printf("\n出口额从多到少进行选择排序后的棒图：\n");
    for (i = 0; i < N; i++) {
        int numStars = (int)(exports[i] * 40.0 / maxExport + 0.5);
        printf("[%2d]%12s\t ",i+1, country[i]);
        for (j = 0; j < numStars; j++) {
            printf("*");
        }
        printf("\n");
    }
    
    	
	// 等待键盘输入回车键继续 
    printf("\n\n等待键盘输入回车键进行插入排序\n\n");
    while (getchar() != '\n');
	
	
// 插入排序
    for (i = 1; i < N; i++)
	{
        float key = IERatio[i];
        int keyImport = import[i];
        int keyExport = exports[i];
        strcpy(tempCountry, country[i]);
        j = i - 1;
        while (j >= 0 && IERatio[j] < key)
		{
            IERatio[j+1] = IERatio[j];
            import[j+1] = import[j];
            exports[j+1] = exports[j];
            strcpy(country[j+1], country[j]);
            j = j - 1;
        }
        IERatio[j+1] = key;
        import[j+1] = keyImport;
        exports[j+1] = keyExport;
        strcpy(country[j+1], tempCountry);
    }
    
    // 打印插入排序后的结果
    printf("按进出口交换比率由大到小进行插入排序后的数据:\n");
    printf("序号\t    国家\t 进口额\t\t出口额\t\t进出口交换比率\n");
    for (i = 0; i < N; i++)
        printf("[%2d]%12s\t%-9d\t%-9d\t%f\n", i+1, country[i], import[i], exports[i], IERatio[i]);
	
	// 打印进出口交换比率排序后的棒图
    float maxRatio = IERatio[0];
    for (i = 1; i < N; i++)
	{
        if (IERatio[i] > maxRatio)
            maxRatio = IERatio[i];
    }

    printf("\n进出口交换比率由大到小进行插入排序后的棒图：\n");
    for (i = 0; i < N; i++)
	{
        int numStars = (int)(IERatio[i] * 40.0 / maxRatio + 0.5);
        printf("[%2d]%12s\t ",i+1, country[i]);
        for (j = 0; j < numStars; j++)
            printf("*");
        printf("\n");
    }
    
    
    	// 等待键盘输入回车键结束 
    printf("\n\n等待键盘输入回车键结束程序\n\n");
    while (getchar() != '\n');
	
	
	return 0;
}
