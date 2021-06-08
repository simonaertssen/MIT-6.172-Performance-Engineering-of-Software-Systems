	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14	sdk_version 10, 14
	.globl	_test                   ## -- Begin function test
	.p2align	4, 0x90
_test:                                  ## @test
Lfunc_begin0:
	.file	1 "/Users/SimonAertssen/Desktop/Zelfstudie/6.172 Performance Engineering of Software Systems/MIT-6.172-Performance-Engineering-of-Software-Systems/Week 3 Vectorization/recitation3" "example4.c"
	.loc	1 10 0                  ## example4.c:10:0
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	##DEBUG_VALUE: test:a <- $rdi
	##DEBUG_VALUE: test:a <- $rdi
Ltmp0:
	##DEBUG_VALUE: test:x <- $rdi
	##DEBUG_VALUE: test:x <- $rdi
	xorpd	%xmm1, %xmm1
	xorl	%eax, %eax
Ltmp1:
	##DEBUG_VALUE: test:i <- 0
	##DEBUG_VALUE: test:y <- 0.000000e+00
	xorpd	%xmm0, %xmm0
	.p2align	4, 0x90
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	##DEBUG_VALUE: test:x <- $rdi
	##DEBUG_VALUE: test:a <- $rdi
Ltmp2:
	.loc	1 18 7 prologue_end     ## example4.c:18:7
	addpd	(%rdi,%rax,8), %xmm1
	addpd	16(%rdi,%rax,8), %xmm0
	addpd	32(%rdi,%rax,8), %xmm1
	addpd	48(%rdi,%rax,8), %xmm0
	addpd	64(%rdi,%rax,8), %xmm1
	addpd	80(%rdi,%rax,8), %xmm0
	addpd	96(%rdi,%rax,8), %xmm1
	addpd	112(%rdi,%rax,8), %xmm0
Ltmp3:
	.loc	1 17 26                 ## example4.c:17:26
	addq	$16, %rax
	cmpq	$65536, %rax            ## imm = 0x10000
	jne	LBB0_1
## %bb.2:
	##DEBUG_VALUE: test:x <- $rdi
	##DEBUG_VALUE: test:a <- $rdi
Ltmp4:
	.loc	1 18 7                  ## example4.c:18:7
	addpd	%xmm1, %xmm0
	haddpd	%xmm0, %xmm0
Ltmp5:
	##DEBUG_VALUE: test:y <- undef
	.loc	1 20 3                  ## example4.c:20:3
	popq	%rbp
	retq
Ltmp6:
Lfunc_end0:
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3               ## -- Begin function main
LCPI1_0:
	.quad	4607182418800017408     ## double 1
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
Lfunc_begin1:
	.loc	1 23 0                  ## example4.c:23:0
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$524320, %rsp           ## imm = 0x80020
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movl	$1, %eax
Ltmp7:
	##DEBUG_VALUE: i <- 0
	movsd	LCPI1_0(%rip), %xmm0    ## xmm0 = mem[0],zero
	.p2align	4, 0x90
LBB1_1:                                 ## =>This Inner Loop Header: Depth=1
Ltmp8:
	##DEBUG_VALUE: i <- undef
	.loc	1 26 22 prologue_end    ## example4.c:26:22
	xorps	%xmm1, %xmm1
	cvtsi2sdl	%eax, %xmm1
	.loc	1 26 15 is_stmt 0       ## example4.c:26:15
	movapd	%xmm0, %xmm2
	divsd	%xmm1, %xmm2
	.loc	1 26 10                 ## example4.c:26:10
	movsd	%xmm2, -524312(%rbp,%rax,8)
Ltmp9:
	##DEBUG_VALUE: i <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
	.loc	1 26 22                 ## example4.c:26:22
	addsd	%xmm0, %xmm1
	.loc	1 26 15                 ## example4.c:26:15
	movapd	%xmm0, %xmm2
	divsd	%xmm1, %xmm2
	.loc	1 26 10                 ## example4.c:26:10
	movsd	%xmm2, -524304(%rbp,%rax,8)
Ltmp10:
	##DEBUG_VALUE: i <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
	.loc	1 25 21 is_stmt 1       ## example4.c:25:21
	addq	$2, %rax
	cmpq	$65537, %rax            ## imm = 0x10001
Ltmp11:
	.loc	1 25 3 is_stmt 0        ## example4.c:25:3
	jne	LBB1_1
