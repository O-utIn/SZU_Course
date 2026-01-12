.data
a: .word 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4
b: .word 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1
c: .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
len: .word 4
control: .word32 0x10000
data: .word32 0x10008

.text
start:
	ld r16, len(r0)  # 发生数据冒险, 提前该指令, 防止堵塞
	daddi r17, r0, 0
	daddi r21, r0, a
	daddi r22, r0, b
	daddi r23, r0, c
loop1:
	slt r8, r17, r16
	daddi r19, r0, 0
	beq r8, r0, exit1  # 滞后发生数据冒险的指令, 开启前推后可防止堵塞
loop2:
	slt r8, r19, r16
	daddi r19, r19, 1  # 提前该指令, 防止堵塞
	beq r8, r0, exit2  # 发生数据相关
     
	dsll r8, r17, 2
	dadd r8, r8, r19
	dsll r8, r8, 3
	
	dadd r9, r8, r21
	dadd r10, r8, r22
	
	ld r9, 0(r9)
	ld r10, 0(r10)
	
	dadd r11, r8, r23  # 滞后不冲突的指令, 防止阻塞
	dadd r12, r9, r10  # 发生数据冒险
	
	sd r12, 0(r11)

	j loop2
exit2:
	daddi r17, r17, 1
	j loop1
exit1: 
	halt
