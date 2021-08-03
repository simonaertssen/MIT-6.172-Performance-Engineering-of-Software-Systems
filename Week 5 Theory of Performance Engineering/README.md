# Theory of Performance Engineering
This week, we will be taking a look at some of the theory behind our approaches to parallelise programs. It is important to set the GitHub theme to 'light' as there is no support yet for math equations in the 'dark' mode.

As a quick recap, here are some of the important definitions:

    T_P = execution time on P processors.
    T_1 = execution time on 1 processor (total amount of work in the program).
    T_f = execution time on an infinite amount of processors. Also called span / critical-path length / computational depth.

When we speak of *execution time*, we also mean the *work* of the program.

The **Work Law** states that the work per processor is always at least the total work divided by the number of processors:
<img src="https://render.githubusercontent.com/render/math?math=T_p \geq \frac{T_1}{P}">

The **Span Law** states that the work per processor is always at least the work on an infinite number of processors:
<img src="https://render.githubusercontent.com/render/math?math=T_p \geq T_{\infty}">

## Greedy Scheduling

### Write-up 1: Solve Exercise 27.1-3 on Page 791 in Chapter 27 of CLRS.
*Prove that a greedy scheduler achieves the following time bound, which is slightly stronger than the bound proven in Theorem 27.1:*
<img src="https://render.githubusercontent.com/render/math?math=T_p \leq \frac{T_1 - T_{\infty}}{P} %2B T_{\infty}">

In theorem 27.1 (page 782) the following is proven: <img src="https://render.githubusercontent.com/render/math?math=T_{P} \leq T_{1} / P  %2B T_{\infty}">


