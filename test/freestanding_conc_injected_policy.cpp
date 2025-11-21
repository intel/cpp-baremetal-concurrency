#define CONC_FREESTANDING

#include <conc/concepts.hpp>
#include <conc/concurrency.hpp>

#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <cstdint>
#include <utility>

namespace {
auto test_before_definition() {
    CHECK(conc::call_in_critical_section([] { return 17; }) == 17);
}

struct custom_policy {
    static inline std::uint64_t count{};

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
    auto c = custom_policy::count;
    CHECK(conc::call_in_critical_section([] { return 17; }) == 17);
    CHECK(custom_policy::count - c == 1);
    CHECK(conc::call_in_critical_section([] { return 17; }) == 17);
    CHECK(custom_policy::count - c == 2);
}

TEST_CASE("injected custom policy is used before definition",
          "[freestanding_injected_policy]") {
    auto c = custom_policy::count;
    test_before_definition();
    CHECK(custom_policy::count - c == 1);
}

TEST_CASE("predicate is used", "[freestanding_injected_policy]") {
    auto predicate_used = 0;
    auto v = conc::call_in_critical_section([] { return 17; },
                                            [&] {
                                                ++predicate_used;
                                                return true;
                                            });
    CHECK(v == 17);
    CHECK(predicate_used == 1);
}
