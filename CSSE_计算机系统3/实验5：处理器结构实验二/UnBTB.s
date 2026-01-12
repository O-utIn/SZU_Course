.data
array:   .word 0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1

			.text
			daddi r23,r0,array
			daddi r9,r0,16
			daddi r8,r0,0	
loop:		dsll r11,r8,3
			dadd r10,r11,r23
			ld r12,0(r10)			#上面三行取数组的值
			daddi r8,r8,1
			
			bne r12,r0,loop		#如果数组值不为0则跳转到loop
			
			slt r10,r8,r9			#次循环判定是为了保证每次都循环16次
			bne r10,r0,loop

			daddi r17,r0,1		#此行代码是为了方便观察Cycles窗口中跳转的情况
		halt
