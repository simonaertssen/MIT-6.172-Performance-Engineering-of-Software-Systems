# Theory of Performance Engineering
This week, we will be taking a look at some of the theory behind our approaches to parallelise programs. It is important to set the GitHub theme to 'light' as there is no support yet for math equations in the 'dark' mode. Solutions to the book 'Introduction To Algorithms' can be found [here](https://github.com/Kelvinson/CLRS-1).

As a quick recap, here are some of the important definitions:

    T_P = execution time on P processors.
    T_1 = execution time on 1 processor (total amount of work in the program).
    T_f = execution time on an infinite amount of processors. Also called span / critical-path length / computational depth.

When we speak of *execution time*, we also mean the *work* of the program.

The **Work Law** states that the work per processor is always at least the total work divided by the number of processors:
<img src="https://render.githubusercontent.com/render/math?math=T_p \geq \frac{T_1}{P}">

The **Span Law** states that the work per processor is always at least the work on an infinite number of processors:
<img src="https://render.githubusercontent.com/render/math?math=T_p \geq T_{\infty}">

The **speedup** of a program is how much faster the same program runs on more than one processor:
<img src="https://render.githubusercontent.com/render/math?math=\frac{T_1}{T_P}">

The **parallelism** of a program is the maximum speedup we can get in the program:
<img src="https://render.githubusercontent.com/render/math?math=\frac{T_1}{T_{\infty}}">
It is also seen as the average amount of work per step in the program.

## Greedy Scheduling

### Write-up 1: Solve Exercise 27.1-3 on Page 791 in Chapter 27 of CLRS.
*Prove that a greedy scheduler achieves the following time bound, which is slightly stronger than the bound proven in Theorem 27.1:*
<img src="https://render.githubusercontent.com/render/math?math=T_p \leq \frac{T_1 - T_{\infty}}{P} %2B T_{\infty}">

In theorem 27.1 (page 782) the following is proven: <img src="https://render.githubusercontent.com/render/math?math=T_{P} \leq \frac{T_{1}}{P} %2B T_{\infty}">. The proof considers the ampount of complete and incomplete steps, where respectively, all processors are active, or some are not, due to the number of available tasks in parallel.

In this proof, the workload performed by all processors in all complete steps is `T_1`. However, the workload is actually `T_1 - T_f`, if we assume we still need to incorporate `T_f` in the total workload of incomplete steps. Hence, the proof follows.

From another point of view: suppose that there are x incomplete steps in a run of the program. Since each of these steps causes at least one unit of work to be done, we have that there is at most (`T1`− x) units of work done in the complete steps. Then, we suppose by contradiction that the number of complete steps is strictly greater than 
<img src="https://render.githubusercontent.com/render/math?math=\left\lfloor\left(T_{1}-x\right) / P\right\rfloor">

Then, we have that the total amount of work done during the complete steps is
<img src="https://render.githubusercontent.com/render/math?math=P \cdot\left(\left\lfloor\left(T_{1}-x\right) / P\right\rfloor %2B 1\right)=P\left\lfloor\left(T_{1}-x\right) / P \right\rfloor %2B P=\left(T_{1}-x\right)-\left(\left(T_{1}-x\right) \mod P\right) %2B P > T_{1}-x">.

This is a contradiction because there are only (`T1` − x) units of work done during the complete steps, which is less than the amount we would be doing. Notice that since `T_f`​ is a bound on the total number of both kinds of steps, it is a bound on the number of incomplete steps, x, so,
<img src="https://render.githubusercontent.com/render/math?math=T_{P} \leq\left\lfloor\left(T_{1}-x\right) / P\right\rfloor %2B x \leq\left\lfloor\left(T_{1}-T_{\infty}\right) / P\right\rfloor %2B T_{\infty}">

The second inequality comes by noting that the middle expression, as a function of x is monotonically increasing, and so is bounded by the largest value of x that is possible, namely `T_f`.


### Write-up 2: 
We need to compute some bounds on the different values of `T`. `T_1` is smaller than `T_P * P` (work law), so from `T_4` we get that `T_1` = at least 4*100 = 400 and from `T_64` we get that `T_1` = at least 64 * 10 = 640. It is clear that 400 is the lowest bound on `T_1`. 

For `T_f`, we only know that it is smaller or equal to 10. The lowest possible value of parallelism is then 400 / 10 = 40. The highest possible value of parallelism is 640/10 = 64.


### Write-up 3:
Using the work law, we can compute the respective `T_1` as at least 80*4 = 320, and `T_f` as maximum 9. Then for `P` = 10, we have a contradiction that 42 is less or equal to (320 - 9)/10 + 9 = 40.


### Write-up 4: Solve Exercise 27.1-5 on Page 791 in Chapter 27 of CLRS.
By the work law for `P` = 4, we have that `T_1` is at least 320. By the span law for `P` = 64, we have that `T_f` is at most 10. Now we will use the inequality from Exercise 27.1-3 to derive a contradiction. For `P` = 10, we have that 42 is less or eaqual to (320 - `T_f`)/10 + `T_f`. Equivalently, this states that `T_f` is greater or equal to 10/9*10, which contradicts that `T_f` is at most 10.


## Multithreaded Matrix Multiplication