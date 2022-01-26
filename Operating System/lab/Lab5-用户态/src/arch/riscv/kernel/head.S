.section .text.init

.global _start
.extern stack_top
.extern start_kernel
.extern kinit
.extern bss_start
.extern bss_end
.extern trap_s
.local trap_m
.local s_mode
.local init_bss
.local init_bss_done

#define mtimecmp 0x2004000
#define mtime 0x200bff8
#define reg_size 0x8

#.equ mtimecmp, 0x2004000
#.equ mtime, 0x200bff8
#.equ reg_size, 0x8

.align 3

_start:

	# To set m mode exception handler -> mtvec = trap_m
	la t0, trap_m
	csrw mtvec, t0

	# To disable global interrupt -> mstatus[3]mstatus.mie = 0
	li t1, 0x8
	csrc mstatus, t1

	# To change mode with mret intruction, need to set 
	# -> mepc = s_mode
	la t0, s_mode
	csrw mepc, t0
	# -> mstatus.mpie = 1 (mstatus[7])
	li t0, 0x80
	csrs mstatus, t0
	# -> mstatus.mpp = 01 for s mode(mstatus[12:11])
	csrr t0, mstatus
	addi t1, zero, 0x1
	slli t1, t1, 11
	or t0, t0, t1	# set [11] = 1
	slli t1, t1, 1
	not t1, t1
	and t0, t0, t1	# set [12] = 0
	csrw mstatus, t0
	# Before swithcing mode, initialize bbs section
#	la t0, bss_start
#	la t1, bss_end
#init_bss:
#	bge t0, t1, init_bss_done
#	sd zero, 0(t0)
#	addi t0, t0, 8
#	j init_bss
#init_bss_done:
	# Now to initialize mtimecmp = mtime + 1000000
	li t0, mtime
	ld t1, 0(t0)
	# debug : make some change here
	#ld t1, 0(mtime)
	li t0, 0x1000000
	add t1, t0, t1
	li t0, mtimecmp
	sd t1, 0(t0)
	#sd t1,0(mtimecmp)
	
	# Before switching mode, prepare timer interrupt delegation
	# -> set mideleg[5] = 1
	addi t0, zero, 1
	slli t0, t0, 5
	csrr t1, mideleg
	or t1, t1, t0
	csrw mideleg, t1

	# set medeleg[12 & 13 & 15]
	li t0, 0x0B000
	csrr t1,medeleg
	or t1,t1,t0
	csrw medeleg, t1 	# enable page fault delegation
	
	# [ !update in lab5 : set medeleg[8] = 1 ]
	li t0, 0x100
	csrs medeleg, t0	# set medeleg[8]

	# -> set sstatus.sie = 1 sie[stie] = 1
	addi t0, zero, 2
	csrr t1, sstatus
	or t1, t1, t0
	csrw sstatus, t1
	addi t0, zero, 0x20
	csrr t1, sie
	or t1, t1, t0
	csrw sie, t1
	# -> set mstatus.mie = 1 mie[mtie] = 1
	addi t0, zero, 0x8
	csrr t1, mstatus
	or t1, t1, t0
	csrw mstatus, t1
	addi t0, zero, 0x80
	csrr t1, mie
	or t1, t1, t0
	csrw mie, t1
	# To set mscratch
	la t0, init_stack_top
	li t1, 1
	slli t1, t1, 12
	sub t0, t0, t1
	csrw mscratch, t0

	#li t1,4194304
	#sub t0,t0,t1
	
	mret

s_mode:
	la t0, vm_boot
	# naive mapping vm_boot in data segment
    srli t0, t0, 12
    li t1, 8 << 60
    or t0, t0, t1
    csrw satp, t0
    sfence.vma
	# To set s mode exception handler -> stvec = trap_s
	la t0, trap_s
	li t1, 0xffffffdf80000000
	add t0, t0, t1
	csrw stvec, t0
	# To set stack pointer
	la sp, init_stack_top
	add sp, sp, t1	
	#lab5	
	# add t0,sp,x0
	# csrw sscratch, t0
	# To jump to start_kernel
	la t0, start_kernel
	add t0, t0, t1
    jr t0

