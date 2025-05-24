#define SIMULATE_FREESTANDING

#include <conc/concepts.hpp>
#include <conc/concurrency.hpp>

#include <catch2/catch_test_macros.hpp>

#include <atomic>
#include <concepts>
#include <cstdint>
#include <utility>

namespace {
auto test_before_definition() {
    conc::call_in_critical_section([] {});
}

struct custom_policy {
    static inline std::atomic<std::uint64_t> count{};

    template <typename = void, std::invocable F, std::predicate... Pred>
        requires(sizeof...(Pred) < 2)
    static inline auto call_in_critical_section(F &&f, auto &&...pred)
        -> decltype(auto) {
        while (true) {
            ++count;
            if ((... and pred())) {
                return std::forward<F>(f)();
            }
        }
    }
};
} // namespace

template <> inline auto conc::injected_policy<> = custom_policy{};

TEST_CASE("standard policy models concept", "[freestanding_injected_policy]") {
    STATIC_REQUIRE(conc::policy<conc::detail::standard_policy<>>);
}

TEST_CASE("custom policy models concept", "[freestanding_injected_policy]") {
    STATIC_REQUIRE(conc::policy<custom_policy>);
}

TEST_CASE("injected custom policy is used", "[freestanding_injected_policy]") {
    custom_policy::count = 0;
    conc::call_in_critical_section([] {});
    CHECK(custom_policy::count == 1);
    conc::call_in_critical_section([] {}, [] { return true; });
    CHECK(custom_policy::count == 2);
}

TEST_CASE("injected custom policy is used before definition",
          "[freestanding_injected_policy]") {
    custom_policy::count = 0;
    test_before_definition();
    CHECK(custom_policy::count == 1);
}

TEST_CASE("predicate is used", "[freestanding_injected_policy]") {
    auto predicate_used = false;
    conc::call_in_critical_section([] {},
                                   [&] {
                                       predicate_used = true;
                                       return true;
                                   });
    CHECK(predicate_used);
}
