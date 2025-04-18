if(${CMAKE_CXX_STANDARD} GREATER_EQUAL 20)
    add_unit_test(
        concepts_test
        CATCH2
        FILES
        concepts.cpp
        LIBRARIES
        warnings
        concurrency)

    add_unit_test(
        freestanding_conc_injected_policy_test
        CATCH2
        FILES
        freestanding_conc_injected_policy.cpp
        LIBRARIES
        warnings
        concurrency)

    add_unit_test(
        hosted_conc_injected_policy_test
        CATCH2
        FILES
        hosted_conc_injected_policy.cpp
        LIBRARIES
        warnings
        concurrency)

    add_unit_test(
        conc_standard_policy_test
        CATCH2
        FILES
        conc_standard_policy.cpp
        LIBRARIES
        warnings
        concurrency
        pthread)

    add_compile_fail_test(fail_no_conc_policy.cpp LIBRARIES concurrency)
endif()

add_unit_test(
    atomic_standard_policy_test
    CATCH2
    FILES
    atomic_standard_policy.cpp
    LIBRARIES
    warnings
    concurrency)

add_unit_test(
    atomic_injected_policy_test
    CATCH2
    FILES
    atomic_injected_policy.cpp
    LIBRARIES
    warnings
    concurrency)
target_compile_definitions(
    atomic_injected_policy_test
    PRIVATE -DATOMIC_CFG="${CMAKE_CURRENT_SOURCE_DIR}/atomic_cfg.hpp")