Ltmp12:
## %bb.2:
	.loc	1 0 3                   ## example4.c:0:3
	xorpd	%xmm1, %xmm1
	xorl	%eax, %eax
	xorpd	%xmm0, %xmm0
	.p2align	4, 0x90
LBB1_3:                                 ## =>This Inner Loop Header: Depth=1
Ltmp13:
	.loc	1 18 7 is_stmt 1        ## example4.c:18:7
	addpd	-524304(%rbp,%rax,8), %xmm1
	addpd	-524288(%rbp,%rax,8), %xmm0
	addpd	-524272(%rbp,%rax,8), %xmm1
	addpd	-524256(%rbp,%rax,8), %xmm0
	addpd	-524240(%rbp,%rax,8), %xmm1
	addpd	-524224(%rbp,%rax,8), %xmm0
	addpd	-524208(%rbp,%rax,8), %xmm1
	addpd	-524192(%rbp,%rax,8), %xmm0
Ltmp14:
	.loc	1 17 26                 ## example4.c:17:26
	addq	$16, %rax
	cmpq	$65536, %rax            ## imm = 0x10000
	jne	LBB1_3
## %bb.4:
Ltmp15:
	.loc	1 18 7                  ## example4.c:18:7
	addpd	%xmm1, %xmm0
	haddpd	%xmm0, %xmm0
Ltmp16:
	##DEBUG_VALUE: main:sum <- undef
	##DEBUG_VALUE: test:y <- undef
	.loc	1 29 3                  ## example4.c:29:3
	movapd	%xmm0, -524320(%rbp)    ## 16-byte Spill
	leaq	L_.str(%rip), %rdi
	movb	$1, %al
	callq	_printf
Ltmp17:
	.loc	1 30 3                  ## example4.c:30:3
	leaq	L_.str.1(%rip), %rdi
	movb	$1, %al
	movaps	-524320(%rbp), %xmm0    ## 16-byte Reload
	callq	_printf
Ltmp18:
	.loc	1 0 3 is_stmt 0         ## example4.c:0:3
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	cmpq	-8(%rbp), %rax
	jne	LBB1_6
## %bb.5:
	.loc	1 31 1 is_stmt 1        ## example4.c:31:1
	xorl	%eax, %eax
	addq	$524320, %rsp           ## imm = 0x80020
	popq	%rbp
	retq
Ltmp19:
LBB1_6:
	.loc	1 0 1 is_stmt 0         ## example4.c:0:1
	callq	___stack_chk_fail
Ltmp20:
Lfunc_end1:
	.cfi_endproc
	.file	2 "/Users/SimonAertssen/Desktop/Zelfstudie/6.172 Performance Engineering of Software Systems/MIT-6.172-Performance-Engineering-of-Software-Systems/Week 3 Vectorization/recitation3" "/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/usr/include/i386/_types.h"
	.file	3 "/Users/SimonAertssen/Desktop/Zelfstudie/6.172 Performance Engineering of Software Systems/MIT-6.172-Performance-Engineering-of-Software-Systems/Week 3 Vectorization/recitation3" "/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/usr/include/sys/_types/_size_t.h"
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"The decimal floating point sum result is: %f\n"

L_.str.1:                               ## @.str.1
	.asciz	"The raw floating point sum result is: %a\n"

	.section	__DWARF,__debug_str,regular,debug
Linfo_string:
	.asciz	"Apple LLVM version 10.0.1 (clang-1001.0.46.4)" ## string offset=0
	.asciz	"example4.c"            ## string offset=46
	.asciz	"/Users/SimonAertssen/Desktop/Zelfstudie/6.172 Performance Engineering of Software Systems/MIT-6.172-Performance-Engineering-of-Software-Systems/Week 3 Vectorization/recitation3" ## string offset=57
	.asciz	"test"                  ## string offset=234
	.asciz	"double"                ## string offset=239
	.asciz	"a"                     ## string offset=246
	.asciz	"y"                     ## string offset=248
	.asciz	"i"                     ## string offset=250
	.asciz	"size_t"                ## string offset=252
	.asciz	"__darwin_size_t"       ## string offset=259
	.asciz	"long unsigned int"     ## string offset=275
	.asciz	"x"                     ## string offset=293
	.asciz	"main"                  ## string offset=295
	.asciz	"int"                   ## string offset=300
	.asciz	"__ARRAY_SIZE_TYPE__"   ## string offset=304
	.asciz	"sum"                   ## string offset=324
	.section	__DWARF,__debug_loc,regular,debug