.align 2
trap_m:
	csrrw tp, mscratch, tp # swap tp and mscratch
	sd t0, 5*reg_size(tp) # Save t0
	mv t0, sp
	mv sp, tp
	sd t0, 2*reg_size(sp) # Save old sp
	csrrw tp, mscratch, tp # swap tp and mscratch
	# addi sp, sp, -32*reg_size
	sd x1, 1*reg_size(sp)
	# sd x2, 2*reg_size(sp)
	sd x3, 3*reg_size(sp)
	sd x4, 4*reg_size(sp)
	# sd x5, 5*reg_size(sp)
	sd x6, 6*reg_size(sp)
	sd x7, 7*reg_size(sp)
	sd x8, 8*reg_size(sp)
	sd x9, 9*reg_size(sp)
	sd x10, 10*reg_size(sp)
	sd x11, 11*reg_size(sp)
	sd x12, 12*reg_size(sp)
	sd x13, 13*reg_size(sp)
	sd x14, 14*reg_size(sp)
	sd x15, 15*reg_size(sp)
	sd x16, 16*reg_size(sp)
	sd x17, 17*reg_size(sp)
	sd x18, 18*reg_size(sp)
	sd x19, 19*reg_size(sp)
	sd x20, 20*reg_size(sp)
	sd x21, 21*reg_size(sp)
	sd x22, 22*reg_size(sp)
	sd x23, 23*reg_size(sp)
	sd x24, 24*reg_size(sp)
	sd x25, 25*reg_size(sp)
	sd x26, 26*reg_size(sp)
	sd x27, 27*reg_size(sp)
	sd x28, 28*reg_size(sp)
	sd x29, 29*reg_size(sp)
	sd x30, 30*reg_size(sp)
	sd x31, 31*reg_size(sp)
handler_m:
	csrr t0, mcause
	addi t1, zero, 1
	slli t1, t1, 63
	and t1, t1, t0
	slli t0, t0, 1
	srli t0, t0, 1
	beqz t1, exception_m
interrupt_m:
	li t1, 7
	bne t1, t0, handler_m_done
	# machine timer interrupt
	# -> unset mie[mtie] and set sip[stip]
	li t0, 0x80
	not t0, t0
	csrr t1, mie
	and t1, t1, t0
	csrw mie, t1	# set mie[7] = 0
	li t0, 0x20
	csrr t1, mip
	or t1, t1, t0
	csrw mip, t1	# set sip[5] = 1
	j handler_m_done
exception_m:
	li t1, 9
	bne t1, t0, handler_m_done
	# ecall from S-mode -> mtimecmp += 1000 and set mie[mtie]
	li t0, mtime
	ld t1, 0(t0)

	#li t2, 0xFFF000
	li t2,0xF00000
	add t1, t1, t2
	li t0, mtimecmp
	sd t1, 0(t0)
	#sd t1,0(mtimecmp)	
	li t0, 0x80
	csrr t1, mie
	or t1, t1, t0
	csrw mie, t1
	li t0, 0x20
	csrc mip, t0
	# need set mepc+=4
	csrr t0, mepc
	addi t0, t0, 4
	csrw mepc, t0
handler_m_done:
	ld x1, 1*reg_size(sp)
	# ld x2, 2*reg_size(sp)
	ld x3, 3*reg_size(sp)
	ld x4, 4*reg_size(sp)
	# ld x5, 5*reg_size(sp)
	ld x6, 6*reg_size(sp)
	ld x7, 7*reg_size(sp)
	ld x8, 8*reg_size(sp)
	ld x9, 9*reg_size(sp)
	ld x10, 10*reg_size(sp)
	ld x11, 11*reg_size(sp)
	ld x12, 12*reg_size(sp)
	ld x13, 13*reg_size(sp)
	ld x14, 14*reg_size(sp)
	ld x15, 15*reg_size(sp)
	ld x16, 16*reg_size(sp)
	ld x17, 17*reg_size(sp)
	ld x18, 18*reg_size(sp)
	ld x19, 19*reg_size(sp)
	ld x20, 20*reg_size(sp)
	ld x21, 21*reg_size(sp)
	ld x22, 22*reg_size(sp)
	ld x23, 23*reg_size(sp)
	ld x24, 24*reg_size(sp)
	ld x25, 25*reg_size(sp)
	ld x26, 26*reg_size(sp)
	ld x27, 27*reg_size(sp)
	ld x28, 28*reg_size(sp)
	ld x29, 29*reg_size(sp)
	ld x30, 30*reg_size(sp)
	ld x31, 31*reg_size(sp)
	# addi sp, sp, 32*reg_size
	ld t0, 5*reg_size(sp)
	ld sp, 2*reg_size(sp)
	mret


	.globl __enablemie
__enablemie:
	addi t0, zero, 0x80
	csrr t1, mie
	or t1, t1, t0
	csrw mie, t1

	ret

.section .data
.align 12
vm_boot:
    .quad 0
    .quad 0
    # 2: 0x80000000
    .quad (0x80000 << 10) | 0xcf
    .zero 379 * 8
	# 382: device address
	.quad 0xcf
	.quad 0
    # 384: 0xffffffe000000000
    .quad (0x80000 << 10) | 0xcf
    .quad 0
