.ORIG     x2000 ; ������ڵ�ַΪ x2000

ADD R6,R6,#-1       ; ��ջָ���һ��Ϊ����Ĵ洢�����ڳ��ռ�
STR R0,R6,#0       ; ���Ĵ��� R0 ��ֵ�洢��ջ��
ADD R6,R6,#-1       ; ��ջָ���һ
STR R1,R6,#0       ; ���Ĵ��� R1 ��ֵ�洢��ջ��
ADD R6,R6,#-1       ; ��ջָ���һ
STR R2,R6,#0       ; ���Ĵ��� R2 ��ֵ�洢��ջ��
ADD R6,R6,#-1       ; ��ջָ���һ
STR R3,R6,#0       ; ���Ĵ��� R3 ��ֵ�洢��ջ��

LOOP    ST  R0,SaveR0      ; ����Ĵ��� R0 ��ֵ�� SaveR0 �ڴ浥Ԫ

CHECK   LDI R1,KBSR       ; �� KBSR �Ĵ�����ֵ���ص� R1 �У����ڼ�� KBSR[15]
        ADD R1,R1,#0      ; �� R1 ��������㣬�޲���
        BRzp    CHECK     ; ��� R1 ���ڵ����㣬�����ѭ����� KBSR[15]����������ѭ��

        LDI R0,KBDR       ; �� KBDR �Ĵ�����ֵ���ص� R0 �У���ȡ��������
        LD  R2,_ENDLINE   ; �� _ENDLINE �ĵ�ַ���ص� R2 ��
        ADD R2,R2,R0      ; �� R2 �� R0 ��ӵõ������ַ��� ASCII ��
        BRnp    LOOP      ; �������Ǹ��������ѭ������������ѭ��

        AND R3,R3,#0      ; �� R3 ����
        ADD R3,R3,#10     ; �� R3 ���� 10

P_LOOP  LD  R0,SaveR0      ; ���� SaveR0 �ڴ浥Ԫ�е�ֵ�� R0 ��

START   LDI R1,DSR        ; �� DSR �Ĵ�����ֵ���ص� R1 �У����ڼ�� DSR[15]
        BRzp    START     ; ��� R1 ���ڵ����㣬�����ѭ���ȴ�����������ѭ��

        STI R0,DDR        ; �� R0 ��ֵ�洢�� DDR �Ĵ����У������ݷ��͸��豸
        ADD R3,R3,#-1     ; �� R3 ��һ
        BRp P_LOOP        ; ������Ϊ���������ѭ��

P_ENDL  LD  R0,ENDLINE     ; �� ENDLINE ��ֵ���ص� R0 ��
        LDI R1,DSR        ; �� DSR �Ĵ�����ֵ���ص� R1 �У����ڼ�� DSR[15]
        BRzp    P_ENDL    ; ��� R1 ���ڵ����㣬�����ѭ���ȴ�����������ѭ��
        STI R0,DDR        ; �� R0 ��ֵ�洢�� DDR �Ĵ����У����ͻ����ַ�

        LDR R3,R6,#0      ; ��ջ�б����ֵ���ص� R3 ��
        ADD R6,R6,#1      ; ��ջָ���һ
        LDR R2,R6,#0      ; ��ջ�б����ֵ���ص� R2 ��
        ADD R6,R6,#1      ; ��ջָ���һ
        LDR R1,R6,#0      ; ��ջ�б����ֵ���ص� R1 ��
        ADD R6,R6,#1      ; ��ջָ���һ
        LDR R0,R6,#0      ; ��ջ�б����ֵ���ص� R0 ��
        ADD R6,R6,#1      ; ��ջָ���һ
        RTI               ;

                          
_ENDLINE .FILL   xFFF6  ; _'\n'
ENDLINE .FILL   x000A   ; ���е�ASCII��
SaveR0  .FILL   #0	
KBSR    .FILL   xFE00
KBDR    .FILL   xFE02
DSR     .FILL   xFE04
DDR     .FILL   xFE06
.END
