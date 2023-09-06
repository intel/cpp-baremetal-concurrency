cmake_minimum_required(VERSION 3.21)

project(
    baremetal-concurrency
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION
        "A header-only C++ library that provides low-level concurrency hooks.")

include(cmake/get_cpm.cmake)
cpmaddpackage("gh:intel/cicd-repo-infrastructure#main")

add_versioned_package("gh:intel/cpp-std-extensions#dc6352e")

add_library(concurrency INTERFACE)
target_compile_features(concurrency INTERFACE cxx_std_20)
target_include_directories(concurrency INTERFACE include/)
target_link_libraries_system(concurrency INTERFACE stdx)

if(PROJECT_IS_TOP_LEVEL)
    add_docs(docs)
    clang_tidy_interface(concurrency)
    add_subdirectory(test)
endif()
