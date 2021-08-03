# Theory of Performance Engineering
This week, we will be taking a look at some of the theory behind our approaches to parallelise programs.

## Greedy Scheduling

### Write-up 1: Solve Exercise 27.1-3 on Page 791 in Chapter 27 of CLRS.
Prove that a greedy scheduler achieves the following time bound, which is slightlystronger than the bound proven in Theorem 27.1:

<img src="https://render.githubusercontent.com/render/math?math=T_p \leq \frac{T_1 - T_{\infty}}{P} + T_{\infty}">