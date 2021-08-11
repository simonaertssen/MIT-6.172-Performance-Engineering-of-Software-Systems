# Project 2 Writeup:

## 0. Performance improvements:
The most important result in this writeup is the performance we obtain with every new algorithm. 

Here, the performance and speedup is given per adjustment.

### Serial performance improvements:

| Description | Execution time (s) | Speedup (relative) | Speedup (absolute) |
| - | - | - | - |
| SnailSpeed's implementation (naive) | | | |
| Quadtree | | | |
| Quadtree with maximum depth | | | |
| Precompute line lengths | | | |
| Update Quadtree without destroying it | | | |
| More efficient way of checking intersections | | | |

### Parallel performance improvements:

| Description | Execution time (s) | Speedup (relative) | Speedup (absolute) |
| - | - | - | - |
| SnailSpeed's implementation (naive) | | | |
| | | | |

--- 

## 1. Testing the number of collisions:
As to verify that our improved algorithms are correct, we need to know how many collisions were detected for every animation. These measurements 

| Animation | Line-Wall Collisions | Line-Line Collisions |