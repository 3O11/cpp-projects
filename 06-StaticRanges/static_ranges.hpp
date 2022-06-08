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
// -> convenience         - Done
// -> copy                -
// -> for_each            - Done
// -> iota_view           -
// -> static_iota_view    -
// -> std                 - Done
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
    // std
    //////////////////////////////////////////////////////////////////////////

    template<typename T, typename U>
    struct range_traits<std::pair<T, U>> : std::integral_constant<std::size_t, 2>
    {
        template<std::size_t I>
        static auto&& get(std::pair<T, U>& r)
        {
            if constexpr (I == 0) return r.first;
            if constexpr (I == 1) return r.second;
        }

        template<std::size_t I>
        static auto&& get(std::pair<T, U>&& r)
        {
            if constexpr (I == 0) return std::move(r.first);
            if constexpr (I == 1) return std::move(r.second);
        }

        template<std::size_t I>
        static auto&& get(const std::pair<T, U>& r)
        {
            if constexpr (I == 0) return r.first;
            if constexpr (I == 1) return r.second;
        }
    };

    template<typename T, std::size_t N>
    struct range_traits<std::array<T, N>> : std::integral_constant<std::size_t, N>
    {
        template<std::size_t I>
        static auto&& get(std::array<T, N>& r)
        {
            if (I < N) return r[I];
        }

        template<std::size_t I>
        static auto&& get(std::array<T, N>&& r)
        {
            if (I < N) return std::move(r[I]);
        }

        template<std::size_t I>
        static auto&& get(const std::array<T, N>& r)
        {
            if (I < N) return r[I];
        }
    };

    template<typename ... Pack>
    struct range_traits<std::tuple<Pack ...>> : std::integral_constant<std::size_t, sizeof...(Pack)>
    {
        template<std::size_t I>
        static auto&& get(std::tuple<Pack ...>& r)
        {
            return std::get<I>(r);
        }

        template<std::size_t I>
        static auto&& get(std::tuple<Pack ...>&& r)
        {
            return std::move(std::get<I>(r));
        }

        template<std::size_t I>
        static auto&& get(const std::tuple<Pack ...>& r)
        {
            return std::get<I>(r);
        }
    };

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

    template <std::size_t I, range R>
    constexpr auto&& element(R&& r)
    {
        // There should be no need for any other special cases, because constness
        // should be preserved by default. I'm not sure if this can be done in a
        // though, it seems to me that it's not possible to prevent the decay of
        // the rvalue reference to lvalue reference.
        if constexpr (std::is_rvalue_reference_v<decltype(r)>)
        {
            return range_traits<std::remove_cvref_t<decltype(r)>>::template get<I>(std::move(r));
        }
        else
        {
            return range_traits<std::remove_cvref_t<decltype(r)>>::template get<I>(r);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // for_each
    //////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template<std::size_t I, std::size_t Size>
        constexpr void for_each_impl(range auto&& r, auto f)
        {
            if constexpr (I < Size)
            {
                f(element<I>(r));
                for_each_impl<I + 1, Size>(r, f);
            }
        }
    }

    constexpr void for_each(range auto&& r, auto f)
    {
        detail::for_each_impl<0, size_v<std::remove_cvref_t<decltype(r)>>>(r, f);
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