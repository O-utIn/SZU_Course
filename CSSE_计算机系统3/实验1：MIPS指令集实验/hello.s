.data
message:  .asciiz "Hello World!"
CONTROL: .word32 0x10000
DATA:    .word32 0x10008

.text
main:
lwu r12,DATA(r0)    ;  M[DATA] -> R12
daddi r21,r0,message  ; MESSAGE -> M[R21]
sd r21,0(r12)           ;  MESSAGE -> M[R12]

lwu r13,CONTROL(r0) ;  M[CONTROL]->R13
daddi r20,r0,4       ;  4->M[R20]
sd r20,0(r13)           ; SET CONTROL = 4 

halt