Lsection_debug_loc:
Ldebug_loc0:
.set Lset0, Lfunc_begin0-Lfunc_begin0
	.quad	Lset0
.set Lset1, Lfunc_end0-Lfunc_begin0
	.quad	Lset1
	.short	1                       ## Loc expr size
	.byte	85                      ## DW_OP_reg5
	.quad	0
	.quad	0
Ldebug_loc1:
.set Lset2, Ltmp0-Lfunc_begin0
	.quad	Lset2
.set Lset3, Lfunc_end0-Lfunc_begin0
	.quad	Lset3
	.short	1                       ## Loc expr size
	.byte	85                      ## DW_OP_reg5
	.quad	0
	.quad	0
Ldebug_loc2:
.set Lset4, Ltmp1-Lfunc_begin0
	.quad	Lset4
.set Lset5, Lfunc_end0-Lfunc_begin0
	.quad	Lset5
	.short	2                       ## Loc expr size
	.byte	48                      ## DW_OP_lit0
	.byte	159                     ## DW_OP_stack_value
	.quad	0
	.quad	0
Ldebug_loc3:
.set Lset6, Ltmp1-Lfunc_begin0
	.quad	Lset6
.set Lset7, Ltmp5-Lfunc_begin0
	.quad	Lset7
	.short	2                       ## Loc expr size
	.byte	48                      ## DW_OP_lit0
	.byte	159                     ## DW_OP_stack_value
	.quad	0
	.quad	0
Ldebug_loc4:
.set Lset8, Ltmp7-Lfunc_begin0
	.quad	Lset8
.set Lset9, Ltmp8-Lfunc_begin0
	.quad	Lset9
	.short	3                       ## Loc expr size
	.byte	17                      ## DW_OP_consts
	.byte	0                       ## 0
	.byte	159                     ## DW_OP_stack_value
	.quad	0
	.quad	0
	.section	__DWARF,__debug_abbrev,regular,debug
