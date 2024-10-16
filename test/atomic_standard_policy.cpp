#include <conc/atomic.hpp>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <thread>
#include <type_traits>

#if __cplusplus >= 202002L
TEST_CASE("standard policy models concepts", "[atomic_standard_policy]") {
    static_assert(atomic::load_store_policy<atomic::detail::standard_policy>);
    static_assert(atomic::exchange_policy<atomic::detail::standard_policy>);
    static_assert(atomic::add_sub_policy<atomic::detail::standard_policy>);
    static_assert(atomic::bitwise_policy<atomic::detail::standard_policy>);
    static_assert(atomic::policy<atomic::detail::standard_policy>);
}
#endif

TEST_CASE("standard policy implements load", "[atomic_standard_policy]") {
    std::uint32_t val{17};
    CHECK(atomic::load(val) == 17);
}

TEST_CASE("standard policy implements store", "[atomic_standard_policy]") {
    std::uint32_t val{17};
    atomic::store(val, 1337);
    CHECK(val == 1337);
}

TEST_CASE("standard policy implements load and store atomically",
          "[atomic_standard_policy]") {
    std::uint32_t val{17};
    auto t1 = std::thread([&] { [[maybe_unused]] auto x = atomic::load(val); });
    auto t2 = std::thread([&] { atomic::store(val, 1337); });
    t1.join();
    t2.join();
    CHECK(val == 1337);
}

TEST_CASE("standard policy implements exchange", "[atomic_standard_policy]") {
    std::uint32_t val{17};
    CHECK(atomic::exchange(val, 1337) == 17);
    CHECK(val == 1337);
}

TEST_CASE("standard policy implements exchange atomically",
          "[atomic_standard_policy]") {
    std::uint32_t val{17};
    auto t1 = std::thread([&] { [[maybe_unused]] auto x = atomic::load(val); });
    auto t2 = std::thread([&] { CHECK(atomic::exchange(val, 1337) == 17); });
    t1.join();
    t2.join();
    CHECK(val == 1337);
}

TEST_CASE("standard policy implements fetch_add", "[atomic_standard_policy]") {
    std::uint32_t val{17};
    CHECK(atomic::fetch_add(val, 1) == 17);
    CHECK(val == 18);
}

TEST_CASE("standard policy implements fetch_add atomically",
          "[atomic_standard_policy]") {
    std::uint32_t val{17};
    auto t1 = std::thread([&] { [[maybe_unused]] auto x = atomic::load(val); });
    auto t2 = std::thread([&] { CHECK(atomic::fetch_add(val, 1) == 17); });
    t1.join();
    t2.join();
    CHECK(val == 18);
}

TEST_CASE("standard policy implements fetch_sub", "[atomic_standard_policy]") {
    std::uint32_t val{17};
    CHECK(atomic::fetch_sub(val, 1) == 17);
    CHECK(val == 16);
}

TEST_CASE("standard policy implements fetch_sub atomically",
          "[atomic_standard_policy]") {
    std::uint32_t val{17};
    auto t1 = std::thread([&] { [[maybe_unused]] auto x = atomic::load(val); });
    auto t2 = std::thread([&] { CHECK(atomic::fetch_sub(val, 1) == 17); });
    t1.join();
    t2.join();
    CHECK(val == 16);
}

TEST_CASE("standard policy implements fetch_and", "[atomic_standard_policy]") {
    std::uint32_t val{0b101};
    CHECK(atomic::fetch_and(val, 0b10) == 0b101);
    CHECK(val == 0);
}

TEST_CASE("standard policy implements fetch_and atomically",
          "[atomic_standard_policy]") {
    std::uint32_t val{0b101};
    auto t1 = std::thread([&] { [[maybe_unused]] auto x = atomic::load(val); });
    auto t2 =
        std::thread([&] { CHECK(atomic::fetch_and(val, 0b10) == 0b101); });
    t1.join();
    t2.join();
    CHECK(val == 0);
}

TEST_CASE("standard policy implements fetch_or", "[atomic_standard_policy]") {
    std::uint32_t val{0b101};
    CHECK(atomic::fetch_or(val, 0b10) == 0b101);
    CHECK(val == 0b111);
}

TEST_CASE("standard policy implements fetch_or atomically",
          "[atomic_standard_policy]") {
    std::uint32_t val{0b101};
    auto t1 = std::thread([&] { [[maybe_unused]] auto x = atomic::load(val); });
    auto t2 = std::thread([&] { CHECK(atomic::fetch_or(val, 0b10) == 0b101); });
    t1.join();
    t2.join();
    CHECK(val == 0b111);
}

TEST_CASE("standard policy implements fetch_xor", "[atomic_standard_policy]") {
    std::uint32_t val{0b101};
    CHECK(atomic::fetch_xor(val, 0b1) == 0b101);
    CHECK(val == 0b100);
}

TEST_CASE("standard policy implements fetch_xor atomically",
          "[atomic_standard_policy]") {
    std::uint32_t val{0b101};
    auto t1 = std::thread([&] { [[maybe_unused]] auto x = atomic::load(val); });
    auto t2 = std::thread([&] { CHECK(atomic::fetch_xor(val, 0b1) == 0b101); });
    t1.join();
    t2.join();
    CHECK(val == 0b100);
}

TEMPLATE_TEST_CASE("standard policy has normal types",
                   "[atomic_standard_policy]", bool, std::uint8_t,
                   std::uint16_t, std::uint32_t, std::uint64_t) {
    static_assert(std::is_same_v<atomic::atomic_type_t<TestType>, TestType>);
}

TEMPLATE_TEST_CASE("standard policy has normal alignment",
                   "[atomic_standard_policy]", bool, std::uint8_t,
                   std::uint16_t, std::uint32_t, std::uint64_t) {
    static_assert(atomic::alignment_of<TestType> == alignof(TestType));
}
