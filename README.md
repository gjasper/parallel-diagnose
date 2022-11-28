This project implements the MergeXplain algorithm proposed in [1] using gecode as the constraint solver.

## Sample Problems

Gecode samples can be found at the samples folder.

In order to compile and run the samples, the path to gecode files must be provided as such:

```
g++ -I<dir>/include -c send-more-money.cpp
g++ -o send-more-money -L<dir>/lib send-more-money.o \
-lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
```

## Dependencies 

Gecode can be installed follow steps in [2].

Some testes may require Catch2, which can be found at [3] or obtained from some package manager.

## References

[1] Y. Hamadi and L. Sais, Eds., Handbook of Parallel Constraint Reasoning. Cham: Springer International Publishing, 2018. doi: 10.1007/978-3-319-63516-3.
    Chapter 14: Parallel Model-Based Diagnosis

[2] https://www.gecode.org/documentation.html

[3] https://github.com/catchorg/Catch2