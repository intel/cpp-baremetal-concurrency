#define SIMULATE_FREESTANDING

#include <conc/concurrency.hpp>

// EXPECT: No standard concurrency policy defined: inject a policy

auto main() -> int {
    conc::call_in_critical_section([] {});
}
