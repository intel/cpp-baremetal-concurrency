#pragma once

#include <cstdint>
#include <type_traits>

template <> struct atomic::atomic_type<bool> {
    using type = std::uint32_t;
};

template <>
constexpr inline auto atomic::alignment_of<std::uint8_t> = std::size_t{4};
