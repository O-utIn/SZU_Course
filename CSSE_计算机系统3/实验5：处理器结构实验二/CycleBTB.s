.data
str:  .asciiz "the data of matrix 3:\n"
mx1:   .space 512
mx2:   .space 512
mx3:   .space 512

			.text
initial:     daddi r22,r0,mx1
             daddi r23,r0,mx2
			daddi r21,r0,mx3
input:		daddi r9,r0,64
			daddi r8,r0,0
loop1:		dsll r11,r8,3
			dadd r10,r11,r22
			dadd r11,r11,r23
			daddi r12,r0,2
			daddi r13,r0,3
			sd r12,0(r10)
			sd r13,0(r11)

			daddi r8,r8,1
			slt r10,r8,r9
			bne r10,r0,loop1

mul:			daddi r16,r0,8
			daddi r17,r0,0	#r17代表i
loop2:		daddi r18,r0,0	#r18代表j
loop3:		daddi r19,r0,0	#r19代表k
			daddi r20,r0,0	#r20存储叠加乘积的中间值
			
dsll r8,r17,6		#r8存储mx1[i][0]的位置，在整个loop3循环中保持不变
			dsll r9,r19,3
			dadd r9,r8,r9		#r9是保存mx1[i][k]相对于mx1[i][0]的偏移量，r9在loop3循环中会改变
			dadd r9,r9,r22	
			ld r9,0(r9)		#读取mx1[i][k]的值
			
dsll r10,r19,6	#r10存储mx2[k][0]值，在整个loop3循环中会发生改变
			dsll r11,r18,3	#r11存储mx2[k][j]相对于mx2[k][0]的值，在整个loop3种保持不变
			dadd r10,r10,r11
			dadd r10,r10,r23
			ld r10,0(r10)		#读取mx2[k][j]的值
			dmul r12,r9,r10	#mx1[i][k]与mx2[k][j]相乘
			dadd r20,r20,r12	#结果与r20累加
			daddi r19,r19,1	#r9的值加一
			
			dsll r9,r19,3
			dadd r9,r8,r9
			dadd r9,r9,r22
			ld r9,0(r8)		#重复上面取mx1[i][k]值的过程，变化的是r19的值加了一，意味着k+1
			dsll r10,r19,6	#r10的值由于r19的改变而发生了改变，即k变成k+1
			dadd r10,r10,r11
			dadd r10,r10,r23
			ld r10,0(r10)		#复上面取mx1[k][j]值的过程，变化的是r19的值加了一，意味着k+1
			dmul r12,r9,r10
			dadd r20,r20,r12
			daddi r19,r19,1	#r19+1

			dsll r9,r19,3
			dadd r9,r8,r9
			dadd r9,r9,r22
			ld r9,0(r8)		#重复上面取mx1[i][k]值的过程，变化的是r19的值加了一，意味着k+1
			dsll r10,r19,6	#r10的值由于r19的改变而发生了改变，即k变成k+1
			dadd r10,r10,r11
			dadd r10,r10,r23
			ld r10,0(r10)		#复上面取mx1[k][j]值的过程，变化的是r19的值加了一，意味着k+1
			dmul r12,r9,r10
			dadd r20,r20,r12
			daddi r19,r19,1	#r19+1


dsll r9,r19,3
			dadd r9,r8,r9
			dadd r9,r9,r22
			ld r9,0(r8)		#重复上面取mx1[i][k]值的过程，变化的是r19的值加了一，意味着k+1
			dsll r10,r19,6	#r10的值由于r19的改变而发生了改变，即k变成k+1
			dadd r10,r10,r11
			dadd r10,r10,r23
			ld r10,0(r10)		#复上面取mx1[k][j]值的过程，变化的是r19的值加了一，意味着k+1
			dmul r12,r9,r10
			dadd r20,r20,r12
			daddi r19,r19,1	#r19+1

dsll r9,r19,3
			dadd r9,r8,r9
			dadd r9,r9,r22
			ld r9,0(r8)		#重复上面取mx1[i][k]值的过程，变化的是r19的值加了一，意味着k+1
			dsll r10,r19,6	#r10的值由于r19的改变而发生了改变，即k变成k+1
			dadd r10,r10,r11
			dadd r10,r10,r23
			ld r10,0(r10)		#复上面取mx1[k][j]值的过程，变化的是r19的值加了一，意味着k+1
			dmul r12,r9,r10
			dadd r20,r20,r12
			daddi r19,r19,1	#r19+1


dsll r9,r19,3
			dadd r9,r8,r9
			dadd r9,r9,r22
			ld r9,0(r8)		#重复上面取mx1[i][k]值的过程，变化的是r19的值加了一，意味着k+1
			dsll r10,r19,6	#r10的值由于r19的改变而发生了改变，即k变成k+1
			dadd r10,r10,r11
			dadd r10,r10,r23
			ld r10,0(r10)		#复上面取mx1[k][j]值的过程，变化的是r19的值加了一，意味着k+1
			dmul r12,r9,r10
			dadd r20,r20,r12
			daddi r19,r19,1	#r19+1


dsll r9,r19,3
			dadd r9,r8,r9
			dadd r9,r9,r22
			ld r9,0(r8)		#重复上面取mx1[i][k]值的过程，变化的是r19的值加了一，意味着k+1
			dsll r10,r19,6	#r10的值由于r19的改变而发生了改变，即k变成k+1
			dadd r10,r10,r11
			dadd r10,r10,r23
			ld r10,0(r10)		#复上面取mx1[k][j]值的过程，变化的是r19的值加了一，意味着k+1
			dmul r12,r9,r10
			dadd r20,r20,r12
			daddi r19,r19,1	#r19+1


dsll r9,r19,3
			dadd r9,r8,r9
			dadd r9,r9,r22
			ld r9,0(r8)		#重复上面取mx1[i][k]值的过程，变化的是r19的值加了一，意味着k+1
			dsll r10,r19,6	#r10的值由于r19的改变而发生了改变，即k变成k+1
			dadd r10,r10,r11
			dadd r10,r10,r23
			ld r10,0(r10)		#复上面取mx1[k][j]值的过程，变化的是r19的值加了一，意味着k+1
			dmul r12,r9,r10
			dadd r20,r20,r12
			daddi r19,r19,1	#r19+1



			dsll r8,r17,6
			dsll r9,r18,3
			dadd r8,r8,r9
			dadd r8,r8,r21
			sd r20,0(r8)

			daddi r18,r18,1
			slt r8,r18,r16
			bne r8,r0,loop3

			daddi r17,r17,1
			slt r8,r17,r16
			bne r8,r0,loop2			

			halt
