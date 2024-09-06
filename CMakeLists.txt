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

add_versioned_package("gh:intel/cpp-std-extensions#dc6352e")

add_library(concurrency INTERFACE)
target_compile_features(concurrency INTERFACE cxx_std_20)
target_link_libraries_system(concurrency INTERFACE stdx)

target_sources(
    concurrency
    INTERFACE FILE_SET
              concurrency
              TYPE
              HEADERS
              BASE_DIRS
              include
              FILES
              include/conc/concepts.hpp
              include/conc/concurrency.hpp)

if(PROJECT_IS_TOP_LEVEL)
    add_docs(docs)
    clang_tidy_interface(concurrency)
    add_subdirectory(test)
endif()
