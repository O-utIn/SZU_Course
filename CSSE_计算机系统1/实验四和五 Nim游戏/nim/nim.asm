	.ORIG	X3000
	LD	R1,Anum	;R1��A��������ʼ3
	LD	R2,Bnum	;R2��B��������ʼ5
	LD	R3,Cnum	;R3��C��������ʼ8	
;
;��������Ϸ����	
player1	JSR	DISPLAY		;����չʾ�����ӳ���
	AND	R5,R5,0		;R5��0��ʾΪ���1
begin1	LEA	R0,Prompt1	;��player1��
	PUTS			;�����ʾ���
	JSR	PLAY		;��ʼ��Ϸ
	LD	R0,NewLine	;/n
	OUT
	LD	R0,NewLine	;/n
	OUT
	ADD	R4,R1,R2
	ADD	R4,R4,R3
	BRnp	player2		;������˵����Ϸ����
	LEA	R0,Win1		;������Ϊ0���1ʤ��
	PUTS
	BRnzp	exit		;��Ϸ����
	
player2	JSR	DISPLAY		;����չʾ�����ӳ���
	ADD	R5,R5,1		;���2
begin2	LEA	R0,Prompt2	;��player2��
	PUTS			;�����ʾ���
	JSR	PLAY		;��ʼ��Ϸ
	LD	R0,NewLine	;/n
	OUT
	LD	R0,NewLine	;/n
	OUT
	ADD	R4,R1,R2
	ADD	R4,R4,R3
	BRnp	player1		;������˵����Ϸ����
	LEA	R0,Win2		;������Ϊ0���2ʤ��
	PUTS
 
exit	HALT
;
;main������
Anum	.FILL		3
Bnum	.FILL		5
Cnum	.FILL		8
NewLine	.FILL		10		;/n
Prompt1	.STRINGZ	"Player 1, choose a row and number of rocks: "	;���1��ʾ���
Prompt2	.STRINGZ	"Player 2, choose a row and number of rocks: "	;���2��ʾ���
Win1	.STRINGZ	"Player 1 Wins.";���1ʤ�����
Win2	.STRINGZ	"Player 2 Wins.";���2ʤ�����
 
 
;չʾ�����ӳ���
DISPLAY	ST	R7,DISPLAYR7
 
	LEA	R0,PromptA	;��A��
	PUTS			;���A��ʾ���
	ADD	R4,R1,0	;��A����
	JSR	PUTBALL		;����չʾ�����ӳ���
 
	LEA	R0,PromptB	;��B��
	PUTS			;���B��ʾ���
	ADD	R4,R2,0	;��B����
	JSR	PUTBALL		;����չʾ�����ӳ���
 
	LEA	R0,PromptC	;��C��
	PUTS			;���C��ʾ���
	ADD	R4,R3,0	;��C����
	JSR	PUTBALL		;����չʾ�����ӳ���
 
	LD	R7,DISPLAYR7
	RET
;
;DISPLAY������
DISPLAYR7 .FILL		0
PromptA	.STRINGZ	"ROW A: "	;A��ʾ���
PromptB	.STRINGZ	"ROW B: "	;B��ʾ���
PromptC	.STRINGZ	"ROW C: "	;C��ʾ���

 
 
;����չʾ�ӳ���
PUTBALL	ST	R7,PUTBALLR7	
 
	LD	R0,o		;'o'
 
output	ADD	R4,R4,-1	;����--
	BRn	finish		;��������������	
	OUT
	BRnzp	output
 
finish	LD	R0,NewLine	;/n
	OUT
 
	LD	R7,PUTBALLR7
	RET
;
;PUTBALL������
PUTBALLR7 .FILL		0
o	.FILL		111		;'o'	


 
;ÿ��ÿ����Ϸ�ӳ���
PLAY	ST	R5,SaveR5
	ST	R7,PLAYR7	
 
	GETC			;�������
	OUT			;����
	ADD	R6,R0,0		;����������R6
 
	GETC			;���ȡ����
	OUT			;����	
	LD	R5,Term		;-'0'=-48
	ADD	R5,R0,R5
	BRnz	warn		
 
	LD	R4,nA
	ADD	R4,R6,R4	;=?'A'
	BRnp	testb		;���Ǿ���ת��B
	NOT	R4,R5
	ADD	R4,R4,1		;-ȡ����
	ADD	R1,R1,R4	;ȡ��
	BRzp	return		;ȡ����Ч����
	ADD	R1,R1,R5	;ȡ����Ч�Ļ�
	BRnzp	warn		;��������
 
testb	LD	R4,nB
	ADD	R4,R6,R4	;=?'B'
	BRnp	testc		;���Ǿ���ת��C
	NOT	R4,R5
	ADD	R4,R4,1		;-ȡ����
	ADD	R2,R2,R4	;ȡ��
	BRzp	return		;ȡ����Ч����
	ADD	R2,R2,R5	;ȡ����Ч�Ļ�
	BRnzp	warn		;��������
 
testc	LD	R4,nC
	ADD	R4,R6,R4	;=?'C'
	BRnp	warn		;���Ǿͱ�������������������
	NOT	R4,R5
	ADD	R4,R4,1		;-ȡ����
	ADD	R3,R3,R4	;ȡ��
	BRzp	return		;ȡ����Ч����
	ADD	R3,R3,R5	;ȡ����Ч�Ļ�
 
warn	LD	R0,NewLine	;/n
	OUT
	LEA	R0,Warning
	PUTS			;�������
	LD	R0,NewLine	;/n
	OUT
	LD	R5,SaveR5	;���������
	BRz	begin1		;Ϊ0�������1
	BRp	begin2		;Ϊ1�������2
	
return	LD	R5,SaveR5
	LD	R7,PLAYR7
	RET
;
;PLAY������
SaveR5	.FILL		0
PLAYR7 	.FILL		0
Term	.FILL		-48		;��ֵASCIIת��
nA	.FILL		-65		;-'A'
nB	.FILL		-66		;-'B'
nC	.FILL		-67		;-'C'
Warning	.STRINGZ	"Invalid move. Try again.";������Ч����

	.END