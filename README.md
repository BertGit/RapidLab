# RapidLab
High-Performance Interval Library using SSE2 instruction sets for fast SIMD operations.  
The library is header file only, so just set up your header search path, #include "interval/core.hpp", and you're ready to go.

### What to use it for
RapidLab is specialized for very fast execution of interval arithmetic. It comes with an optimization algorithm for finding the guaranteed minimum in a constrained search space based on branch-and-bound. By providing 1st and/or 2nd order derivatives, RapidLab can improve pruning of the branch-and-bound algorithm and significantly speed up solution finding.  

See test/optimizer.test.cpp for examples on how to use RapidLab optimizer.

### How to build the test examples
cd test && make clean all && ./interval_test
