# define WORD_SIZE 8
# define CONTEXT_SIZE (33 * WORD_SIZE)

.extern start_kernel
.extern bitmap_init
.extern paging_init
.extern kernel_pgtbl
.extern init_stack_top
.extern stack_top

.section .text.init
.globl _start
.align 3 
_start:
	# clear satp register -> disable MMU for now
	li t0, 0
	csrw satp, t0

	# write mstatus reg
	li t0, 0x80A		# set mstatus.mie = 1 && mstatus.sie = 1 && mstatus.mpp = 01 && mstatus.mpie = 0
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

	# set medeleg[12 & 13 & 15]
	li t0, 0x0B000
	csrs medeleg, t0 	# enable page fault delegation

	# write mtvec = &trap_m
	la t0, trap_m	# t0 = &trap_m
	csrw mtvec, t0	

	# write stvec = &trap_s
	la t0, trap_s
	csrw stvec, t0
	
	# write mepc = &s_mode
	la t0, s_mode		# t0 = &s_mode
	csrw mepc,t0

	# initialize mscratch
	la t0, stack_top 		# t0 = &stack_top(pa)
	csrw mscratch, t0		# set mscratch register = &stack_top(pa)

	# initialize sp
	la sp, init_stack_top # MMU is disabled, so we can just use PA
	
	# initialize bss segment
	la t0, bss_start    
	la t1, bss_end
1:
	sd x0, (t0)
	add t0, t0, 8
	blt t0, t1, 1b
	
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

	call paging_init

	li t0, 8			# set satp mode = Sv39
	slli t0, t0, 60		# t0 = 8L << 60
	la t1, kernel_pgtbl # t1 = &kernel_pgtbl
	ld t2, (t1)			# t2 = value of kernel_pgtbl
	srli t1, t2, 12		# move to the lower 12 bits of satp
	or t0, t0, t1		# t0 = SATP_SV39 | (kernel_pgtbl >> 12)
	csrw satp, t0		# set satp, enable MMU

	sfence.vma zero, zero

	# write stvec = &trap_s (va)
	la t0, trap_s		# t0 = &trap_s(pa)
	li t1, 0x080000000 	# t1 = KERNBASE
	sub t0, t0, t1		# t0 = &trap_s - 0x80000000 (offset)
	li t1, 0xffffffe000000000	# t1 = HIGHBASE
	add t0, t0, t1 		# t0 = &trap_s(pa) - KERNBASE + HIGHBASE (t0 = &trap_s(va))
	csrw stvec, t0

	# initialize stack pointer (va)
	la t0, init_stack_top   # t0 = &init_stack_top(pa)
	li t1, 0x080000000		# t1 = KERNBASE
	sub t0, t0, t1
	li t1, 0xffffffe000000000
	add t0, t0, t1			# t0 = &init_stack_top(va)
	mv sp, t0				# sp = &init_stack_top(va)

	la t0, start_kernel # t0 = &start_kernel(pa)
	li t1, 0x080000000		
	sub t0, t0, t1
	li t1, 0xffffffe000000000
	add t0, t0, t1		# t1 = &start_kernel(va)
	jr t0				# jump from S mode to start_kernel(va)


