# define WORD_SIZE 8
# define CONTEXT_SIZE (30 * WORD_SIZE)

.section .text.init
.globl _start
.align 3 
_start:
	# write mstatus reg
	li t0, 0x0A		# set mstatus.mie = 1 && mstatus.sie = 1 
	csrw mstatus, t0
	
	# set sstatus[sie]
	li t0, 0x02
	csrs sstatus, t0	# set sstatus[sie] = 1
	
	# set mie[mtie]		mie.MTIE(bit 7)
	li t0,0x80         # set mie[mtie] = 1
	csrs mie, t0		
	
	# set sie[stie]		
	li t0, 0x20
	csrs sie, t0		# set sie[stie] = 1
	
	# set mideleg[5]
	li t0, 0x20			# t0 = 0010 0000
	csrs mideleg, t0	# set mideleg[5] = 1

	# write mtvec = &trap_m
	la t0, trap_m	# t0 = &trap_m
	csrw mtvec, t0	

	# write stvec = &trap_s
	la t0, trap_s	# t0 = &trap_s
	csrw stvec, t0
	
	# write mepc = &s_mode
	la t0, s_mode		# t0 = &s_mode
	csrw mepc,t0
	
	# initialize bss segment
	la t0, bss_start    
	la t1, bss_end
1:
	sd x0, (t0)
	add t0, t0, 8
	blt t0, t1, 1b
	
	# initialize stack pointer
	la sp, stack_top   # set stack point to stack_top

	# set mtimecmp = mtime + 1000000
	li t0, 0x200bff8    # t0 = addr[mtime]
	li t1, 0x2004000	# t1 = addr[mtimecmp]
	ld t2, (t0)			# t2 = val[mtime]

					#addi t2, t2, 1000000
	li t0, 1000000
	add t2, t2, t0

	sd t2, (t1)			# val[mtimecmp] = val[mtime] + 1000000
	
	# mode switch(M->S)
	mret	# PC = mepc(s_mode) -> move from machine mode to supervisor mode

s_mode:
	la sp, stack_top
	j start_kernel		# jump from supervisor mode to start_kernel function



.globl trap_m  # machine mode trap handler (trap_m)
.align 3
trap_m:
	# save all the registers
	addi sp, sp, -CONTEXT_SIZE
	sd x1, 0 * WORD_SIZE(sp)
	sd x5, 1 * WORD_SIZE(sp)
	sd x6, 2 * WORD_SIZE(sp)
	sd x7, 3 * WORD_SIZE(sp)
	sd x8, 4 * WORD_SIZE(sp)
	sd x9, 5 * WORD_SIZE(sp)
	sd x10, 6 * WORD_SIZE(sp)
	sd x11, 7 * WORD_SIZE(sp)
	sd x12, 8 * WORD_SIZE(sp)
	sd x13, 9 * WORD_SIZE(sp)
	sd x14, 10 * WORD_SIZE(sp)
	sd x15, 11 * WORD_SIZE(sp)
	sd x16, 12 * WORD_SIZE(sp)
	sd x17, 13 * WORD_SIZE(sp)
	sd x18, 14 * WORD_SIZE(sp)
	sd x19, 15 * WORD_SIZE(sp)
	sd x20, 16 * WORD_SIZE(sp)
	sd x21, 17 * WORD_SIZE(sp)
	sd x22, 18 * WORD_SIZE(sp)
	sd x23, 19 * WORD_SIZE(sp)
	sd x24, 20 * WORD_SIZE(sp)
	sd x25, 21 * WORD_SIZE(sp)
	sd x26, 22 * WORD_SIZE(sp)
	sd x27, 23 * WORD_SIZE(sp)
	sd x28, 24 * WORD_SIZE(sp)
	sd x29, 25 * WORD_SIZE(sp)
	sd x30, 26 * WORD_SIZE(sp)
	sd x31, 27 * WORD_SIZE(sp)

	# save the needed CSRs
	csrr t0, mstatus
	sd t0, 28 * WORD_SIZE(sp)
	csrr t0, mtvec
	sd t0, 29 * WORD_SIZE(sp)
	csrr t0, mepc
	sd t0, 30 * WORD_SIZE(sp)

	csrr t0, mcause			# read trap cause
	bgez t0, m_exception 	# determine interrupt(<0) or exception(>=0), branch if exception
	andi t0, t0, 0x3f		# isolate the exception code field
	li t1, 7
	bne t0, t1, m_other_int # branch if not a timer interrupt

	# handle timer interrupt

	# disable mie[mtie] (bit 7)
	li t0, 0x080
	csrc mie, t0

	# enable sip[stip] (bit 5)
	li t0, 0x20
	csrs mip, t0


	# timer interrupt, not need (mepc+4)
	j trap_m_ret

