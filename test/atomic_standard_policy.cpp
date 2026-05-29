#include <conc/atomic.hpp>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <thread>
#include <type_traits>

#if __cplusplus >= 202002L
TEST_CASE("standard policy models concepts", "[atomic_standard_policy]") {
    STATIC_REQUIRE(atomic::load_store_policy<atomic::detail::standard_policy>);
    STATIC_REQUIRE(atomic::exchange_policy<atomic::detail::standard_policy>);
    STATIC_REQUIRE(atomic::add_sub_policy<atomic::detail::standard_policy>);
    STATIC_REQUIRE(atomic::bitwise_policy<atomic::detail::standard_policy>);
    STATIC_REQUIRE(atomic::policy<atomic::detail::standard_policy>);
}
#endif

TEST_CASE("standard policy implements load", "[atomic_standard_policy]") {
    std::uint32_t val{17};
    CHECK(atomic::load(val) == 17);
}

TEST_CASE("standard policy implements store", "[atomic_standard_policy]") {
    std::uint32_t val{17};
    CHECK(atomic::load(val) == 17);
    atomic::store(val, 1337);
    CHECK(val == 1337);
}

TEST_CASE("standard policy implements load and store atomically",
          "[atomic_standard_policy]") {
    std::uint32_t val{17};
    std::uint32_t t1_value{};
    auto t1 = std::thread([&] { t1_value += atomic::load(val); });
    auto t2 = std::thread([&] {
        auto x = atomic::load(val);
        atomic::store(val, ++x);
    });
    t1.join();
    t2.join();
    CHECK((t1_value == 17 or t1_value == 18));
    CHECK(val == 18);
}

TEST_CASE("standard policy implements exchange", "[atomic_standard_policy]") {
    std::uint32_t val{17};
    CHECK(atomic::exchange(val, 1337) == 17);
    CHECK(val == 1337);
}

TEST_CASE("standard policy implements exchange atomically",
          "[atomic_standard_policy]") {
    std::uint32_t val{17};
    std::uint32_t t1_value{};
    std::uint32_t t2_value{};
    auto t1 = std::thread([&] { t1_value += atomic::load(val); });
    auto t2 = std::thread([&] { t2_value += atomic::exchange(val, 1337); });
    t1.join();
    t2.join();
    CHECK((t1_value == 17 or t1_value == 1337));
    CHECK(t2_value == 17);
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
    std::uint32_t t1_value{};
    std::uint32_t t2_value{};
    auto t1 = std::thread([&] { t1_value += atomic::load(val); });
    auto t2 = std::thread([&] { t2_value += atomic::fetch_add(val, 1); });
    t1.join();
    t2.join();
    CHECK((t1_value == 17 or t1_value == 18));
    CHECK(t2_value == 17);
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
    std::uint32_t t1_value{};
    std::uint32_t t2_value{};
    auto t1 = std::thread([&] { t1_value += atomic::load(val); });
    auto t2 = std::thread([&] { t2_value += atomic::fetch_sub(val, 1); });
    t1.join();
    t2.join();
    CHECK((t1_value == 17 or t1_value == 16));
    CHECK(t2_value == 17);
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
    std::uint32_t t1_value{};
    std::uint32_t t2_value{};
    auto t1 = std::thread([&] { t1_value += atomic::load(val); });
    auto t2 = std::thread([&] { t2_value += atomic::fetch_and(val, 0b10); });
    t1.join();
    t2.join();
    CHECK((t1_value == 0b101 or t1_value == 0));
    CHECK(t2_value == 0b101);
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
    std::uint32_t t1_value{};
    std::uint32_t t2_value{};
    auto t1 = std::thread([&] { t1_value += atomic::load(val); });
    auto t2 = std::thread([&] { t2_value += atomic::fetch_or(val, 0b10); });
    t1.join();
    t2.join();
    CHECK((t1_value == 0b111 or t1_value == 0b101));
    CHECK(t2_value == 0b101);
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
    std::uint32_t t1_value{};
    std::uint32_t t2_value{};
    auto t1 = std::thread([&] { t1_value += atomic::load(val); });
    auto t2 = std::thread([&] { t2_value += atomic::fetch_xor(val, 0b1); });
    t1.join();
    t2.join();
    CHECK((t1_value == 0b100 or t1_value == 0b101));
    CHECK(t2_value == 0b101);
    CHECK(val == 0b100);
}

TEMPLATE_TEST_CASE("standard policy has normal types",
                   "[atomic_standard_policy]", bool, std::uint8_t,
                   std::uint16_t, std::uint32_t, std::uint64_t) {
    STATIC_REQUIRE(std::is_same_v<atomic::atomic_type_t<TestType>, TestType>);
}

TEMPLATE_TEST_CASE("standard policy has normal alignment",
                   "[atomic_standard_policy]", bool, std::uint8_t,
                   std::uint16_t, std::uint32_t, std::uint64_t) {
    STATIC_REQUIRE(atomic::alignment_of<TestType> == alignof(TestType));
}
