#ifndef _STATIC_RANGES_H
#define _STATIC_RANGES_H

#include <utility>
#include <tuple>
#include <array>
#include <cstdint>
#include <type_traits>

// Feature list:
// -> Namespaces          - Done
// -> Common declarations - Done
// -> all_view            - 
// -> concepts            -
// -> convenience         -
// -> copy                -
// -> for_each            -
// -> iota_view           -
// -> static_iota_view    -
// -> std                 -
// -> to                  -
// -> transform_view      -
// -> transform           -


namespace static_ranges
{
    template <typename T> struct range_traits;
    struct view_base {};

    //////////////////////////////////////////////////////////////////////////
    // concepts
    //////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template <typename, typename = void>
        static constexpr bool is_complete_v = false;

        template <typename T>
        static constexpr bool is_complete_v<T, std::void_t<decltype(sizeof(T))>> = true;
    }

    template <typename T>
    concept range = requires(T r)
    {
        detail::is_complete_v<range_traits<std::remove_cvref_t<T>>>;
        std::is_same_v<decltype(static_ranges::range_traits<std::remove_cvref_t<T>>::value), std::size_t>;
    };

    template <typename T>
    concept view = range<T> && std::is_base_of_v<view_base, T>;

    //////////////////////////////////////////////////////////////////////////
    // convenience
    //////////////////////////////////////////////////////////////////////////

    template <typename T>
    static constexpr std::size_t size_v = static_cast<std::size_t>(range_traits<std::remove_cvref_t<T>>::value);

    template <typename T, typename U>
    static constexpr std::size_t size_v<std::pair<T, U>> = 2;

    template <typename ... Pack>
    static constexpr std::size_t size_v<std::tuple<Pack ...>> = sizeof...(Pack);

    template <std::size_t N, typename T>
    static constexpr std::size_t size_v<std::array<T, N>> = N;

    template <std::size_t I>
    constexpr auto&& element(range auto&& r)
    {
        if constexpr (std::is_rvalue_reference_v<decltype(r)>)
        {
            return range_traits<std::remove_cvref_t<decltype(r)>>::template get<I>(std::move(r));
        }
        else
        {
            return range_traits<std::remove_cvref_t<decltype(r)>>::template get<I>(r);
        }
    }

    template <std::size_t I, typename U, typename V>
    constexpr auto&& element(std::pair<U, V>&& r)
    {
        static_assert(I >= 0 && I <= 1);
        if constexpr (I == 0)
        {
            return r.first();
        }
        else
        {
            return r.second();
        }
    }

    template <std::size_t I, typename ... Pack>
    constexpr auto&& element(std::tuple<Pack ...> r)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    // for_each
    //////////////////////////////////////////////////////////////////////////

    namespace detail
    {
    }

    constexpr void for_each(range auto&& r, auto f)
    {
    }

    namespace views
    {
        //////////////////////////////////////////////////////////////////////
        // all_view
        //////////////////////////////////////////////////////////////////////

        template<typename T>
        struct all
        {
            
        };

        //////////////////////////////////////////////////////////////////////
        // iota_view
        //////////////////////////////////////////////////////////////////////
    }
}

namespace static_views = static_ranges::views;

#endif //_STATIC_RANGES_H