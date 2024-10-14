#pragma once

#include <concepts>

namespace conc {
template <typename T>
concept concurrency_policy =
    requires(auto (*f)()->int &&, auto (*pred)()->bool) {
        { T::call_in_critical_section(f) } -> std::same_as<int &&>;
        { T::call_in_critical_section(f, pred) } -> std::same_as<int &&>;
    };
} // namespace conc
