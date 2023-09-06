#pragma once

#include <stdx/concepts.hpp>

namespace conc {
template <typename T>
concept concurrency_policy =
    requires(auto (*f)()->int &&, auto (*pred)()->bool) {
        { T::call_in_critical_section(f) } -> stdx::same_as<int &&>;
        { T::call_in_critical_section(f, pred) } -> stdx::same_as<int &&>;
    };
} // namespace conc