Lsection_abbrev:
	.byte	1                       ## Abbreviation Code
	.byte	17                      ## DW_TAG_compile_unit
	.byte	1                       ## DW_CHILDREN_yes
	.byte	37                      ## DW_AT_producer
	.byte	14                      ## DW_FORM_strp
	.byte	19                      ## DW_AT_language
	.byte	5                       ## DW_FORM_data2
	.byte	3                       ## DW_AT_name
	.byte	14                      ## DW_FORM_strp
	.byte	16                      ## DW_AT_stmt_list
	.byte	23                      ## DW_FORM_sec_offset
	.byte	27                      ## DW_AT_comp_dir
	.byte	14                      ## DW_FORM_strp
	.ascii	"\341\177"              ## DW_AT_APPLE_optimized
	.byte	25                      ## DW_FORM_flag_present
	.byte	17                      ## DW_AT_low_pc
	.byte	1                       ## DW_FORM_addr
	.byte	18                      ## DW_AT_high_pc
	.byte	6                       ## DW_FORM_data4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	2                       ## Abbreviation Code
	.byte	46                      ## DW_TAG_subprogram
	.byte	1                       ## DW_CHILDREN_yes
	.byte	17                      ## DW_AT_low_pc
	.byte	1                       ## DW_FORM_addr
	.byte	18                      ## DW_AT_high_pc
	.byte	6                       ## DW_FORM_data4
	.byte	64                      ## DW_AT_frame_base
	.byte	24                      ## DW_FORM_exprloc
	.byte	122                     ## DW_AT_call_all_calls
	.byte	25                      ## DW_FORM_flag_present
	.byte	49                      ## DW_AT_abstract_origin
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	3                       ## Abbreviation Code
	.byte	5                       ## DW_TAG_formal_parameter
	.byte	0                       ## DW_CHILDREN_no
	.byte	2                       ## DW_AT_location
	.byte	23                      ## DW_FORM_sec_offset
	.byte	49                      ## DW_AT_abstract_origin
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	4                       ## Abbreviation Code
	.byte	52                      ## DW_TAG_variable
	.byte	0                       ## DW_CHILDREN_no
	.byte	2                       ## DW_AT_location
	.byte	23                      ## DW_FORM_sec_offset
	.byte	49                      ## DW_AT_abstract_origin
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	5                       ## Abbreviation Code
	.byte	46                      ## DW_TAG_subprogram
	.byte	1                       ## DW_CHILDREN_yes
	.byte	3                       ## DW_AT_name
	.byte	14                      ## DW_FORM_strp
	.byte	58                      ## DW_AT_decl_file
	.byte	11                      ## DW_FORM_data1
	.byte	59                      ## DW_AT_decl_line
	.byte	11                      ## DW_FORM_data1
	.byte	39                      ## DW_AT_prototyped
	.byte	25                      ## DW_FORM_flag_present
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	63                      ## DW_AT_external
	.byte	25                      ## DW_FORM_flag_present
	.ascii	"\341\177"              ## DW_AT_APPLE_optimized
	.byte	25                      ## DW_FORM_flag_present
	.byte	32                      ## DW_AT_inline
	.byte	11                      ## DW_FORM_data1
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	6                       ## Abbreviation Code
	.byte	5                       ## DW_TAG_formal_parameter
	.byte	0                       ## DW_CHILDREN_no
	.byte	3                       ## DW_AT_name
	.byte	14                      ## DW_FORM_strp
	.byte	58                      ## DW_AT_decl_file
	.byte	11                      ## DW_FORM_data1
	.byte	59                      ## DW_AT_decl_line
	.byte	11                      ## DW_FORM_data1
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	7                       ## Abbreviation Code
	.byte	52                      ## DW_TAG_variable
	.byte	0                       ## DW_CHILDREN_no
	.byte	3                       ## DW_AT_name
	.byte	14                      ## DW_FORM_strp
	.byte	58                      ## DW_AT_decl_file
	.byte	11                      ## DW_FORM_data1
	.byte	59                      ## DW_AT_decl_line
	.byte	11                      ## DW_FORM_data1
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	8                       ## Abbreviation Code
	.byte	36                      ## DW_TAG_base_type
	.byte	0                       ## DW_CHILDREN_no
	.byte	3                       ## DW_AT_name
	.byte	14                      ## DW_FORM_strp
	.byte	62                      ## DW_AT_encoding
	.byte	11                      ## DW_FORM_data1
	.byte	11                      ## DW_AT_byte_size
	.byte	11                      ## DW_FORM_data1
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	9                       ## Abbreviation Code
	.byte	55                      ## DW_TAG_restrict_type
	.byte	0                       ## DW_CHILDREN_no
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	10                      ## Abbreviation Code
	.byte	15                      ## DW_TAG_pointer_type
	.byte	0                       ## DW_CHILDREN_no
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	11                      ## Abbreviation Code
	.byte	22                      ## DW_TAG_typedef
	.byte	0                       ## DW_CHILDREN_no
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	3                       ## DW_AT_name
	.byte	14                      ## DW_FORM_strp
	.byte	58                      ## DW_AT_decl_file
	.byte	11                      ## DW_FORM_data1
	.byte	59                      ## DW_AT_decl_line
	.byte	11                      ## DW_FORM_data1
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	12                      ## Abbreviation Code
	.byte	46                      ## DW_TAG_subprogram
	.byte	1                       ## DW_CHILDREN_yes
	.byte	17                      ## DW_AT_low_pc
	.byte	1                       ## DW_FORM_addr
	.byte	18                      ## DW_AT_high_pc
	.byte	6                       ## DW_FORM_data4
	.byte	64                      ## DW_AT_frame_base
	.byte	24                      ## DW_FORM_exprloc
	.byte	122                     ## DW_AT_call_all_calls
	.byte	25                      ## DW_FORM_flag_present
	.byte	3                       ## DW_AT_name
	.byte	14                      ## DW_FORM_strp
	.byte	58                      ## DW_AT_decl_file
	.byte	11                      ## DW_FORM_data1
	.byte	59                      ## DW_AT_decl_line
	.byte	11                      ## DW_FORM_data1
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	63                      ## DW_AT_external
	.byte	25                      ## DW_FORM_flag_present
	.ascii	"\341\177"              ## DW_AT_APPLE_optimized
	.byte	25                      ## DW_FORM_flag_present
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	13                      ## Abbreviation Code
	.byte	52                      ## DW_TAG_variable
	.byte	0                       ## DW_CHILDREN_no
	.byte	2                       ## DW_AT_location
	.byte	24                      ## DW_FORM_exprloc
	.byte	3                       ## DW_AT_name
	.byte	14                      ## DW_FORM_strp
	.byte	58                      ## DW_AT_decl_file
	.byte	11                      ## DW_FORM_data1
	.byte	59                      ## DW_AT_decl_line
	.byte	11                      ## DW_FORM_data1
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	14                      ## Abbreviation Code
	.byte	11                      ## DW_TAG_lexical_block
	.byte	1                       ## DW_CHILDREN_yes
	.byte	17                      ## DW_AT_low_pc
	.byte	1                       ## DW_FORM_addr
	.byte	18                      ## DW_AT_high_pc
	.byte	6                       ## DW_FORM_data4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	15                      ## Abbreviation Code
	.byte	52                      ## DW_TAG_variable
	.byte	0                       ## DW_CHILDREN_no
	.byte	2                       ## DW_AT_location
	.byte	23                      ## DW_FORM_sec_offset
	.byte	3                       ## DW_AT_name
	.byte	14                      ## DW_FORM_strp
	.byte	58                      ## DW_AT_decl_file
	.byte	11                      ## DW_FORM_data1
	.byte	59                      ## DW_AT_decl_line
	.byte	11                      ## DW_FORM_data1
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	16                      ## Abbreviation Code
	.byte	29                      ## DW_TAG_inlined_subroutine
	.byte	1                       ## DW_CHILDREN_yes
	.byte	49                      ## DW_AT_abstract_origin
	.byte	19                      ## DW_FORM_ref4
	.byte	17                      ## DW_AT_low_pc
	.byte	1                       ## DW_FORM_addr
	.byte	18                      ## DW_AT_high_pc
	.byte	6                       ## DW_FORM_data4
	.byte	88                      ## DW_AT_call_file
	.byte	11                      ## DW_FORM_data1
	.byte	89                      ## DW_AT_call_line
	.byte	11                      ## DW_FORM_data1
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	17                      ## Abbreviation Code
	.byte	52                      ## DW_TAG_variable
	.byte	0                       ## DW_CHILDREN_no
	.byte	49                      ## DW_AT_abstract_origin
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	18                      ## Abbreviation Code
	.byte	1                       ## DW_TAG_array_type
	.byte	1                       ## DW_CHILDREN_yes
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	19                      ## Abbreviation Code
	.byte	33                      ## DW_TAG_subrange_type
	.byte	0                       ## DW_CHILDREN_no
	.byte	73                      ## DW_AT_type
	.byte	19                      ## DW_FORM_ref4
	.byte	55                      ## DW_AT_count
	.byte	6                       ## DW_FORM_data4
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	20                      ## Abbreviation Code
	.byte	36                      ## DW_TAG_base_type
	.byte	0                       ## DW_CHILDREN_no
	.byte	3                       ## DW_AT_name
	.byte	14                      ## DW_FORM_strp
	.byte	11                      ## DW_AT_byte_size
	.byte	11                      ## DW_FORM_data1
	.byte	62                      ## DW_AT_encoding
	.byte	11                      ## DW_FORM_data1
	.byte	0                       ## EOM(1)
	.byte	0                       ## EOM(2)
	.byte	0                       ## EOM(3)
	.section	__DWARF,__debug_info,regular,debug
