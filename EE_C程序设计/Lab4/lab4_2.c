#include <stdio.h>

int main()
{
	int basic, extra;
	printf("�������ۺϻ����ɼ��͸�����ɼ�(��Ϊ����)���ո�ָ����ݣ��س�ȷ��:");
	scanf("%d%d",&basic, &extra);
	if(basic > 100 || basic < 0 || extra > 30 || extra <0)
	{
		printf("�ɼ����Ϸ�");
		return 0;
	}
	else
		printf("�ۺϻ����ɼ�Ϊ:%d\n������ɼ�Ϊ:%d\n",basic,extra);
		if(basic < 60)
			printf("�ȼ�ΪF,��Ӧ����Ϊ0.0");
		else if(60 <= basic &&basic <= 64)
			printf("�ȼ�ΪD,��Ӧ����Ϊ1.0");
		else if(65 <= basic && basic <= 69)
			printf("�ȼ�ΪC,��Ӧ����Ϊ2.0");
		else if(70 <= basic && basic <= 74)
			printf("�ȼ�ΪC+,��Ӧ����Ϊ2.5");
		else if(75 <= basic && basic <= 79)
			printf("�ȼ�ΪB,��Ӧ����Ϊ3.0");
		else if(80 <= basic && basic <= 84) 
		{
			if(18 <= extra && extra <= 23)
				printf("�ȼ�ΪA,��Ӧ����Ϊ4.0");
			else if(24 <= extra && extra <=30)
				printf("�ȼ�ΪA+����Ӧ����Ϊ4.5");
			else
				printf("�ȼ�ΪB+,��Ӧ����Ϊ3.5");
		}
		else if(85 <= basic && basic <=92)
		{
			if(18 <= extra && extra <=30)
				printf("�ȼ�ΪA+����Ӧ����Ϊ4.5");
			else
				printf("�ȼ�ΪA����Ӧ����Ϊ4.0");
		}
		else
			printf("�ȼ�ΪA+����Ӧ����Ϊ4.5");

	return 0;
}
