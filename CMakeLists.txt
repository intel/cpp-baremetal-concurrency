cmake_minimum_required(VERSION 3.21)

project(
    baremetal-concurrency
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION
        "A header-only C++ library that provides low-level concurrency hooks.")

include(cmake/get_cpm.cmake)
if(PROJECT_IS_TOP_LEVEL)
    cpmaddpackage("gh:intel/cicd-repo-infrastructure#dev")
else()
    cpmaddpackage("gh:intel/cicd-repo-infrastructure#3e2bef0")
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23)
endif()

add_library(concurrency INTERFACE)
target_compile_features(concurrency INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

target_sources(
    concurrency
    INTERFACE FILE_SET
              concurrency
              TYPE
              HEADERS
              BASE_DIRS
              include
              FILES
              include/conc/atomic.hpp
              include/conc/concepts.hpp
              include/conc/concurrency.hpp
              include/conc/detail/freestanding.hpp)

if(PROJECT_IS_TOP_LEVEL)
    add_docs(docs)
    clang_tidy_interface(concurrency)
    add_subdirectory(test)
endif()
