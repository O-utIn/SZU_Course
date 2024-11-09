#include<stdio.h>
#include <string.h>
#define N 20

int main()
{
	char country[N][12] = {"����","�ձ�","����","�¹�","Խ��","�Ĵ�����","��������","����","����˹","̩��","ӡ��","ӡ��������","����","Ӣ��","�¼���","ɳ�ذ�����","ī����","����","���ɱ�","���ô�"};
	int import[N] = {17970081,20550277,21344326,11991430,9231942,16352908,9830508,11000670,7960907,6183263,2814551,6392283,1400494,2567686,3881973,5696861,1913928,3910500,2476131,3048978};
	int exports[N] = {57607523,16581426,14880548,11517540,13789507,6638009,7865488,5361123,6755117,6935458,9750751,6064749,10243171,8702212,5510346,3032124,6743974,4593066,5151026,5151026};
	float IERatio[N];
	
	int i,j,temp;
	char tempCountry[12];
    float tempRatio;
    
	for(i = 0; i < N; i++)
    	IERatio[i] = (float)import[i] / exports[i];

//	// ��ӡ��ÿ�����ҵĽ����ڽ�������
//	for (i = 0; i < N; i++)
//    	printf("%2d %12s �Ľ����ڽ�������Ϊ: %f\n",i+1, country[i], IERatio[i]);

	
    	
    	
    // ð������
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
    
    // ��ӡð�������Ľ��
    printf("�����ڶ�Ӷൽ�ٽ���ð������������:\n");
    printf("���\t    ����\t ���ڶ�\t\t���ڶ�\t\t�����ڽ�������\n");
    for (i = 0; i < N; i++)
        printf("[%2d]%12s\t%-9d\t%-9d\t%f\n", i+1, country[i], import[i], exports[i], IERatio[i]);
    
    // ��ӡ���ڶ�ð�������İ�ͼ
    int maxImport = import[0];
    for (i = 1; i < N; i++)
	{
        if (import[i] > maxImport)
            maxImport = import[i];
    }
    
    printf("\n���ڶ�Ӷൽ�ٽ���ð�������İ�ͼ:\n");
    for (i = 0; i < N; i++)
	{
        int numStars = (int)(import[i] * 40.0 / maxImport + 0.5);
        printf("[%2d]%12s\t ",i+1, country[i]);
        for (j = 0; j < numStars; j++)
            printf("*");
        printf("\n");
    }
	
	
	// �ȴ���������س������� 
    printf("\n\n�ȴ���������س�������ѡ������\n\n");
    while (getchar() != '\n');
	
	
	// ѡ������
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

    // ��ӡѡ�������Ľ��
    printf("�����ڶ�Ӷൽ�ٽ���ѡ������������:\n");
    printf("���\t    ����\t ���ڶ�\t\t���ڶ�\t\t�����ڽ�������\n");
    for (i = 0; i < N; i++)
        printf("[%2d]%12s\t%-9d\t%-9d\t%f\n", i+1, country[i], import[i], exports[i], IERatio[i]);

        // ��ӡ���ڶ������İ�ͼ
    int maxExport = exports[0];
    for (i = 1; i < N; i++) {
        if (exports[i] > maxExport) {
            maxExport = exports[i];
        }
    }

    printf("\n���ڶ�Ӷൽ�ٽ���ѡ�������İ�ͼ��\n");
    for (i = 0; i < N; i++) {
        int numStars = (int)(exports[i] * 40.0 / maxExport + 0.5);
        printf("[%2d]%12s\t ",i+1, country[i]);
        for (j = 0; j < numStars; j++) {
            printf("*");
        }
        printf("\n");
    }
    
    	
	// �ȴ���������س������� 
    printf("\n\n�ȴ���������س������в�������\n\n");
    while (getchar() != '\n');
	
	
// ��������
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
    
    // ��ӡ���������Ľ��
    printf("�������ڽ��������ɴ�С���в�������������:\n");
    printf("���\t    ����\t ���ڶ�\t\t���ڶ�\t\t�����ڽ�������\n");
    for (i = 0; i < N; i++)
        printf("[%2d]%12s\t%-9d\t%-9d\t%f\n", i+1, country[i], import[i], exports[i], IERatio[i]);
	
	// ��ӡ�����ڽ������������İ�ͼ
    float maxRatio = IERatio[0];
    for (i = 1; i < N; i++)
	{
        if (IERatio[i] > maxRatio)
            maxRatio = IERatio[i];
    }

    printf("\n�����ڽ��������ɴ�С���в��������İ�ͼ��\n");
    for (i = 0; i < N; i++)
	{
        int numStars = (int)(IERatio[i] * 40.0 / maxRatio + 0.5);
        printf("[%2d]%12s\t ",i+1, country[i]);
        for (j = 0; j < numStars; j++)
            printf("*");
        printf("\n");
    }
    
    
    	// �ȴ���������س������� 
    printf("\n\n�ȴ���������س�����������\n\n");
    while (getchar() != '\n');
	
	
	return 0;
}
