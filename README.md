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

The library provides for low-level control over two primitive concurrency
mechanisms: atomic operations and critical sections.

See the [full documentation](https://intel.github.io/cpp-baremetal-concurrency/).

C++ standard support is as follows:

- C++23: [main branch](https://github.com/intel/cpp-baremetal-concurrency/tree/main) (active development)
- C++20: [cpp20 branch](https://github.com/intel/cpp-baremetal-concurrency/tree/main) (supported)
- C++17: unsupported, but the atomic operations part of the library from the [cpp20 branch](https://github.com/intel/cpp-baremetal-concurrency/tree/cpp20) will work with C++17 also.

Compiler support:

| Branch | GCC versions | Clang versions |
| --- | --- | --- |
| [main](https://github.com/intel/cpp-baremetal-concurrency/tree/main) | 12 thru 14 | 18 thru 21 |
| [cpp20](https://github.com/intel/cpp-baremetal-concurrency/tree/cpp20) | 12 thru 14 | 14 thru 21 |
