add_unit_test(
    concepts_test
    CATCH2
    FILES
    concepts.cpp
    LIBRARIES
    warnings
    concurrency)

add_unit_test(
    freestanding_injected_policy_test
    CATCH2
    FILES
    freestanding_injected_policy.cpp
    LIBRARIES
    warnings
    concurrency)

add_unit_test(
    hosted_injected_policy_test
    CATCH2
    FILES
    hosted_injected_policy.cpp
    LIBRARIES
    warnings
    concurrency)

add_unit_test(
    standard_policy_test
    CATCH2
    FILES
    standard_policy.cpp
    LIBRARIES
    warnings
    concurrency
    pthread)

add_compile_fail_test(fail_no_policy.cpp LIBRARIES concurrency)
