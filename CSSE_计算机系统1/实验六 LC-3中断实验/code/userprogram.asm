.ORIG    x3000
LD   R6, stack       ; 将栈指针初始值加载到R6寄存器
LD   R1, interrupt      ; 将中断服务例程的地址加载到R1寄存器
LD   R2, xianjin           ; 将中断向量表入口地址加载到R2寄存器
STR  R1, R2, #0         ; 将中断服务例程地址存储到中断向量表
LD   R3, ie             ; 将中断使能标志加载到R3寄存器
STI  R3, KBSR           ; 将中断使能标志存储到键盘状态寄存器(KBSR)，启用键盘中断
						
display   LEA R0, str1    ; 将字符串str1的地址加载到R0寄存器
        TRAP x22        ; 使用TRAP指令输出字符串str1
        LEA R0, str2    ; 将字符串str2的地址加载到R0寄存器
        JSR DELAY       ; 调用DELAY子程序，进行延迟
        TRAP x22        ; 使用TRAP指令输出字符串str2
        JSR DELAY       ; 再次调用DELAY子程序，进行延迟
        BRnzp display     ; 无限循环，继续输出字符串
        HALT            ; 停止程序（不应执行到这里）

DELAY   ST  R1, SaveR1  ; 保存R1寄存器的值到SaveR1
        LD  R1, count   ; 将延迟计数器值加载到R1寄存器（25000）
loop     ADD R1, R1, #-1 ; 递减R1
        BRp loop         ; 如果R1 > 0，则继续循环
        LD  R1, SaveR1  ; 恢复R1寄存器的值
        RET             ; 返回主程序


str1    .STRINGZ "ICS     ICS     ICS     ICS     ICS     ICS\n"
str2    .STRINGZ "    ICS     ICS     ICS     ICS     ICS\n"
ie      .FILL x4000      ; 中断使能标志（0100 0000 0000 0000）
KBSR    .FILL xFE00      
KBDR    .FILL xFE02      
count   .FILL #25000     
xianjin    .FILL x0180      ; 
interrupt .FILL x2000    ;
SaveR1  .BLKW 1          ; 用于保存R1的值
stack .FILL x3000     ; 栈指针初始位置
DSR     .FILL xFE04    
DDR     .FILL xFE06    
.END