Lsection_info:
Lcu_begin0:
	.long	334                     ## Length of Unit
	.short	4                       ## DWARF version number
.set Lset10, Lsection_abbrev-Lsection_abbrev ## Offset Into Abbrev. Section
	.long	Lset10
	.byte	8                       ## Address Size (in bytes)
	.byte	1                       ## Abbrev [1] 0xb:0x147 DW_TAG_compile_unit
	.long	0                       ## DW_AT_producer
	.short	12                      ## DW_AT_language
	.long	46                      ## DW_AT_name
.set Lset11, Lline_table_start0-Lsection_line ## DW_AT_stmt_list
	.long	Lset11
	.long	57                      ## DW_AT_comp_dir
                                        ## DW_AT_APPLE_optimized
	.quad	Lfunc_begin0            ## DW_AT_low_pc
.set Lset12, Lfunc_end1-Lfunc_begin0    ## DW_AT_high_pc
	.long	Lset12
	.byte	2                       ## Abbrev [2] 0x2a:0x38 DW_TAG_subprogram
	.quad	Lfunc_begin0            ## DW_AT_low_pc
.set Lset13, Lfunc_end0-Lfunc_begin0    ## DW_AT_high_pc
	.long	Lset13
	.byte	1                       ## DW_AT_frame_base
	.byte	86
                                        ## DW_AT_call_all_calls
	.long	98                      ## DW_AT_abstract_origin
	.byte	3                       ## Abbrev [3] 0x3d:0x9 DW_TAG_formal_parameter
