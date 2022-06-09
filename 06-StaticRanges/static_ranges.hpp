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
// -> all_view            - Done
// -> concepts            -
// -> convenience         - Done
// -> copy                - Done
// -> for_each            - Done
// -> iota_view           - Done (I'm not sure what's different, my local diff shows no difference)
// -> static_iota_view    - Done
// -> std                 - Done
// -> to                  - Done
// -> transform_view      -
// -> transform           - Done

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
            if constexpr (I < N) return r[I];
        }

        template<std::size_t I>
        static auto&& get(std::array<T, N>&& r)
        {
            if constexpr (I < N) return std::move(r[I]);
        }

        template<std::size_t I>
        static auto&& get(const std::array<T, N>& r)
        {
            if constexpr (I < N) return r[I];
        }
    };

    template<typename ... Pack>
    struct range_traits<std::tuple<Pack ...>> : std::integral_constant<std::size_t, sizeof...(Pack)>
    {
        template<std::size_t I>
        static auto&& get(std::tuple<Pack ...>& r)
        {
            if constexpr (I < sizeof...(Pack)) return std::get<I>(r);
        }

        template<std::size_t I>
        static auto&& get(std::tuple<Pack ...>&& r)
        {
            if constexpr (I < sizeof...(Pack)) return std::move(std::get<I>(r));
        }

        template<std::size_t I>
        static auto&& get(const std::tuple<Pack ...>& r)
        {
            if constexpr (I < sizeof...(Pack)) return std::get<I>(r);
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
    inline constexpr auto&& element(R&& r)
    {
        auto&& e = range_traits<std::remove_cvref_t<R>>::template get<I>(std::forward<R>(r));
        return std::forward<decltype(e)>(e);
    }

    //////////////////////////////////////////////////////////////////////////
    // for_each
    //////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template<std::size_t I, std::size_t Size, range R, typename Func>
        constexpr void for_each_impl(R&& r, Func f)
        {
            if constexpr (I < Size)
            {
                f(element<I>(r));
                for_each_impl<I + 1, Size>(r, f);
            }
        }
    }

    template<range R, typename Func>
    inline constexpr void for_each(R&& r, Func f)
    {
        detail::for_each_impl<0, size_v<std::remove_cvref_t<R>>>(r, f);
    }

    //////////////////////////////////////////////////////////////////////////
    // copy
    //////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template<std::size_t I, std::size_t Size, range R1, range R2>
        inline constexpr void copy_impl(R1&& r1, R2&& r2)
        {
            if constexpr (I < Size)
            {
                static_assert(std::is_assignable_v<decltype(element<I>(r2)), decltype(element<I>(r1))>);
                element<I>(r2) = element<I>(r1);
                copy_impl<I + 1, Size, R1, R2>(r1, r2);
            }
        }
    }

    template<range R1, range R2>
    inline constexpr void copy(R1&& r1, R2&& r2)
    {
        static_assert(size_v<R1> == size_v<R2>);

        detail::copy_impl<0, size_v<R1>, R1, R2>(std::forward<R1>(r1), std::forward<R2>(r2));
    }

    //////////////////////////////////////////////////////////////////////////
    // to
    //////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template <std::size_t I, std::size_t Size, range R, typename... Types>
        inline constexpr auto to_tuple_impl(R&& r, std::tuple<Types...> tuple)
        {
            if constexpr (I < Size)
            {
                return to_tuple_impl<I + 1, Size>(r, std::tuple_cat(tuple, std::tuple{element<I>(r)}));
            }
            else
            {
                return tuple;
            }
        }
    }

    template<range R>
    inline constexpr auto to_tuple(R&& r)
    {
        return detail::to_tuple_impl<0, size_v<R>, R>(r, {});
    }

    template<range R>
    inline constexpr auto to_pair(R&& r)
    {
        static_assert(size_v<R> == 2);

        return std::pair(element<0>(r), element<1>(r));
    }

    template<typename T, range R>
    inline constexpr auto to_array(R&& r)
    {
        std::array<T, size_v<R>> out;
        copy(r, out);
        return out;
    }

    namespace views
    {
        //////////////////////////////////////////////////////////////////////
        // all_view
        //////////////////////////////////////////////////////////////////////

        template<range R>
        struct all_view : view_base
        {
            constexpr all_view(R& r)
                : ref(r)
            {}

            operator R& ()
            {
                return ref;
            }

            // std::reference_wrapper was thankfully not necessary here.
            // I suspect that GCC has some problems with infering the types
            // because using std::reference_wrapper<std::remove_cvref_t<R>>
            // worked for me under Clang 13.1.6, but not under GCC 11.2.
            R& ref;
        };

        template<typename RV>
        auto all(RV& rv)
        {
            if constexpr (view<RV>)
            {
                return rv;
            }
            else
            {
                return all_view<RV>(rv);
            }
        };

        //////////////////////////////////////////////////////////////////////
        // iota_view
        //////////////////////////////////////////////////////////////////////

        template<typename T, std::size_t N>
        struct iota_view : view_base
        {};

        template<typename T, std::size_t N>
        auto iota()
        {
            static_assert(std::is_integral_v<T>);
            return iota_view<T, N>();
        }

        //////////////////////////////////////////////////////////////////////
        // static_iota_view
        //////////////////////////////////////////////////////////////////////

        template<typename T, std::size_t N>
        struct static_iota_view : view_base
        {};

        template<typename T, std::size_t N>
        auto static_iota()
        {
            static_assert(std::is_integral_v<T>);
            return static_iota_view<T, N>();
        }

        //////////////////////////////////////////////////////////////////////
        // transform_view
        //////////////////////////////////////////////////////////////////////

        template<range R, typename Func>
        struct transform_view : view_base
        {
            constexpr transform_view(R& r, Func f)
                : ref(r), f(f)
            {}

            R& ref;
            Func f;
        };

        template<range R, typename Func>
        inline constexpr views::transform_view<R, Func> transform(R&& r, Func f)
        {
            return views::transform_view<R, Func>(r, f);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // transform
    //////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template<std::size_t I, std::size_t Size, range R1, range R2, typename Func>
        inline constexpr void transform_impl(R1&& r1, R2&& r2, Func f)
        {
            if constexpr (I < Size)
            {
                element<I>(r2) = f(element<I>(r1));
                transform_impl<I + 1, Size>(std::forward<R1>(r1), std::forward<R2>(r2), f);
            }
        }

        template<std::size_t I, std::size_t Size, range R1, range R2, range R3, typename Func>
        inline constexpr void transform_impl(R1&& r1, R2&& r2, R3&& r3, Func f)
        {
            if constexpr (I < Size)
            {
                element<I>(r3) = f(element<I>(r1), element<I>(r2));
                transform_impl<I + 1, Size>(std::forward<R1>(r1), std::forward<R2>(r2), std::forward<R3>(r3), f);
            }
        }
    }

    template<range R1, range R2, typename Func>
    inline constexpr void transform(R1&& r1, R2&& r2, Func f)
    {
        static_assert(size_v<R1> == size_v<R2>);

        detail::transform_impl<0, size_v<R1>>(std::forward<R1>(r1), std::forward<R2>(r2), f);
    }

    template<range R1, range R2, range R3, typename Func>
    inline constexpr void transform(R1&& r1, R2&& r2, R3&& r3, Func f)
    {
        static_assert(size_v<R1> == size_v<R2> && size_v<R3> == size_v<R1>);

        detail::transform_impl<0, size_v<R1>>(std::forward<R1>(r1), std::forward<R2>(r2), std::forward<R3>(r3), f);
    }

    //////////////////////////////////////////////////////////////////////////
    // view traits (needed for proper range functionality)
    //////////////////////////////////////////////////////////////////////////

    template<range R>
    struct range_traits<views::all_view<R>> : std::integral_constant<std::size_t, size_v<R>>
    {
        template<std::size_t I>
        static auto&& get(views::all_view<R>& v)
        {
            if constexpr (I < size_v<R>) return range_traits<std::remove_cvref_t<R>>::template get<I>(v.ref);
        }
    };

    template<typename T, std::size_t N>
    struct range_traits<views::iota_view<T, N>> : std::integral_constant<std::size_t, N>
    {
        template<std::size_t I>
        static auto get(views::iota_view<T, N>)
        {
            if constexpr (I < N) return T(I);
        }
    };

    template<typename T, std::size_t N>
    struct range_traits<views::static_iota_view<T, N>> : std::integral_constant<std::size_t, N>
    {
        template<std::size_t I>
        static auto get(views::static_iota_view<T, N>)
        {
            if constexpr (I < N) return std::integral_constant<T, I>();
        }
    };

    template<range R, typename Func>
    struct range_traits<views::transform_view<R, Func>> : std::integral_constant<std::size_t, size_v<R>>
    {
        template<std::size_t I>
        static auto get(views::transform_view<R, Func> v)
        {
            if constexpr (I < size_v<R>) return v.f(range_traits<std::remove_cvref_t<R>>::template get<I>(v.ref));
        }
    };
}

namespace static_views = static_ranges::views;

#endif //_STATIC_RANGES_H