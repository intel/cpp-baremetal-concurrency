#include <conc/concepts.hpp>

#include <catch2/catch_test_macros.hpp>

namespace {
struct good_policy {
    template <typename F>
    [[nodiscard]] constexpr static auto call_in_critical_section(F &&f)
        -> decltype(auto) {
        return std::forward<F>(f)();
    }

    template <typename F>
    [[nodiscard]] constexpr static auto call_in_critical_section(F &&f, auto &&)
        -> decltype(auto) {
        return std::forward<F>(f)();
    }
};

struct bad_policy {
    static auto call_in_critical_section(auto &&f) { return f(); }
    static auto call_in_critical_section(auto &&f, auto &&) { return f(); }
};

struct not_a_policy {};
} // namespace

TEST_CASE("concurrency_policy", "[concepts]") {
    static_assert(conc::concurrency_policy<good_policy>);
    static_assert(not conc::concurrency_policy<not_a_policy>);
    static_assert(not conc::concurrency_policy<bad_policy>);
}
