.data
CONTROL: .word 0x10000
DATA: .word 0x10008
NUM1: .word 0  # 乘数1
NUM2: .word 0  # 乘数2
STACK_SIZE: .space 20  # 栈大小
STACK_POINTER: .word 0  # 栈指针
INPUT_STRING: .asciiz "please enter two numbers:\n"
RESULT_STRING: .asciiz "result:\n"
WARNING_STRING: .asciiz "warning: result overflow\n"

# ----------------------------------------

.text
main:
	daddi $sp, $zero, STACK_SIZE  # 栈空间不超过 5 个 int 
	
	# 准备参数 DATA , CONTROL
	lw $a1, DATA($zero)  # a1 = M[0x10008]
	lw $a2, CONTROL($zero)  # a2 = M[0x10000]
	
	# 输出 INPUT_STRING
	daddi $a0, $zero, INPUT_STRING  # a0 = &INPUT_STRING
	jal writeString  # call writeString()
	
	# 输入 NUM1, NUM2
	daddi $a0, $zero, NUM1  # 读入 NUM1
	jal readInt  # call readInt()
	daddi $a0, $zero, NUM2  # 读入 NUM2
	jal readInt  # call readInt()
	
	# 求积
	# t0: i
	# t1: NUM1
	# t2: NUM2
	# t3: NUM1 的当前位
	# t4: ans
	daddi $t0, $zero, 32  # i = 32
	lw $t1, NUM1($zero)  # t1 = M[NUM1]
	lw $t2, NUM2($zero)  # t2 = M[NUM2]
loop:
	beq $t0, $zero, endLoop  # if (i == 0) goto endLoop;
	andi $t3, $t1, 1  # t3 = NUM1 的当前位
	beq $t3, $zero, notAdd  # if (t3 == 0) goto notAdd;
	dadd $t4, $t4, $t2  # ans += t2
notAdd:
	dsrl $t1, $t1, 1  # NUM1 >>= 1
	dsll $t2, $t2, 1  # NUM2 <<= 1
	daddi $t0, $t0, -1  # i--
	j loop
endLoop:
	# 输出 RESULT_STRING
	daddi $a0, $zero, RESULT_STRING  # a0 = &RESULT_STRING
	jal writeString  # call writeString()
	
	# 输出 ans
	daddi $a0, $t4, 0  # a0 = ans
	jal writeInt  # call writeInt()
	
	# 溢出检测
	dsrl $t4, $t4, 16  # ans >>= 16
	dsrl $t4, $t4, 16  # ans >>= 16
	beq $t4, $zero, halt  # if (ans == 0) goto halt;
	
	# 输出 WARNING_STRING
	daddi $a0, $zero, WARNING_STRING  # a0 = &WARNING_STRING
	jal writeString  # call writeString()
halt:
	halt
	
# ----------------------------------------

# writeString(a0, a1, a2)
# a0: 要输出的 string 的地址
# a1: DATA
# a2: CONTROL
writeString:
	daddi $sp, $sp, -4  # 分配 1 个 int 的栈帧
	sw $ra, ($sp)  # M[sp] = ra , 保存返回地址
	
	sw $a0, ($a1)  # M[DATA] = 要输出的 string 的地址
	daddi $t0, $zero, 4  # t0 = 4
	sw $t0, ($a2)  # M[CONTROL] = 4
	
	lw $ra, ($sp)  # ra = M[sp] , 恢复返回地址
	daddi $sp, $sp, 4  # 回收栈帧
	jr $ra  # return

# ----------------------------------------

# readInt(a0, a1, a2)
# a0: 要输入的 int 的地址
# a1: DATA
# a2: CONTROL
readInt:
	daddi $sp, $sp, -4  # 分配 1 个 int 的栈帧
	sw $ra, ($sp)  # M[sp] = ra , 保存返回地址
	
	# 输入一个 int 到 M[DATA]
	daddi $t0, $zero, 8  # t0 = 8
	sw $t0, ($a2)  # M[CONTROL] = 8
	
	# 将读入的 int 存到 M[a0]
	lw $t1, ($a1)  # t1 = M[DATA]
	sw $t1, ($a0)  # M[a0] = DATA
	
	lw $ra, ($sp)  # ra = M[sp] , 恢复返回地址
	daddi $sp, $sp, 4  # 回收栈帧
	jr $ra  # return

# ----------------------------------------

# writeInt(a0, a1, a2)
# a0: 要输出的 int 的地址
# a1: DATA
# a2: CONTROL
writeInt:
	daddi $sp, $sp, -4  # 分配 1 个 int 的栈帧
	sw $ra, ($sp)  # M[sp] = ra , 保存返回地址
	
	sw $a0, ($a1)  # M[DATA] = 要输出的 int 的地址
	daddi $t0, $zero, 2  # t0 = 2
	sw $t0, ($a2)  # M[CONTROL] = 2
	
	lw $ra, ($sp)  # ra = M[sp] , 恢复返回地址
	daddi $sp, $sp, 4  # 回收栈帧
	jr $ra  # return
	