.globl trap_m  # machine mode trap handler (trap_m)
.align 3
trap_m:
	csrrw sp, mscratch, sp 		

	# save sp; set sp = &stack_top
	# save all the registers
	addi sp, sp, -CONTEXT_SIZE
	sd x1, 0 * WORD_SIZE(sp)
	sd x2, 1 * WORD_SIZE(sp)
	sd x3, 2 * WORD_SIZE(sp)	
	sd x4, 3 * WORD_SIZE(sp)	
	sd x5, 4 * WORD_SIZE(sp)
	sd x6, 5 * WORD_SIZE(sp)
	sd x7, 6 * WORD_SIZE(sp)
	sd x8, 7 * WORD_SIZE(sp)
	sd x9, 8 * WORD_SIZE(sp)
	sd x10, 9 * WORD_SIZE(sp)
	sd x11, 10 * WORD_SIZE(sp)
	sd x12, 11 * WORD_SIZE(sp)
	sd x13, 12 * WORD_SIZE(sp)
	sd x14, 13 * WORD_SIZE(sp)
	sd x15, 14 * WORD_SIZE(sp)
	sd x16, 15 * WORD_SIZE(sp)
	sd x17, 16 * WORD_SIZE(sp)
	sd x18, 17 * WORD_SIZE(sp)
	sd x19, 18 * WORD_SIZE(sp)
	sd x20, 19 * WORD_SIZE(sp)
	sd x21, 20 * WORD_SIZE(sp)
	sd x22, 21 * WORD_SIZE(sp)
	sd x23, 22 * WORD_SIZE(sp)
	sd x24, 23 * WORD_SIZE(sp)
	sd x25, 24 * WORD_SIZE(sp)
	sd x26, 25 * WORD_SIZE(sp)
	sd x27, 26 * WORD_SIZE(sp)
	sd x28, 27 * WORD_SIZE(sp)
	sd x29, 28 * WORD_SIZE(sp)
	sd x30, 29 * WORD_SIZE(sp)
	sd x31, 30 * WORD_SIZE(sp)

	# save the needed CSRs
	csrr t0, mstatus
	sd t0, 31 * WORD_SIZE(sp)
	csrr t0, mtvec
	sd t0, 32 * WORD_SIZE(sp)
	csrr t0, mepc
	sd t0, 33 * WORD_SIZE(sp)

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
.align 3
trap_m_ret:
	# restore the CSRs
	ld t0, 33 * WORD_SIZE(sp)
	csrw mepc, t0					
	ld t0, 32 * WORD_SIZE(sp)
	csrw mtvec, t0
	ld t0, 31 * WORD_SIZE(sp)
	csrw mstatus, t0

	# restore all the registers
	ld x31, 30 * WORD_SIZE(sp)
	ld x30, 29 * WORD_SIZE(sp)
	ld x29, 28 * WORD_SIZE(sp)
	ld x28, 27 * WORD_SIZE(sp)
	ld x27, 26 * WORD_SIZE(sp)
	ld x26, 25 * WORD_SIZE(sp)
	ld x25, 24 * WORD_SIZE(sp)
	ld x24, 23 * WORD_SIZE(sp)
	ld x23, 22 * WORD_SIZE(sp)
	ld x22, 21 * WORD_SIZE(sp)
	ld x21, 20 * WORD_SIZE(sp)
	ld x20, 19 * WORD_SIZE(sp)
	ld x19, 18 * WORD_SIZE(sp)
	ld x18, 17 * WORD_SIZE(sp)
	ld x17, 16 * WORD_SIZE(sp)
	ld x16, 15 * WORD_SIZE(sp)
	ld x15, 14 * WORD_SIZE(sp)
	ld x14, 13 * WORD_SIZE(sp)
	ld x13, 12 * WORD_SIZE(sp)
	ld x12, 11 * WORD_SIZE(sp)
	ld x11, 10 * WORD_SIZE(sp)
	ld x10, 9 * WORD_SIZE(sp)
	ld x9, 8 * WORD_SIZE(sp)
	ld x8, 7 * WORD_SIZE(sp)
	ld x7, 6 * WORD_SIZE(sp)
	ld x6, 5 * WORD_SIZE(sp)
	ld x5, 4 * WORD_SIZE(sp)
	ld x4, 3 * WORD_SIZE(sp)	
	ld x3, 2 * WORD_SIZE(sp)	
	ld x2, 1 * WORD_SIZE(sp)	
	ld x1, 0 * WORD_SIZE(sp)

	addi sp, sp, CONTEXT_SIZE

	csrrw sp, mscratch, sp 	# restore sp; mscratch = &stack_top
	mret  	

.align 3
m_exception:
	csrr t0, mcause
	andi t0, t0, 0x3f
	li t1, 9					# t1 = ecall from S-mode
	beq t0, t1, ecall_from_s	# branch if ecall from s mode

	# otherwise it is other exceptions
	
	# modify the mepc in the stack (mpec+=4)
	ld t0, 33 * WORD_SIZE(sp)
	addi t0, t0, 4
	sd t0, 33 * WORD_SIZE(sp)

	j trap_m_ret

.align 3
m_other_int:
	j trap_m_ret

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
	ld t0, 33 * WORD_SIZE(sp)
	addi t0, t0, 4
	sd t0, 33 * WORD_SIZE(sp)

	## ToDo

	j trap_m_ret





