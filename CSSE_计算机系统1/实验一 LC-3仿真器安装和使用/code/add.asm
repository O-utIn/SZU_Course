.ORIG x3000
LD R6, ASCII
LD R5, NEGASCII
TRAP x23 ;����
ADD R1,R0,x0 ;����һ���������� R0
ADD R1,R1,R5 ;����һ�� ASCII ����ת������ֵ
TRAP x23 ;��һ��ʻ��
ADD R0,R0,R5 ;����һ�� ASCII ����ת������ֵ
ADD R2,R0,R1 ;���������������
ADD R2,R2,R6 ;����ת���� ASCII ��ʾ
LEA R0,MESG ;�����ַ����ĵ�ַ
TRAP x22 ;����ַ���
ADD R0,R2,x0 ;������� RO
TRAP x21 ;��ʾ���
HALT
ASCII .FILL x30 ;mask��ת���� ASCII
NEGASCII .FILL xFFD0 ;mask��-x30
MESG .STRINGZ "The sum of those two numbers is "
.END 








