#include <conc/atomic.hpp>

#include <catch2/catch_test_macros.hpp>

#include <atomic>
#include <cstdint>
#include <type_traits>

namespace {
struct custom_policy {
    template <typename T>
    static auto load(T const &t,
                     std::memory_order = std::memory_order_seq_cst) -> T {
        return t;
    }
    template <typename T>
    static auto store(T &t, T &value,
                      std::memory_order = std::memory_order_seq_cst) -> void {
        t = value;
    }
};
} // namespace

template <> inline auto atomic::injected_policy<> = custom_policy{};

#if __cplusplus >= 202002L
TEST_CASE("injected policy models load_store", "[atomic_injected_policy]") {
    static_assert(atomic::load_store_policy<custom_policy>);
}
#endif

TEST_CASE("injected policy implements load", "[atomic_injected_policy]") {
    std::uint32_t val{17};
    CHECK(atomic::load(val) == 17);
}

TEST_CASE("injected policy implements store", "[atomic_injected_policy]") {
    std::uint32_t val{17};
    atomic::store(val, 1337);
    CHECK(val == 1337);
}

TEST_CASE("injected policy can inject different atomic types",
          "[atomic_injected_policy]") {
    static_assert(std::is_same_v<atomic::atomic_type_t<bool>, std::uint32_t>);
    static_assert(atomic::alignment_of<bool> == alignof(std::uint32_t));
}

TEST_CASE("injected policy can inject different atomic alignments",
          "[atomic_injected_policy]") {
    static_assert(atomic::alignment_of<std::uint8_t> == 4);
}