# before calling this ret, you should modify the mepc in the stack
.globl trap_m_ret
.align 3
trap_m_ret:
	# restore the CSRs
	ld t0, 30 * WORD_SIZE(sp)
	csrw mepc, t0					
	ld t0, 29 * WORD_SIZE(sp)
	csrw mtvec, t0
	ld t0, 28 * WORD_SIZE(sp)
	csrw mstatus, t0

	# restore all the registers
	ld x31, 27 * WORD_SIZE(sp)
	ld x30, 26 * WORD_SIZE(sp)
	ld x29, 25 * WORD_SIZE(sp)
	ld x28, 24 * WORD_SIZE(sp)
	ld x27, 23 * WORD_SIZE(sp)
	ld x26, 22 * WORD_SIZE(sp)
	ld x25, 21 * WORD_SIZE(sp)
	ld x24, 20 * WORD_SIZE(sp)
	ld x23, 19 * WORD_SIZE(sp)
	ld x22, 18 * WORD_SIZE(sp)
	ld x21, 17 * WORD_SIZE(sp)
	ld x20, 16 * WORD_SIZE(sp)
	ld x19, 15 * WORD_SIZE(sp)
	ld x18, 14 * WORD_SIZE(sp)
	ld x17, 13 * WORD_SIZE(sp)
	ld x16, 12 * WORD_SIZE(sp)
	ld x15, 11 * WORD_SIZE(sp)
	ld x14, 10 * WORD_SIZE(sp)
	ld x13, 9 * WORD_SIZE(sp)
	ld x12, 8 * WORD_SIZE(sp)
	ld x11, 7 * WORD_SIZE(sp)
	ld x10, 6 * WORD_SIZE(sp)
	ld x9, 5 * WORD_SIZE(sp)
	ld x8, 4 * WORD_SIZE(sp)
	ld x7, 3 * WORD_SIZE(sp)
	ld x6, 2 * WORD_SIZE(sp)
	ld x5, 1 * WORD_SIZE(sp)
	ld x1, 0 * WORD_SIZE(sp)

	addi sp, sp, CONTEXT_SIZE
	mret  	



.globl m_exception
.align 3
m_exception:
	csrr t0, mcause
	andi t0, t0, 0x3f
	li t1, 9					# t1 = ecall from S-mode
	beq t0, t1, ecall_from_s	# branch if ecall from s mode

	# otherwise it is other exceptions
	
	# modify the mepc in the stack (mpec+=4)
	ld t0, 30 * WORD_SIZE(sp)
	addi t0, t0, 4
	sd t0, 30 * WORD_SIZE(sp)

	j trap_m_ret



.globl m_other_int
.align 3
m_other_int:
	j trap_m_ret



.globl ecall_from_s
.align 3
ecall_from_s:
	# handle ecall from s mode
	# set mtimecmp += 100000
	li t0, 0x2004000	# t0 = addr[mtimecmp]
	ld t1, (t0)
	#addi t1, t1, 100000
	li t2, 100000
	add t1, t1, t2
	sd t1, (t0)		

	# enable mie[mtie] (bit 7)
	li t0, 0x080
	csrs mie, t0
	
	# modify the mepc in the stack (mpec+=4)
	ld t0, 30 * WORD_SIZE(sp)
	addi t0, t0, 4
	sd t0, 30 * WORD_SIZE(sp)

	## ToDo

	j trap_m_ret


