#include <conc/concurrency.hpp>

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <iterator>
#include <random>
#include <thread>

TEST_CASE("standard policy allows 'recursive' critical_sections",
          "[standard_policy]") {
    auto const value = conc::call_in_critical_section(
        [] { return conc::call_in_critical_section([] { return 1; }); });
    CHECK(value == 1);
}

namespace {
struct rng_CS;
struct count_CS;

auto get_rng() -> auto & {
    std::array<int, std::mt19937::state_size> seed_data;
    std::random_device r;
    std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    static std::mt19937 rng(seq);
    return rng;
}
} // namespace

TEST_CASE("standard policy works", "[standard_policy]") {
    constexpr auto N = 10u;
    auto &rng = get_rng();
    auto count = 0;
    auto dis = std::uniform_int_distribution{1, 10};

    std::array<std::thread, N> threads{};
    for (auto i = 0u; i < N; ++i) {
        threads[i] = std::thread{[&] {
            auto const d = conc::call_in_critical_section(
                [&] { return std::chrono::milliseconds{dis(rng)}; });
            std::this_thread::sleep_for(d);
            conc::call_in_critical_section([&] { ++count; });
        }};
    }
    for (auto i = 0u; i < N; ++i) {
        threads[i].join();
    }

    CHECK(count == N);
}

TEST_CASE("standard policy allows different-ID critical_sections",
          "[standard_policy]") {
    auto &rng = get_rng();
    auto count = 0;
    auto dis = std::uniform_int_distribution{1, 10};

    auto t1 = std::thread{[&] {
        auto const d = conc::call_in_critical_section<rng_CS>(
            [&] { return std::chrono::milliseconds{dis(rng)}; });
        std::this_thread::sleep_for(d);
        conc::call_in_critical_section<count_CS>([&] { ++count; });
    }};
    auto t2 = std::thread{[&] {
        auto const d = conc::call_in_critical_section<rng_CS>(
            [&] { return std::chrono::milliseconds{dis(rng)}; });
        std::this_thread::sleep_for(d);
        conc::call_in_critical_section<count_CS>([&] { ++count; });
    }};
    t1.join();
    t2.join();

    CHECK(count == 2);
}
