# define WORD_SIZE 8
# define CONTEXT_SIZE (33 * WORD_SIZE)

COUNT:
.dword 0

.section .text.entry

.extern do_timer
.extern print_inst_pgfault
.extern print_ld_pgfault
.extern print_st_pgfault

.globl trap_s	# supervisor mode exeception handler (trap_s)
.align 3
trap_s:
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
	csrr t0, sstatus
	sd t0, 31 * WORD_SIZE(sp)
	csrr t0, stvec
	sd t0, 32 * WORD_SIZE(sp)
	csrr t0, sepc
	sd t0, 33 * WORD_SIZE(sp)

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

	call do_timer

	# clear sip[stip] (bit 5)
skip_print:
	li t0, 32
	csrc sip, t0
	
	# init ecall jump to M mode
	ecall

	j trap_s_ret


.align 3
trap_s_ret:
	# restore the CSRs
	ld t0, 33 * WORD_SIZE(sp)
	csrw sepc, t0					
	ld t0, 32 * WORD_SIZE(sp)
	csrw stvec, t0
	ld t0, 31 * WORD_SIZE(sp)
	csrw sstatus, t0

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
	sret  

.globl s_other_int
.align 3
s_other_int:
	j trap_s_ret


.align 3
s_exception:
	csrr t0, scause

	li t1, 0x08000		# t1 = bit 15
	and t2, t0, t1
	beq t1, t2, store_pgfault_exception

	li t1, 0x2000	# t1 = bit 13
	and t2, t0, t1
	beq t1, t2, load_pgfault_exception

	li t1, 0x4000	# t1 = bit 12
	and t2, t0, t1
	beq t1, t2, inst_pgfault_exception

	# modify the sepc in the stack (spec+=4)
	ld t0, 33 * WORD_SIZE(sp)
	addi t0, t0, 4
	sd t0, 33 * WORD_SIZE(sp)

	j trap_s_ret

.align 3
store_pgfault_exception:
	call print_st_pgfault
	# modify the sepc in the stack (spec+=4)
	ld t0, 33 * WORD_SIZE(sp)
	addi t0, t0, 4
	sd t0, 33 * WORD_SIZE(sp)

	j trap_s_ret
	
.align 3
load_pgfault_exception:
	call print_ld_pgfault
	# modify the sepc in the stack (spec+=4)
	ld t0, 33 * WORD_SIZE(sp)
	addi t0, t0, 4
	sd t0, 33 * WORD_SIZE(sp)

	j trap_s_ret	


.align 3
inst_pgfault_exception:
	call print_inst_pgfault
	# modify the sepc in the stack (spec+=4)
	ld t0, 33 * WORD_SIZE(sp)
	addi t0, t0, 4
	sd t0, 33 * WORD_SIZE(sp)

	j trap_s_ret	


.globl __switch_to    #a0 = prev, a1 = next 
.align 3
__switch_to:
	addi t0, a0, 40	# thread offset = 40B
	sd ra, 0 * WORD_SIZE(t0)
	sd sp, 1 * WORD_SIZE(t0)
	sd s0, 2 * WORD_SIZE(t0)
	sd s1, 3 * WORD_SIZE(t0)
	sd s2, 4 * WORD_SIZE(t0)
	sd s3, 5 * WORD_SIZE(t0)
	sd s4, 6 * WORD_SIZE(t0)
	sd s5, 7 * WORD_SIZE(t0)
	sd s6, 8 * WORD_SIZE(t0)
	sd s7, 9 * WORD_SIZE(t0)
	sd s8, 10 * WORD_SIZE(t0)
	sd s9, 11 * WORD_SIZE(t0)
	sd s10, 12 * WORD_SIZE(t0)
	sd s11, 13 * WORD_SIZE(t0)
	
	addi t0, a1, 40
	ld ra, 0 * WORD_SIZE(t0)
	ld sp, 1 * WORD_SIZE(t0)
	ld s0, 2 * WORD_SIZE(t0)
	ld s1, 3 * WORD_SIZE(t0)
	ld s2, 4 * WORD_SIZE(t0)
	ld s3, 5 * WORD_SIZE(t0)
	ld s4, 6 * WORD_SIZE(t0)
	ld s5, 7 * WORD_SIZE(t0)
	ld s6, 8 * WORD_SIZE(t0)
	ld s7, 9 * WORD_SIZE(t0)
	ld s8, 10 * WORD_SIZE(t0)
	ld s9, 11 * WORD_SIZE(t0)
	ld s10, 12 * WORD_SIZE(t0)
	ld s11, 13 * WORD_SIZE(t0)
	ret 



