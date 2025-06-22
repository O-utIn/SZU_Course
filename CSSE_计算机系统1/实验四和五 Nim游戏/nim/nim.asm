	.ORIG	X3000
	LD	R1,Anum	;R1存A球数，初始3
	LD	R2,Bnum	;R2存B球数，初始5
	LD	R3,Cnum	;R3存C球数，初始8	
;
;主函数游戏部分	
player1	JSR	DISPLAY		;调用展示界面子程序
	AND	R5,R5,0		;R5清0表示为玩家1
begin1	LEA	R0,Prompt1	;传player1串
	PUTS			;输出提示语句
	JSR	PLAY		;开始游戏
	LD	R0,NewLine	;/n
	OUT
	LD	R0,NewLine	;/n
	OUT
	ADD	R4,R1,R2
	ADD	R4,R4,R3
	BRnp	player2		;球数不说明游戏继续
	LEA	R0,Win1		;总球数为0玩家1胜利
	PUTS
	BRnzp	exit		;游戏结束
	
player2	JSR	DISPLAY		;调用展示界面子程序
	ADD	R5,R5,1		;玩家2
begin2	LEA	R0,Prompt2	;传player2串
	PUTS			;输出提示语句
	JSR	PLAY		;开始游戏
	LD	R0,NewLine	;/n
	OUT
	LD	R0,NewLine	;/n
	OUT
	ADD	R4,R1,R2
	ADD	R4,R4,R3
	BRnp	player1		;球数不说明游戏继续
	LEA	R0,Win2		;总球数为0玩家2胜利
	PUTS
 
exit	HALT
;
;main数据区
Anum	.FILL		3
Bnum	.FILL		5
Cnum	.FILL		8
NewLine	.FILL		10		;/n
Prompt1	.STRINGZ	"Player 1, choose a row and number of rocks: "	;玩家1提示语句
Prompt2	.STRINGZ	"Player 2, choose a row and number of rocks: "	;玩家2提示语句
Win1	.STRINGZ	"Player 1 Wins.";玩家1胜利语句
Win2	.STRINGZ	"Player 2 Wins.";玩家2胜利语句
 
 
;展示界面子程序
DISPLAY	ST	R7,DISPLAYR7
 
	LEA	R0,PromptA	;传A串
	PUTS			;输出A提示语句
	ADD	R4,R1,0	;传A球数
	JSR	PUTBALL		;调用展示球数子程序
 
	LEA	R0,PromptB	;传B串
	PUTS			;输出B提示语句
	ADD	R4,R2,0	;传B球数
	JSR	PUTBALL		;调用展示球数子程序
 
	LEA	R0,PromptC	;传C串
	PUTS			;输出C提示语句
	ADD	R4,R3,0	;传C球数
	JSR	PUTBALL		;调用展示球数子程序
 
	LD	R7,DISPLAYR7
	RET
;
;DISPLAY数据区
DISPLAYR7 .FILL		0
PromptA	.STRINGZ	"ROW A: "	;A提示语句
PromptB	.STRINGZ	"ROW B: "	;B提示语句
PromptC	.STRINGZ	"ROW C: "	;C提示语句

 
 
;球数展示子程序
PUTBALL	ST	R7,PUTBALLR7	
 
	LD	R0,o		;'o'
 
output	ADD	R4,R4,-1	;计数--
	BRn	finish		;输出完毕跳到换行	
	OUT
	BRnzp	output
 
finish	LD	R0,NewLine	;/n
	OUT
 
	LD	R7,PUTBALLR7
	RET
;
;PUTBALL数据区
PUTBALLR7 .FILL		0
o	.FILL		111		;'o'	


 
;每人每轮游戏子程序
PLAY	ST	R5,SaveR5
	ST	R7,PLAYR7	
 
	GETC			;获得行名
	OUT			;回显
	ADD	R6,R0,0		;将行名存于R6
 
	GETC			;获得取球数
	OUT			;回显	
	LD	R5,Term		;-'0'=-48
	ADD	R5,R0,R5
	BRnz	warn		
 
	LD	R4,nA
	ADD	R4,R6,R4	;=?'A'
	BRnp	testb		;不是就跳转查B
	NOT	R4,R5
	ADD	R4,R4,1		;-取球数
	ADD	R1,R1,R4	;取球
	BRzp	return		;取球有效返回
	ADD	R1,R1,R5	;取球无效改回
	BRnzp	warn		;警告重来
 
testb	LD	R4,nB
	ADD	R4,R6,R4	;=?'B'
	BRnp	testc		;不是就跳转查C
	NOT	R4,R5
	ADD	R4,R4,1		;-取球数
	ADD	R2,R2,R4	;取球
	BRzp	return		;取球有效返回
	ADD	R2,R2,R5	;取球无效改回
	BRnzp	warn		;警告重来
 
testc	LD	R4,nC
	ADD	R4,R6,R4	;=?'C'
	BRnp	warn		;不是就表明输入有误跳至警告
	NOT	R4,R5
	ADD	R4,R4,1		;-取球数
	ADD	R3,R3,R4	;取球
	BRzp	return		;取球有效返回
	ADD	R3,R3,R5	;取球无效改回
 
warn	LD	R0,NewLine	;/n
	OUT
	LEA	R0,Warning
	PUTS			;输出警告
	LD	R0,NewLine	;/n
	OUT
	LD	R5,SaveR5	;获得玩家序号
	BRz	begin1		;为0跳回玩家1
	BRp	begin2		;为1跳回玩家2
	
return	LD	R5,SaveR5
	LD	R7,PLAYR7
	RET
;
;PLAY数据区
SaveR5	.FILL		0
PLAYR7 	.FILL		0
Term	.FILL		-48		;数值ASCII转换
nA	.FILL		-65		;-'A'
nB	.FILL		-66		;-'B'
nC	.FILL		-67		;-'C'
Warning	.STRINGZ	"Invalid move. Try again.";输入无效警告

	.END