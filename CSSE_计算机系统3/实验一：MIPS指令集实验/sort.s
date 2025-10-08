.data
arr: .word 1, 3, 4, 2, 5, 7, 0, 8,6,9
before: .asciiz "before sort the array is:\n"
after: .asciiz "after sort the array is:\n"
CONTROL: .word 0x10000
DATA: .word 0x10008
SP: .word 0x300

.text
main:
    #cout  “before sort the array is”
    ld r13, CONTROL(r0)  
    ld r14, DATA(r0)  
    daddi r9, r0, 4  
    daddi r8, r0, before
	sd r8, (r14)  
    sd r9, (r13)  

    daddi r9, r0, 2 
    daddi r2, r0, 10 
    daddi r1, r0, 0
firstCout:
    dsll r3, r1, 3 
    ld r8, arr(r3)  
    sd r8, (r14) 
    sd r9, (r13) 
	
    daddi r1, r1, 1  
    bne r1, r2, firstCout  
    # sort()
    ld r29, SP(r0)  
    daddi r4, r0, arr  
    daddi r5, r0, 10 
    jal sort  

    #cout  “after sort the array is”
    daddi r9, r0, 4  
    daddi r8, r0, after
    sd r8, (r14)  
	sd r9, (r13)  
	
    # print the array
    daddi r9, r0, 2 
    daddi r2, r0, 10  
    daddi r1, r0, 0 
secCout:
    dsll r3, r1, 3  
    ld r8, arr(r3)  
    sd r8, (r14)  
    sd r9, (r13)  
	
    daddi r1, r1, 1  # , i++
    bne r1, r2, secCout 
    halt

sort:
    daddi r29, r29, -24  
    sd $ra, 16(r29)  
    sd r13, 8(r29)  
    sd r14,0(r29)  

    dadd r22, r4, r0  
    daddi r23, r5, 0  

    and r18, r18, r0 
loop1:
    # for (i = 0; i < n; i++) 
    slt r10, r18, r23  #  if  i <  n
    beq r10, r0, end1   # if  i >=  n
daddi r19, r18, -1 

loop2:
# for (j = i - 1; j >= 0; j--)
    slti r10, r19, 0  # j < 0
    bne r10, r0, end2  
	
    dsll r11, r19, 3  
    dadd r12, r22, r11  
    ld r16, 0(r12)  
    ld r17, 8(r12)  
	
    # if (a[j] > a[j + 1]) 
    slt r10, r17, r16  
    beq r10, r0, end2  
    dadd r4, r0, r12  
daddi r5, r12, 8  
# swap(a[j], a[j + 1])
    jal swap  
	
    daddi r19, r19, -1  # j--
    j loop2
	
end2:
    daddi r18,r18,1  # i--
    j loop1  

end1:
    # restore
    ld r14, 0(r29)  
    ld r13, 8(r29)  
    ld $ra, 16(r29)  
    daddi r29, r29, 24  
    jr $ra  

swap:  
    # swap( a, b)
    ld r8, 0(r4)  
    ld r10, 0(r5)  
    sd r10, 0(r4)  
    sd r8, 0(r5)  
    jr $ra  
