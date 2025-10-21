#pragma once

#include <conc/concurrency.hpp>

#if __STDC_HOSTED__ == 0
#error conc::test_policy is designed for desktop testing and requires a hosted implementation
#endif

#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <mutex>
#include <random>
#include <thread>
#include <utility>

namespace conc {
class test_policy {
    template <typename> static inline std::mutex m{};

    [[maybe_unused]] static auto get_rng() -> auto & {
        thread_local auto rng = [] {
            std::array<int, std::mt19937::state_size> seed_data;
            std::random_device r;
            std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
            std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
            return std::mt19937{seq};
        }();
        return rng;
    }

    template <typename Uniq> struct [[nodiscard]] cs_raii_t {
        cs_raii_t() {
            m<Uniq>.lock();
            ++lock_count;
        }
        ~cs_raii_t() {
            ++unlock_count;
            m<Uniq>.unlock();
        }
    };

  public:
    static inline std::atomic<std::uint32_t> lock_count{};
    static inline std::atomic<std::uint32_t> unlock_count{};

    static auto reset_counts() {
        lock_count = 0;
        unlock_count = 0;
    }

    template <typename Uniq = void, std::invocable F, std::predicate... Pred>
        requires(sizeof...(Pred) < 2)
    static inline auto call_in_critical_section(F &&f, Pred &&...pred)
        -> decltype(std::forward<F>(f)()) {
        while (true) {
            std::uniform_int_distribution<> dis{5, 10};
            auto const d1 = std::chrono::milliseconds{dis(get_rng())};
            auto const d2 = std::chrono::milliseconds{dis(get_rng())};
            std::this_thread::sleep_for(d1);

            [[maybe_unused]] cs_raii_t<Uniq> lock{};
            std::this_thread::sleep_for(d2);
            if ((... and pred())) {
                return std::forward<F>(f)();
            }
        }
    }
};

template <> inline auto injected_policy<> = test_policy{};
} // namespace conc
