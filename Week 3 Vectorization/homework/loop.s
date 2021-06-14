	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14	sdk_version 10, 14
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4               ## -- Begin function main
LCPI0_0:
	.long	1127219200              ## 0x43300000
	.long	1160773632              ## 0x45300000
	.long	0                       ## 0x0
	.long	0                       ## 0x0
LCPI0_1:
	.quad	4841369599423283200     ## double 4503599627370496
	.quad	4985484787499139072     ## double 1.9342813113834067E+25
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3
LCPI0_2:
	.quad	4472406533629990549     ## double 1.0000000000000001E-9
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%rbx
	subq	$12312, %rsp            ## imm = 0x3018
	.cfi_offset %rbx, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -32(%rbp)
	movl	$0, -12324(%rbp)
	leaq	L_.memset_pattern.4(%rip), %rbx
	leaq	-4128(%rbp), %rdi
	movl	$4096, %edx             ## imm = 0x1000
	movq	%rbx, %rsi
	callq	_memset_pattern16
	leaq	-8224(%rbp), %rdi
	movl	$4096, %edx             ## imm = 0x1000
	movq	%rbx, %rsi
	callq	_memset_pattern16
	leaq	-12320(%rbp), %rdi
	movl	$4096, %edx             ## imm = 0x1000
	movq	%rbx, %rsi
	callq	_memset_pattern16
	xorl	%ebx, %ebx
	callq	_mach_absolute_time
	movq	%rax, %r14
	.p2align	4, 0x90
LBB0_1:                                 ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB0_2 Depth 2
                                        ##     Child Loop BB0_4 Depth 2
	xorl	%eax, %eax
	.p2align	4, 0x90
LBB0_2:                                 ##   Parent Loop BB0_1 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	vmovdqa	-8224(%rbp,%rax,8), %xmm0
	vpaddd	-4128(%rbp,%rax,8), %xmm0, %xmm0
	vmovd	%xmm0, -12320(%rbp,%rax,8)
	vpextrd	$2, %xmm0, -12312(%rbp,%rax,8)
	addq	$2, %rax
	cmpq	$510, %rax              ## imm = 0x1FE
	jne	LBB0_2
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	$1020, %eax             ## imm = 0x3FC
	.p2align	4, 0x90
LBB0_4:                                 ##   Parent Loop BB0_1 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	movl	-8224(%rbp,%rax,4), %ecx
	addl	-4128(%rbp,%rax,4), %ecx
	movl	%ecx, -12320(%rbp,%rax,4)
	addq	$2, %rax
	cmpq	$1024, %rax             ## imm = 0x400
	jb	LBB0_4
## %bb.5:                               ##   in Loop: Header=BB0_1 Depth=1
	incl	%ebx
	cmpl	$10000000, %ebx         ## imm = 0x989680
	jne	LBB0_1
## %bb.6:
	callq	_mach_absolute_time
	movq	%rax, %rbx
	leaq	-12324(%rbp), %rdi
	callq	_rand_r
	movl	%eax, %ecx
	sarl	$31, %ecx
	shrl	$22, %ecx
	addl	%eax, %ecx
	andl	$-1024, %ecx            ## imm = 0xFC00
	subl	%ecx, %eax
	cltq
	movl	-12320(%rbp,%rax,4), %r15d
	leaq	_tdiff.timebase(%rip), %rdi
	callq	_mach_timebase_info
	subq	%r14, %rbx
	vmovq	%rbx, %xmm0
	vpunpckldq	LCPI0_0(%rip), %xmm0, %xmm0 ## xmm0 = xmm0[0],mem[0],xmm0[1],mem[1]
	vsubpd	LCPI0_1(%rip), %xmm0, %xmm0
	vhaddpd	%xmm0, %xmm0, %xmm0
	movl	_tdiff.timebase(%rip), %eax
	vcvtsi2sdq	%rax, %xmm1, %xmm1
	movl	_tdiff.timebase+4(%rip), %eax
	vcvtsi2sdq	%rax, %xmm2, %xmm2
	vmulsd	%xmm0, %xmm1, %xmm0
	vdivsd	%xmm2, %xmm0, %xmm0
	vmulsd	LCPI0_2(%rip), %xmm0, %xmm0
	leaq	L_.str(%rip), %rdi
	leaq	L_.str.1(%rip), %rcx
	leaq	L_.str.2(%rip), %r8
	movl	$1024, %esi             ## imm = 0x400
	movl	$10000000, %edx         ## imm = 0x989680
	movb	$1, %al
	callq	_printf
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	cmpq	-32(%rbp), %rax
	jne	LBB0_8
## %bb.7:
	movl	%r15d, %eax
	addq	$12312, %rsp            ## imm = 0x3018
	popq	%rbx
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
LBB0_8:
	callq	___stack_chk_fail
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Elapsed execution time: %f sec; N: %d, I: %d, __OP__: %s, __TYPE__: %s\n"

L_.str.1:                               ## @.str.1
	.asciz	"+"

L_.str.2:                               ## @.str.2
	.asciz	"uint32_t"

.zerofill __DATA,__bss,_tdiff.timebase,8,2 ## @tdiff.timebase
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4               ## @.memset_pattern.4
L_.memset_pattern.4:
	.long	1                       ## 0x1
	.long	1                       ## 0x1
	.long	1                       ## 0x1
	.long	1                       ## 0x1


.subsections_via_symbols
