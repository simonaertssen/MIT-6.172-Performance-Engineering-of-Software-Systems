# ANSWERS
## RECITATION
## WRITEUP 2
Use 32-bit aligned registers.


## WRITEUP 3
Seems like the ternary operator does not compare arrays element-wise.


## WRITEUP 4
The memory is not aligned. It should be possible to either reallign the 
b array by moving everything one place further or by making it possible
to align with an offset. This should definitely be faster.


## WRITEUP 5
The ffast-math flag was already set from a prior experiment! The addsd command 
is replaced by a addpd command.

clang -O3 example4.c -o example4; ./example4
The decimal floating point sum result is: 11.667578
The raw floating point sum result is: 0x1.755cccec10aa5p+3

clang -O3 -ffast-math  example4.c -o example4; ./example4
The decimal floating point sum result is: 11.667578
The raw floating point sum result is: 0x1.755cccec10aa3p+3


HOMEWORK
## WRITEUP 6
Without vectorisation:
Elapsed execution time: 0.051179 sec; N: 1024, I: 100000, __OP__: +, __TYPE__: uint32_t
With vectorisation:
Elapsed execution time: 0.014415 sec; N: 1024, I: 100000, __OP__: +, __TYPE__: uint32_t
With AVX instructions:
Elapsed execution time: 0.015936 sec; N: 1024, I: 100000, __OP__: +, __TYPE__: uint32_t

The last two timings are not that different, the mean just fluctuates a lot! We need 100 
times more data points!

Without vectorisation:
Elapsed execution time: 5.143872 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
With vectorisation:
Elapsed execution time: 1.236273 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
With AVX instructions:
Elapsed execution time: 1.240279 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t

The speedup gained by vectorisation is about 4 times, for both vectorisations. This makes
sense, as the SSE vector registers are usually 128 bits wide, so 4 x 32 = 128. The AVX 
registers on the MacBook are apparently also 4 bits wide. When using the DTU cluster, 
we obtain the following.

Without vectorisation:
Elapsed execution time: 1.627987 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
With vectorisation:
Elapsed execution time: 1.623288 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
With AVX instructions:
Elapsed execution time: 0.955319 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t

On the DTU cluster we cannot seem to disable vectorisation, but here the AVX2 vectorisation 
clearly yields an advantage, with a speedup of about 2. This is because the AVX registers
on the cluster are 256 bits wide, instead of 128bits.


## WRITEUP 7
Without vectorisation there is a lot of pushing 'long' values around. With the initial 
vectorisation present we can see that the speedup is gained by the 'movdqa' command, 
which is fast because it uses an aligned move. With AVX instructions enabled, we see the 
'vmovdqu' command. Ideally we would need 'vmovdqa' but that is for the future.


## WRITEUP 8
There seems to be a command for the most used operators
__OP__ + yields vpaddd
__OP__ - yields vpsubd
__OP__ * yields vpmulld
__OP__ / yields unvectorised code
__OP__ % yields unvectorised code
__OP__ << yields vpsllvd
__OP__ >> yields vpsrlvd
Using bit operators like && also seems to yield vectorised code but it's a little more complex,
as a mask has to be made.

With AVX2, the vectorised code is mostly here:
vmovdqu	-4128(%rbp,%rax,4), %ymm0
vpsllvd	-8224(%rbp,%rax,4), %ymm0, %ymm0
vmovdqu	%ymm0, -12320(%rbp,%rax,4)

Without, we get something much more horrible:
movdqa	-8224(%rbp,%rax,4), %xmm1
pslld	$23, %xmm1
paddd	%xmm0, %xmm1
cvttps2dq	%xmm1, %xmm1
pmulld	-4128(%rbp,%rax,4), %xmm1
movdqa	%xmm1, -12320(%rbp,%rax,4)

Seems like it does vectorise but having a dedicated command also seems handy. The digits get shifted
left by a constant value, then the precision gets increased and a multiplication happens... Shit...
Only some these look like vector instructions.


## WRITEUP 9
Baseline (unvectorised):
Elapsed execution time: 5.355811 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint64_t
Elapsed execution time: 4.876741 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
Elapsed execution time: 4.880880 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint16_t
Elapsed execution time: 4.871564 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint8_t
It seems like there is almost no performance increase when decreasing data type width.

