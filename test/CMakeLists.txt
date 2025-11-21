function(add_tests)
    set(multiValueArgs FILES MULL_EXCLUSIONS)
    cmake_parse_arguments(TEST "" "" "${multiValueArgs}" ${ARGN})

    foreach(name ${TEST_FILES})
        add_unit_test(
            "${name}_test"
            CATCH2
            FILES
            "${name}.cpp"
            LIBRARIES
            warnings
            concurrency
            pthread)
        if(NOT name IN_LIST TEST_MULL_EXCLUSIONS)
            add_mull_test("${name}_test" EXCLUDE_CTEST)
        endif()
    endforeach()
endfunction()

add_tests(
    FILES
    atomic_injected_policy
    atomic_standard_policy
    conc_standard_policy
    conc_test_policy
    concepts
    freestanding_conc_injected_policy
    hosted_conc_injected_policy
    MULL_EXCLUSIONS
    conc_standard_policy
    conc_test_policy)

add_compile_fail_test(fail_no_conc_policy.cpp LIBRARIES concurrency)

target_compile_definitions(
    atomic_injected_policy_test
    PRIVATE -DATOMIC_CFG="${CMAKE_CURRENT_SOURCE_DIR}/atomic_cfg.hpp")
