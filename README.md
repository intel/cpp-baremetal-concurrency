# C++ Bare Metal Concurrency

[![Unit Tests](https://github.com/intel/cpp-baremetal-concurrency/actions/workflows/unit_tests.yml/badge.svg)](https://github.com/intel/cpp-baremetal-concurrency/actions/workflows/unit_tests.yml)

*C++ Bare Metal Concurrency* is a small C++ header-only library that abstracts
the idea of concurrency so that code that runs using standard C++ concurrency
constructs can also run using concurrency mechanisms provided by a bare metal
microcontroller.

A careful reading of
\[[intro.multithread](https://eel.is/c++draft/intro.multithread)\] is
recommended to understand how the C++ memory model defines well-formed
concurrent execution.

C++20 is required to use the `conc` namespace functionality; C++17 will suffice
for the `atomic` namespace functionality. The following compilers are supported:
 
- clang 14 through 19
- gcc 12 through 13

See the [full documentation](https://intel.github.io/cpp-baremetal-concurrency/).
