#include<stdio.h>
#include<math.h>

int main()
{
	int i,row,col;
    printf("����Ϊ0~99֮���ƽ������:\n");
    printf("        ");
    for(i = 0; i < 10;i++)
        printf("%-11d",i);
    printf("\n");

    for(row = 0;row < 10;row++) 
	{
        printf("%-6d",row);
        for(col = 0;col < 10;col++) 
		{
            int num = row * 10 + col;
            double squareRoot = sqrt(num);
            printf("%-11.3f",squareRoot);
        }
        printf("\n");
    }
	return 0;
}
