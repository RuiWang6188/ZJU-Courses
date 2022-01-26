# define WORD_SIZE 8
# define CONTEXT_SIZE (30 * WORD_SIZE)

COUNT:
.dword 0

.section .text.entry

.globl trap_s	# supervisor mode exeception handler (trap_s)
.align 3
trap_s:
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
	csrr t0, sstatus
	sd t0, 28 * WORD_SIZE(sp)
	csrr t0, stvec
	sd t0, 29 * WORD_SIZE(sp)
	csrr t0, sepc
	sd t0, 30 * WORD_SIZE(sp)

	csrr t0, scause
	bgez t0, s_exception 	# determine interrupt(<0) or exception(>=0)
	andi t0, t0, 0x3f		# isolate the exception code field
	li t1, 5
	bne t0, t1, s_other_int # branch if not a timer interrupt

	# handle timer interrupt

	# increase the count
	la t0, COUNT
	ld t1, (t0)				# t1 = val(COUNT1)
	addi t1, t1, 1			# COUNT1 += 1 
	sd t1, (t0)

	# print the interrupt count
	li t2, 100000
	blt t1, t2, skip_print
	and t1, t1, x0
	sd t1, (t0)				# set COUNT1 = 0

	call print_message

	# clear sip[stip] (bit 5)
skip_print:
	li t0, 32
	csrc sip, t0
	
	# init ecall jump to M mode
	ecall

	j trap_s_ret


.globl trap_s_ret
.align 3
trap_s_ret:
	# restore the CSRs
	ld t0, 30 * WORD_SIZE(sp)
	csrw sepc, t0					
	ld t0, 29 * WORD_SIZE(sp)
	csrw stvec, t0
	ld t0, 28 * WORD_SIZE(sp)
	csrw sstatus, t0

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
	sret  

.globl s_other_int
.align 3
s_other_int:
	j trap_s_ret


.globl s_exception
.align 3
s_exception:
	# modify the mepc in the stack (mpec+=4)
	ld t0, 30 * WORD_SIZE(sp)
	addi t0, t0, 4
	sd t0, 30 * WORD_SIZE(sp)

	j trap_s_ret


