.ORIG x3000
LD R6, ASCII
LD R5, NEGASCII
TRAP x23 ;输入
ADD R1,R0,x0 ;将第一个整数传给 R0
ADD R1,R1,R5 ;将第一个 ASCII 数字转换成数值
TRAP x23 ;另一个驶入
ADD R0,R0,R5 ;将另一个 ASCII 数字转换成数值
ADD R2,R0,R1 ;将这两个整数相加
ADD R2,R2,R6 ;将和转换成 ASCII 表示
LEA R0,MESG ;载入字符串的地址
TRAP x22 ;输出字符串
ADD R0,R2,x0 ;结果传给 RO
TRAP x21 ;显示结果
HALT
ASCII .FILL x30 ;mask：转换成 ASCII
NEGASCII .FILL xFFD0 ;mask：-x30
MESG .STRINGZ "The sum of those two numbers is "
.END 








