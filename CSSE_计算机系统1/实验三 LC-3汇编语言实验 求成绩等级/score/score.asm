       .orig x3000;
       LD    R0,score;
       LD    R4,num;
       ADD   R0,R0,-1;
;外层循环loop1
loop1  ADD   R0,R0,1;R0作为排序的第一个指针
       ADD   R1,R0,1;R1永远从R0的下一个开始
       AND   R5,R5,0;R5作为记录每个R0对应的已经进行了循环的次数
       ADD   R4,R4,-1;R4作为相应R0的所需循环的次数
       BRz   copy;当R4为0时说明内外层循环都已结束，此时就跳转到copy指令进行成绩的保存
;内层循环loop2
loop2  LDR   R2,R0,0;
       LDR   R3,R1,0;得到R0,R1所在地址的值
       ADD   R5,R5,1;循环次数加1
       NOT   R6,R3;
       ADD   R6,R6,1;R6=-R3
       ADD   R7,R2,R6;
       BRzp  flag;进行对R0与R1的地址的值的大小判断，若R0比R1小则进行如下交换操作，否则就跳转
       STR   R3,R0,0;
       STR   R2,R1,0;将R0,R1的地址的值进行交换
;进行跳转循环的判断
flag   ADD   R6,R4,0;
       NOT   R7,R5;
       ADD   R7,R7,1;
       ADD   R6,R6,R7;进行R4与R5的大小判断，若R4等于R5，说明内层循环结束，跳转到外层循环；否则继续
       BRz   loop1;
       ADD   R1,R1,1;继续则对R1加1，进行下一个地址，接着跳转到内层循环
       BRnzp loop2;
;进行将成绩从x3200复制到x4000的操作
copy   LD    R0,score;
       LD    R4,num;
       LD    R7,new;
copy1  LDR   R1,R0,0;
       STR   R1,R7,0;
       ADD   R0,R0,1;
       ADD   R7,R7,1;
       ADD   R4,R4,-1;
       BRp   copy1;
;进行成绩等级计算
       LD    R0,new;
       LD    R1,score1;
       LD    R2,score2;
       AND   R3,R3,0;用于计数的
       AND   R4,R4,0;用于记录A等级的
       AND   R5,R5,0;记录B等级的
;进行判断A
tmpA   LDR   R6,R0,0;
       ADD   R6,R6,R1;判断地址上的分数与85的大小关系
       BRn   tmpB;若小于85，则进行判断B等级的操作
       ADD   R4,R4,1;A等级人数+1
       ADD   R0,R0,1;地址移向下一位
       ADD   R3,R3,1;计数+1
       ADD   R7,R3,-4;判断是否已经到达前25%
       BRn   tmpA;
;进行判断B
tmpB   LDR   R6,R0,0;
       ADD   R6,R6,R2;判断分数与75的大小关系
       BRn   baocun;若小于75，则跳出计数，去记录A,B,C等级人数
       ADD   R5,R5,1;B等级人数+1
       ADD   R0,R0,1;地址移向下一位
       ADD   R3,R3,1;计数+1
       ADD   R7,R3,-8;判断是否到达前50%
       BRn   tmpB;
;进行记录人数
baocun LD    R7,dizhiA;记录A的地址
       LD    R6,dizhiB;记录B的地址
       STR   R4,R7,0;存放A的人数至地址x4100
       STR   R5,R6,0;存放B的人数至地址x4101
;代码区结束
       TRAP  x25;
;存放数据区
score    .FILL  x3200;
new      .FILL  x4000;
num      .FILL  16;
score1   .FILL  -85;
score2   .FILL  -75;
dizhiA   .FILL  x4100;
dizhiB   .FILL  x4101;
;
       .end;