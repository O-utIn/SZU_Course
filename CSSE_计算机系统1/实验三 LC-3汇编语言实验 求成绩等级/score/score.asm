       .orig x3000;
       LD    R0,score;
       LD    R4,num;
       ADD   R0,R0,-1;
;���ѭ��loop1
loop1  ADD   R0,R0,1;R0��Ϊ����ĵ�һ��ָ��
       ADD   R1,R0,1;R1��Զ��R0����һ����ʼ
       AND   R5,R5,0;R5��Ϊ��¼ÿ��R0��Ӧ���Ѿ�������ѭ���Ĵ���
       ADD   R4,R4,-1;R4��Ϊ��ӦR0������ѭ���Ĵ���
       BRz   copy;��R4Ϊ0ʱ˵�������ѭ�����ѽ�������ʱ����ת��copyָ����гɼ��ı���
;�ڲ�ѭ��loop2
loop2  LDR   R2,R0,0;
       LDR   R3,R1,0;�õ�R0,R1���ڵ�ַ��ֵ
       ADD   R5,R5,1;ѭ��������1
       NOT   R6,R3;
       ADD   R6,R6,1;R6=-R3
       ADD   R7,R2,R6;
       BRzp  flag;���ж�R0��R1�ĵ�ַ��ֵ�Ĵ�С�жϣ���R0��R1С��������½����������������ת
       STR   R3,R0,0;
       STR   R2,R1,0;��R0,R1�ĵ�ַ��ֵ���н���
;������תѭ�����ж�
flag   ADD   R6,R4,0;
       NOT   R7,R5;
       ADD   R7,R7,1;
       ADD   R6,R6,R7;����R4��R5�Ĵ�С�жϣ���R4����R5��˵���ڲ�ѭ����������ת�����ѭ�����������
       BRz   loop1;
       ADD   R1,R1,1;�������R1��1��������һ����ַ��������ת���ڲ�ѭ��
       BRnzp loop2;
;���н��ɼ���x3200���Ƶ�x4000�Ĳ���
copy   LD    R0,score;
       LD    R4,num;
       LD    R7,new;
copy1  LDR   R1,R0,0;
       STR   R1,R7,0;
       ADD   R0,R0,1;
       ADD   R7,R7,1;
       ADD   R4,R4,-1;
       BRp   copy1;
;���гɼ��ȼ�����
       LD    R0,new;
       LD    R1,score1;
       LD    R2,score2;
       AND   R3,R3,0;���ڼ�����
       AND   R4,R4,0;���ڼ�¼A�ȼ���
       AND   R5,R5,0;��¼B�ȼ���
;�����ж�A
tmpA   LDR   R6,R0,0;
       ADD   R6,R6,R1;�жϵ�ַ�ϵķ�����85�Ĵ�С��ϵ
       BRn   tmpB;��С��85��������ж�B�ȼ��Ĳ���
       ADD   R4,R4,1;A�ȼ�����+1
       ADD   R0,R0,1;��ַ������һλ
       ADD   R3,R3,1;����+1
       ADD   R7,R3,-4;�ж��Ƿ��Ѿ�����ǰ25%
       BRn   tmpA;
;�����ж�B
tmpB   LDR   R6,R0,0;
       ADD   R6,R6,R2;�жϷ�����75�Ĵ�С��ϵ
       BRn   baocun;��С��75��������������ȥ��¼A,B,C�ȼ�����
       ADD   R5,R5,1;B�ȼ�����+1
       ADD   R0,R0,1;��ַ������һλ
       ADD   R3,R3,1;����+1
       ADD   R7,R3,-8;�ж��Ƿ񵽴�ǰ50%
       BRn   tmpB;
;���м�¼����
baocun LD    R7,dizhiA;��¼A�ĵ�ַ
       LD    R6,dizhiB;��¼B�ĵ�ַ
       STR   R4,R7,0;���A����������ַx4100
       STR   R5,R6,0;���B����������ַx4101
;����������
       TRAP  x25;
;���������
score    .FILL  x3200;
new      .FILL  x4000;
num      .FILL  16;
score1   .FILL  -85;
score2   .FILL  -75;
dizhiA   .FILL  x4100;
dizhiB   .FILL  x4101;
;
       .end;