.ORIG     x2000 ; 程序入口地址为 x2000

ADD R6,R6,#-1       ; 将栈指针减一，为下面的存储操作腾出空间
STR R0,R6,#0       ; 将寄存器 R0 的值存储到栈中
ADD R6,R6,#-1       ; 将栈指针减一
STR R1,R6,#0       ; 将寄存器 R1 的值存储到栈中
ADD R6,R6,#-1       ; 将栈指针减一
STR R2,R6,#0       ; 将寄存器 R2 的值存储到栈中
ADD R6,R6,#-1       ; 将栈指针减一
STR R3,R6,#0       ; 将寄存器 R3 的值存储到栈中

LOOP    ST  R0,SaveR0      ; 保存寄存器 R0 的值到 SaveR0 内存单元

CHECK   LDI R1,KBSR       ; 将 KBSR 寄存器的值加载到 R1 中，用于检查 KBSR[15]
        ADD R1,R1,#0      ; 将 R1 自身加上零，无操作
        BRzp    CHECK     ; 如果 R1 大于等于零，则继续循环检查 KBSR[15]，否则跳出循环

        LDI R0,KBDR       ; 将 KBDR 寄存器的值加载到 R0 中，读取键盘数据
        LD  R2,_ENDLINE   ; 将 _ENDLINE 的地址加载到 R2 中
        ADD R2,R2,R0      ; 将 R2 和 R0 相加得到换行字符的 ASCII 码
        BRnp    LOOP      ; 如果结果非负，则继续循环，否则跳出循环

        AND R3,R3,#0      ; 将 R3 清零
        ADD R3,R3,#10     ; 将 R3 加上 10

P_LOOP  LD  R0,SaveR0      ; 加载 SaveR0 内存单元中的值到 R0 中

START   LDI R1,DSR        ; 将 DSR 寄存器的值加载到 R1 中，用于检查 DSR[15]
        BRzp    START     ; 如果 R1 大于等于零，则继续循环等待，否则跳出循环

        STI R0,DDR        ; 将 R0 的值存储到 DDR 寄存器中，将数据发送给设备
        ADD R3,R3,#-1     ; 将 R3 减一
        BRp P_LOOP        ; 如果结果为正，则继续循环

P_ENDL  LD  R0,ENDLINE     ; 将 ENDLINE 的值加载到 R0 中
        LDI R1,DSR        ; 将 DSR 寄存器的值加载到 R1 中，用于检查 DSR[15]
        BRzp    P_ENDL    ; 如果 R1 大于等于零，则继续循环等待，否则跳出循环
        STI R0,DDR        ; 将 R0 的值存储到 DDR 寄存器中，发送换行字符

        LDR R3,R6,#0      ; 将栈中保存的值加载到 R3 中
        ADD R6,R6,#1      ; 将栈指针加一
        LDR R2,R6,#0      ; 将栈中保存的值加载到 R2 中
        ADD R6,R6,#1      ; 将栈指针加一
        LDR R1,R6,#0      ; 将栈中保存的值加载到 R1 中
        ADD R6,R6,#1      ; 将栈指针加一
        LDR R0,R6,#0      ; 将栈中保存的值加载到 R0 中
        ADD R6,R6,#1      ; 将栈指针加一
        RTI               ;

                          
_ENDLINE .FILL   xFFF6  ; _'\n'
ENDLINE .FILL   x000A   ; 换行的ASCII码
SaveR0  .FILL   #0	
KBSR    .FILL   xFE00
KBDR    .FILL   xFE02
DSR     .FILL   xFE04
DDR     .FILL   xFE06
.END
