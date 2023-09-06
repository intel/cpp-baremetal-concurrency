#pragma once

#ifdef SIMULATE_FREESTANDING
#define HAS_MUTEX 0
#else
#define HAS_MUTEX __has_include(<mutex>)
#endif

#include <stdx/concepts.hpp>
#include <stdx/type_traits.hpp>

#if HAS_MUTEX
#include <mutex>
#endif
#include <conc/concepts.hpp>

#include <utility>

namespace conc {
namespace detail {
#if HAS_MUTEX
template <typename Mutex = std::mutex> class standard_policy {
    template <typename> static inline Mutex m{};

  public:
    template <typename Uniq = void, stdx::invocable F, stdx::predicate... Pred>
        requires(sizeof...(Pred) < 2)
    static inline auto call_in_critical_section(F &&f, auto &&...pred)
        -> decltype(std::forward<F>(f)()) {
        while (true) {
            [[maybe_unused]] std::lock_guard l{m<Uniq>};
            if ((... and pred())) {
                return std::forward<F>(f)();
            }
        }
    }
};
#else
template <typename = void> struct standard_policy {
    template <typename = void, stdx::invocable F, stdx::predicate... Pred>
        requires(sizeof...(Pred) < 2)
    static inline auto call_in_critical_section(F &&f, Pred &&...)
        -> decltype(std::forward<F>(f)()) {
        static_assert(
            stdx::always_false_v<F, Pred...>,
            "No standard concurrency policy defined: inject a policy");
        return std::forward<F>(f)();
    }
};
#endif
} // namespace detail

template <typename...> inline auto injected_policy = detail::standard_policy{};

template <typename Uniq = decltype([] {}), typename... DummyArgs,
          stdx::invocable F, stdx::predicate... Pred>
    requires(sizeof...(DummyArgs) == 0 and sizeof...(Pred) < 2)
inline auto call_in_critical_section(F &&f, Pred &&...pred)
    -> decltype(std::forward<F>(f)()) {
    concurrency_policy auto &p = injected_policy<DummyArgs...>;
    return p.template call_in_critical_section<Uniq>(
        std::forward<F>(f), std::forward<Pred>(pred)...);
}
} // namespace conc

#undef HAS_MUTEX