.set Lset14, Ldebug_loc0-Lsection_debug_loc ## DW_AT_location
	.long	Lset14
	.long	110                     ## DW_AT_abstract_origin
	.byte	4                       ## Abbrev [4] 0x46:0x9 DW_TAG_variable
.set Lset15, Ldebug_loc1-Lsection_debug_loc ## DW_AT_location
	.long	Lset15
	.long	143                     ## DW_AT_abstract_origin
	.byte	4                       ## Abbrev [4] 0x4f:0x9 DW_TAG_variable
.set Lset16, Ldebug_loc2-Lsection_debug_loc ## DW_AT_location
	.long	Lset16
	.long	132                     ## DW_AT_abstract_origin
	.byte	4                       ## Abbrev [4] 0x58:0x9 DW_TAG_variable
.set Lset17, Ldebug_loc3-Lsection_debug_loc ## DW_AT_location
	.long	Lset17
	.long	121                     ## DW_AT_abstract_origin
	.byte	0                       ## End Of Children Mark
	.byte	5                       ## Abbrev [5] 0x62:0x39 DW_TAG_subprogram
	.long	234                     ## DW_AT_name
	.byte	1                       ## DW_AT_decl_file
	.byte	10                      ## DW_AT_decl_line
                                        ## DW_AT_prototyped
	.long	155                     ## DW_AT_type
                                        ## DW_AT_external
                                        ## DW_AT_APPLE_optimized
	.byte	1                       ## DW_AT_inline
	.byte	6                       ## Abbrev [6] 0x6e:0xb DW_TAG_formal_parameter
	.long	246                     ## DW_AT_name
	.byte	1                       ## DW_AT_decl_file
	.byte	10                      ## DW_AT_decl_line
	.long	162                     ## DW_AT_type
	.byte	7                       ## Abbrev [7] 0x79:0xb DW_TAG_variable
	.long	248                     ## DW_AT_name
	.byte	1                       ## DW_AT_decl_file
	.byte	15                      ## DW_AT_decl_line
	.long	155                     ## DW_AT_type
	.byte	7                       ## Abbrev [7] 0x84:0xb DW_TAG_variable
	.long	250                     ## DW_AT_name
	.byte	1                       ## DW_AT_decl_file
	.byte	11                      ## DW_AT_decl_line
	.long	172                     ## DW_AT_type
	.byte	7                       ## Abbrev [7] 0x8f:0xb DW_TAG_variable
	.long	293                     ## DW_AT_name
	.byte	1                       ## DW_AT_decl_file
	.byte	13                      ## DW_AT_decl_line
	.long	167                     ## DW_AT_type
	.byte	0                       ## End Of Children Mark
	.byte	8                       ## Abbrev [8] 0x9b:0x7 DW_TAG_base_type
	.long	239                     ## DW_AT_name
	.byte	4                       ## DW_AT_encoding
	.byte	8                       ## DW_AT_byte_size
	.byte	9                       ## Abbrev [9] 0xa2:0x5 DW_TAG_restrict_type
	.long	167                     ## DW_AT_type
	.byte	10                      ## Abbrev [10] 0xa7:0x5 DW_TAG_pointer_type
	.long	155                     ## DW_AT_type
	.byte	11                      ## Abbrev [11] 0xac:0xb DW_TAG_typedef
	.long	183                     ## DW_AT_type
	.long	252                     ## DW_AT_name
	.byte	3                       ## DW_AT_decl_file
	.byte	31                      ## DW_AT_decl_line
	.byte	11                      ## Abbrev [11] 0xb7:0xb DW_TAG_typedef
	.long	194                     ## DW_AT_type
	.long	259                     ## DW_AT_name
	.byte	2                       ## DW_AT_decl_file
	.byte	92                      ## DW_AT_decl_line
	.byte	8                       ## Abbrev [8] 0xc2:0x7 DW_TAG_base_type
	.long	275                     ## DW_AT_name
	.byte	7                       ## DW_AT_encoding
	.byte	8                       ## DW_AT_byte_size
	.byte	12                      ## Abbrev [12] 0xc9:0x6b DW_TAG_subprogram
	.quad	Lfunc_begin1            ## DW_AT_low_pc
