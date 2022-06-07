#ifndef static_for_test_struct_hpp_
#define static_for_test_struct_hpp_

#include "static_ranges.hpp"

#include <cstddef>
#include <string>
#include <utility>

struct range_three {
	int x;
	double y;
	std::string z;
};

template<>
struct static_ranges::range_traits<range_three> : std::integral_constant<std::size_t, 3> {
	template<std::size_t I>
	static auto&& get(range_three& r)
	{
		if constexpr (I == 0) { return r.x; }
		if constexpr (I == 1) { return r.y; }
		if constexpr (I == 2) { return r.z; }
	}

	template<std::size_t I>
	static auto&& get(range_three&& r)
	{
		if constexpr (I == 0) { return std::move(r.x); }
		if constexpr (I == 1) { return std::move(r.y); }
		if constexpr (I == 2) { return std::move(r.z); }
	}

	template<std::size_t I>
	static auto&& get(const range_three& r)
	{
		if constexpr (I == 0) { return r.x; }
		if constexpr (I == 1) { return r.y; }
		if constexpr (I == 2) { return r.z; }
	}
};

struct range_three_2 {
	std::string u, v;
	std::size_t w;
};

template<>
struct static_ranges::range_traits<range_three_2> : std::integral_constant<std::size_t, 3> {
	template<std::size_t I, typename T2>
	static auto&& get(T2&& r)
	{
		if constexpr (I == 0) { return std::forward<T2>(r).u; }
		if constexpr (I == 1) { return std::forward<T2>(r).v; }
		if constexpr (I == 2) { return std::forward<T2>(r).w; }
	}
};

struct range_two {
	int a;
	double b;
};

template<>
struct static_ranges::range_traits<range_two> : std::integral_constant<std::size_t, 2> {
	template<std::size_t I, typename T2>
	static auto&& get(T2&& r)
	{
		if constexpr (I == 0) { return std::forward<T2>(r).a; }
		if constexpr (I == 1) { return std::forward<T2>(r).b; }
	}
};

struct range_ten {
	int arr[10];
};

template<>
struct static_ranges::range_traits<range_ten> : std::integral_constant<std::size_t, 10> {
	template<std::size_t I, typename T2>
	static auto&& get(T2&& r)
	{
		return std::forward<T2>(r).arr[I];
	}
};

#endif

/**/