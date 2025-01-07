#include <conc/concepts.hpp>

#include <catch2/catch_test_macros.hpp>

namespace {
struct good_conc_policy {
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

struct bad_conc_policy_return_decay {
    static auto call_in_critical_section(auto &&f) { return f(); }
    static auto call_in_critical_section(auto &&f, auto &&) { return f(); }
};

struct bad_conc_policy_no_pred {
    static auto call_in_critical_section(auto &&f) -> decltype(auto) {
        return f();
    }
};

struct not_a_policy {};
} // namespace

TEST_CASE("concurrency_policy", "[concepts]") {
    static_assert(conc::policy<good_conc_policy>);
    static_assert(not conc::policy<not_a_policy>);
    static_assert(not conc::policy<bad_conc_policy_return_decay>);
    static_assert(not conc::policy<bad_conc_policy_no_pred>);
}

namespace {
struct atomic_load_store_policy {
    template <typename T>
    static auto load(T const &t,
                     std::memory_order mo = std::memory_order_seq_cst) -> T;
    template <typename T>
    static auto store(T &t, T &value,
                      std::memory_order mo = std::memory_order_seq_cst) -> void;
};

struct atomic_exchange_policy : atomic_load_store_policy {
    template <typename T>
    static auto exchange(T &t, T &value,
                         std::memory_order mo = std::memory_order_seq_cst) -> T;
};

struct atomic_add_sub_policy : atomic_load_store_policy {
    template <typename T>
    static auto fetch_add(T &t, T &value,
                          std::memory_order mo = std::memory_order_seq_cst)
        -> T;
    template <typename T>
    static auto fetch_sub(T &t, T &value,
                          std::memory_order mo = std::memory_order_seq_cst)
        -> T;
};

struct atomic_bitwise_policy : atomic_load_store_policy {
    template <typename T>
    static auto fetch_and(T &t, T &value,
                          std::memory_order mo = std::memory_order_seq_cst)
        -> T;
    template <typename T>
    static auto fetch_or(T &t, T &value,
                         std::memory_order mo = std::memory_order_seq_cst) -> T;
    template <typename T>
    static auto fetch_xor(T &t, T &value,
                          std::memory_order mo = std::memory_order_seq_cst)
        -> T;
};

struct atomic_policy : atomic_exchange_policy,
                       atomic_add_sub_policy,
                       atomic_bitwise_policy {};
} // namespace

TEST_CASE("good atomic policies", "[concepts]") {
    static_assert(atomic::load_store_policy<atomic_load_store_policy>);
    static_assert(atomic::exchange_policy<atomic_exchange_policy>);
    static_assert(atomic::add_sub_policy<atomic_add_sub_policy>);
    static_assert(atomic::bitwise_policy<atomic_bitwise_policy>);
    static_assert(atomic::policy<atomic_policy>);
    static_assert(not atomic::policy<not_a_policy>);
}

namespace {
struct bad_load_store_policy_no_load {
    template <typename T>
    static auto store(T &t, T &value,
                      std::memory_order mo = std::memory_order_seq_cst) -> void;
};

struct bad_load_store_policy_no_store {
    template <typename T>
    static auto store(T &t, T &value,
                      std::memory_order mo = std::memory_order_seq_cst) -> void;
};

struct bad_load_store_policy_no_memory_order {
    template <typename T> static auto load(T const &t) -> T;
    template <typename T> static auto store(T &t, T &value) -> void;
};

struct bad_exchange_policy_no_return : atomic_load_store_policy {
    template <typename T>
    static auto exchange(T &t, T &value,
                         std::memory_order mo = std::memory_order_seq_cst)
        -> void;
};
} // namespace

TEST_CASE("bad atomic policies", "[concepts]") {
    static_assert(not atomic::load_store_policy<bad_load_store_policy_no_load>);
    static_assert(
        not atomic::load_store_policy<bad_load_store_policy_no_store>);
    static_assert(
        not atomic::load_store_policy<bad_load_store_policy_no_memory_order>);
    static_assert(not atomic::exchange_policy<bad_exchange_policy_no_return>);
}