.set Lset18, Lfunc_end1-Lfunc_begin1    ## DW_AT_high_pc
	.long	Lset18
	.byte	1                       ## DW_AT_frame_base
	.byte	86
                                        ## DW_AT_call_all_calls
	.long	295                     ## DW_AT_name
	.byte	1                       ## DW_AT_decl_file
	.byte	23                      ## DW_AT_decl_line
	.long	308                     ## DW_AT_type
                                        ## DW_AT_external
                                        ## DW_AT_APPLE_optimized
	.byte	13                      ## Abbrev [13] 0xe2:0x10 DW_TAG_variable
	.byte	4                       ## DW_AT_location
	.byte	145
	.ascii	"\360\377_"
	.long	246                     ## DW_AT_name
	.byte	1                       ## DW_AT_decl_file
	.byte	24                      ## DW_AT_decl_line
	.long	315                     ## DW_AT_type
	.byte	7                       ## Abbrev [7] 0xf2:0xb DW_TAG_variable
	.long	324                     ## DW_AT_name
	.byte	1                       ## DW_AT_decl_file
	.byte	28                      ## DW_AT_decl_line
	.long	155                     ## DW_AT_type
	.byte	14                      ## Abbrev [14] 0xfd:0x1d DW_TAG_lexical_block
	.quad	Ltmp8                   ## DW_AT_low_pc
.set Lset19, Ltmp12-Ltmp8               ## DW_AT_high_pc
	.long	Lset19
	.byte	15                      ## Abbrev [15] 0x10a:0xf DW_TAG_variable
.set Lset20, Ldebug_loc4-Lsection_debug_loc ## DW_AT_location
	.long	Lset20
	.long	250                     ## DW_AT_name
	.byte	1                       ## DW_AT_decl_file
	.byte	25                      ## DW_AT_decl_line
	.long	308                     ## DW_AT_type
	.byte	0                       ## End Of Children Mark
	.byte	16                      ## Abbrev [16] 0x11a:0x19 DW_TAG_inlined_subroutine
	.long	98                      ## DW_AT_abstract_origin
	.quad	Ltmp13                  ## DW_AT_low_pc
.set Lset21, Ltmp16-Ltmp13              ## DW_AT_high_pc
	.long	Lset21
	.byte	1                       ## DW_AT_call_file
	.byte	28                      ## DW_AT_call_line
	.byte	17                      ## Abbrev [17] 0x12d:0x5 DW_TAG_variable
	.long	121                     ## DW_AT_abstract_origin
	.byte	0                       ## End Of Children Mark
	.byte	0                       ## End Of Children Mark
	.byte	8                       ## Abbrev [8] 0x134:0x7 DW_TAG_base_type
	.long	300                     ## DW_AT_name
	.byte	5                       ## DW_AT_encoding
	.byte	4                       ## DW_AT_byte_size
	.byte	18                      ## Abbrev [18] 0x13b:0xf DW_TAG_array_type
	.long	155                     ## DW_AT_type
	.byte	19                      ## Abbrev [19] 0x140:0x9 DW_TAG_subrange_type
	.long	330                     ## DW_AT_type
	.long	65536                   ## DW_AT_count
	.byte	0                       ## End Of Children Mark
	.byte	20                      ## Abbrev [20] 0x14a:0x7 DW_TAG_base_type
	.long	304                     ## DW_AT_name
	.byte	8                       ## DW_AT_byte_size
	.byte	7                       ## DW_AT_encoding
	.byte	0                       ## End Of Children Mark
	.section	__DWARF,__debug_macinfo,regular,debug
Ldebug_macinfo:
	.byte	0                       ## End Of Macro List Mark
	.section	__DWARF,__apple_names,regular,debug
Lnames_begin:
	.long	1212240712              ## Header Magic
	.short	1                       ## Header Version
	.short	0                       ## Header Hash Function
	.long	2                       ## Header Bucket Count
	.long	2                       ## Header Hash Count
	.long	12                      ## Header Data Length
	.long	0                       ## HeaderData Die Offset Base
	.long	1                       ## HeaderData Atom Count
	.short	1                       ## DW_ATOM_die_offset
	.short	6                       ## DW_FORM_data4
	.long	0                       ## Bucket 0
	.long	1                       ## Bucket 1
	.long	2090499946              ## Hash in Bucket 0
	.long	2090756197              ## Hash in Bucket 1
.set Lset22, LNames1-Lnames_begin       ## Offset in Bucket 0
	.long	Lset22
.set Lset23, LNames0-Lnames_begin       ## Offset in Bucket 1
	.long	Lset23
LNames1:
	.long	295                     ## main
	.long	1                       ## Num DIEs
	.long	201
	.long	0