Vectorised (VECTORIZE=1):
Elapsed execution time: 2.426008 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint64_t
Elapsed execution time: 1.311766 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
Elapsed execution time: 0.604613 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint16_t
Elapsed execution time: 0.003570 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint8_t
This yields a speed-up of about 2, 4, 8 and 1360 respectively. Insane!

Vectorised (VECTORIZE=1 AVX2=1):
Elapsed execution time: 2.525990 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint64_t
Elapsed execution time: 1.232248 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
Elapsed execution time: 0.003610 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint16_t
Elapsed execution time: 0.003279 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint8_t
Funny enough the speedup is about 2, 4, 1360 and 1360 respectively. The AVX registers on this
laptop surely must be only 128 bits wide.


## WRITEUP 10
For the baseline we obtain:
Elapsed execution time: 5.102470 sec; N: 1024, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 5.054718 sec; N: 1024, I: 10000000, __OP__: *, __TYPE__: uint8_t

For the vectorised code we obtain:
Elapsed execution time: 5.159584 sec; N: 1024, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 0.003583 sec; N: 1024, I: 10000000, __OP__: *, __TYPE__: uint8_t

This yields a speedup of 1 and 1400 respectively.


## WRITEUP 12
Baseline (unvectorised):
Elapsed execution time: 14.567039 sec; N: 2048, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 4.832512 sec; N: 1024, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 2.384933 sec; N: 512, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 1.274390 sec; N: 256, I: 10000000, __OP__: *, __TYPE__: uint64_t

Vectorised (VECTORIZE=1):
Elapsed execution time: 13.288087 sec; N: 2048, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 4.916160 sec; N: 1024, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 2.480489 sec; N: 512, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 1.267368 sec; N: 256, I: 10000000, __OP__: *, __TYPE__: uint64_t

Vectorised (VECTORIZE=1 AVX2=1):
Elapsed execution time: 10.420156 sec; N: 2048, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 5.146025 sec; N: 1024, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 2.498619 sec; N: 512, I: 10000000, __OP__: *, __TYPE__: uint64_t
Elapsed execution time: 1.284762 sec; N: 256, I: 10000000, __OP__: *, __TYPE__: uint64_t

We can see that speedups are better when we are using large arrays.


## WRITEUP 13
The code is never vectorised when a stride is implemented. 


## WRITEUP 14
Baseline (with a stride of 2:
Elapsed execution time: 2.584729 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
Vectorised (VECTORIZE=1):
Elapsed execution time: 2.436141 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
Vectorised (VECTORIZE=1 AVX2=1):
Elapsed execution time: 2.570534 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t

So not a single block was vectorised! We can use ##pragma clang loop vectorize_width(x)
and that does seem to produce vectorised code:
vmovdqa	-8224(%rbp,%rax,8), %xmm0
vpaddd	-4128(%rbp,%rax,8), %xmm0, %xmm0
vmovd	%xmm0, -12320(%rbp,%rax,8)
vpextrd	$2, %xmm0, -12312(%rbp,%rax,8)

Vectorised (VECTORIZE=1):
Elapsed execution time: 1.688359 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
Vectorised (VECTORIZE=1 AVX2=1):
Elapsed execution time: 1.659592 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t

So now it actually is vectorised and the speedup is about 2!

What is the result of using 'interleave'?

Baseline:
Elapsed execution time: 1.588413 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
Vectorised (VECTORIZE=1):
Elapsed execution time: 1.742076 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
Vectorised (VECTORIZE=1 AVX2=1):
Elapsed execution time: 1.642037 sec; N: 1024, I: 10000000, __OP__: +, __TYPE__: uint32_t
No speedup with this functionality :p


## WRITEUP 15
It seems like the vector is shuffled with 'pshufd' and then two add commands follow:
.loc	4 72 15                 #### loop.c:72:15
pshufd	$78, %xmm1, %xmm0       #### xmm0 = xmm1[2,3,0,1]
paddd	%xmm1, %xmm0
phaddd	%xmm0, %xmm0
movd	%xmm0, %r15d
So it is vectorised!

