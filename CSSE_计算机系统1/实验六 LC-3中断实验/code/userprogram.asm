.ORIG    x3000
LD   R6, stack       ; ��ջָ���ʼֵ���ص�R6�Ĵ���
LD   R1, interrupt      ; ���жϷ������̵ĵ�ַ���ص�R1�Ĵ���
LD   R2, xianjin           ; ���ж���������ڵ�ַ���ص�R2�Ĵ���
STR  R1, R2, #0         ; ���жϷ������̵�ַ�洢���ж�������
LD   R3, ie             ; ���ж�ʹ�ܱ�־���ص�R3�Ĵ���
STI  R3, KBSR           ; ���ж�ʹ�ܱ�־�洢������״̬�Ĵ���(KBSR)�����ü����ж�
						
display   LEA R0, str1    ; ���ַ���str1�ĵ�ַ���ص�R0�Ĵ���
        TRAP x22        ; ʹ��TRAPָ������ַ���str1
        LEA R0, str2    ; ���ַ���str2�ĵ�ַ���ص�R0�Ĵ���
        JSR DELAY       ; ����DELAY�ӳ��򣬽����ӳ�
        TRAP x22        ; ʹ��TRAPָ������ַ���str2
        JSR DELAY       ; �ٴε���DELAY�ӳ��򣬽����ӳ�
        BRnzp display     ; ����ѭ������������ַ���
        HALT            ; ֹͣ���򣨲�Ӧִ�е����

DELAY   ST  R1, SaveR1  ; ����R1�Ĵ�����ֵ��SaveR1
        LD  R1, count   ; ���ӳټ�����ֵ���ص�R1�Ĵ�����25000��
loop     ADD R1, R1, #-1 ; �ݼ�R1
        BRp loop         ; ���R1 > 0�������ѭ��
        LD  R1, SaveR1  ; �ָ�R1�Ĵ�����ֵ
        RET             ; ����������


str1    .STRINGZ "ICS     ICS     ICS     ICS     ICS     ICS\n"
str2    .STRINGZ "    ICS     ICS     ICS     ICS     ICS\n"
ie      .FILL x4000      ; �ж�ʹ�ܱ�־��0100 0000 0000 0000��
KBSR    .FILL xFE00      
KBDR    .FILL xFE02      
count   .FILL #25000     
xianjin    .FILL x0180      ; 
interrupt .FILL x2000    ;
SaveR1  .BLKW 1          ; ���ڱ���R1��ֵ
stack .FILL x3000     ; ջָ���ʼλ��
DSR     .FILL xFE04    
DDR     .FILL xFE06    
.END