LNames0:
	.long	234                     ## test
	.long	2                       ## Num DIEs
	.long	42
	.long	282
	.long	0
	.section	__DWARF,__apple_objc,regular,debug
Lobjc_begin:
	.long	1212240712              ## Header Magic
	.short	1                       ## Header Version
	.short	0                       ## Header Hash Function
	.long	1                       ## Header Bucket Count
	.long	0                       ## Header Hash Count
	.long	12                      ## Header Data Length
	.long	0                       ## HeaderData Die Offset Base
	.long	1                       ## HeaderData Atom Count
	.short	1                       ## DW_ATOM_die_offset
	.short	6                       ## DW_FORM_data4
	.long	-1                      ## Bucket 0
	.section	__DWARF,__apple_namespac,regular,debug
Lnamespac_begin:
	.long	1212240712              ## Header Magic
	.short	1                       ## Header Version
	.short	0                       ## Header Hash Function
	.long	1                       ## Header Bucket Count
	.long	0                       ## Header Hash Count
	.long	12                      ## Header Data Length
	.long	0                       ## HeaderData Die Offset Base
	.long	1                       ## HeaderData Atom Count
	.short	1                       ## DW_ATOM_die_offset
	.short	6                       ## DW_FORM_data4
	.long	-1                      ## Bucket 0
	.section	__DWARF,__apple_types,regular,debug
Ltypes_begin:
	.long	1212240712              ## Header Magic
	.short	1                       ## Header Version
	.short	0                       ## Header Hash Function
	.long	6                       ## Header Bucket Count
	.long	6                       ## Header Hash Count
	.long	20                      ## Header Data Length
	.long	0                       ## HeaderData Die Offset Base
	.long	3                       ## HeaderData Atom Count
	.short	1                       ## DW_ATOM_die_offset
	.short	6                       ## DW_FORM_data4
	.short	3                       ## DW_ATOM_die_tag
	.short	5                       ## DW_FORM_data2
	.short	4                       ## DW_ATOM_type_flags
	.short	11                      ## DW_FORM_data1
	.long	-1                      ## Bucket 0
	.long	0                       ## Bucket 1
	.long	1                       ## Bucket 2
	.long	2                       ## Bucket 3
	.long	3                       ## Bucket 4
	.long	4                       ## Bucket 5
	.long	-282664779              ## Hash in Bucket 1
	.long	193495088               ## Hash in Bucket 2
	.long	466678419               ## Hash in Bucket 3
	.long	-113419488              ## Hash in Bucket 4
	.long	-594775205              ## Hash in Bucket 5
	.long	-80380739               ## Hash in Bucket 5
.set Lset24, Ltypes0-Ltypes_begin       ## Offset in Bucket 1
	.long	Lset24
.set Lset25, Ltypes4-Ltypes_begin       ## Offset in Bucket 2
	.long	Lset25
.set Lset26, Ltypes5-Ltypes_begin       ## Offset in Bucket 3
	.long	Lset26
.set Lset27, Ltypes3-Ltypes_begin       ## Offset in Bucket 4
	.long	Lset27
.set Lset28, Ltypes1-Ltypes_begin       ## Offset in Bucket 5
	.long	Lset28
.set Lset29, Ltypes2-Ltypes_begin       ## Offset in Bucket 5
	.long	Lset29
Ltypes0:
	.long	259                     ## __darwin_size_t
	.long	1                       ## Num DIEs
	.long	183
	.short	22
	.byte	0
	.long	0
Ltypes4:
	.long	300                     ## int
	.long	1                       ## Num DIEs
	.long	308
	.short	36
	.byte	0
	.long	0
Ltypes5:
	.long	252                     ## size_t
	.long	1                       ## Num DIEs
	.long	172
	.short	22
	.byte	0
	.long	0
Ltypes3:
	.long	239                     ## double
	.long	1                       ## Num DIEs
	.long	155
	.short	36
	.byte	0
	.long	0
Ltypes1:
	.long	304                     ## __ARRAY_SIZE_TYPE__
	.long	1                       ## Num DIEs
	.long	330
	.short	36
	.byte	0
	.long	0
Ltypes2:
	.long	275                     ## long unsigned int
	.long	1                       ## Num DIEs
	.long	194
	.short	36
	.byte	0
	.long	0

.subsections_via_symbols
	.section	__DWARF,__debug_line,regular,debug
Lsection_line:
Lline_table_start0:
