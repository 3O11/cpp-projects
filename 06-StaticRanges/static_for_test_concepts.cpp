#include "static_for_test_struct.hpp"

#include <iostream>
#include <iomanip>

struct norange {};

static_assert(!static_ranges::range<norange>, "norange shall not be a range");
static_assert(!static_ranges::view<norange>, "norange shall not be a view");

static_assert(static_ranges::range<range_two>, "range_two shall be a range");
static_assert(static_ranges::range<range_three>, "range_three shall be a range");
static_assert(static_ranges::range<range_three_2>, "range_three_2 shall be a range");
static_assert(static_ranges::range<range_ten>, "range_ten shall be a range");

static_assert(! static_ranges::view<range_two>, "range_two shall not be a view");
static_assert(! static_ranges::view<range_three>, "range_three shall not be a view");
static_assert(! static_ranges::view<range_three_2>, "range_three_2 shall not be a view");
static_assert(! static_ranges::view<range_ten>, "range_ten shall not be a view");

using my_pair = std::pair<int, std::string>;
using my_tuple = std::tuple<int, double, std::string>;
using my_array = std::array<int, 10>;

static_assert(static_ranges::range<my_pair>, "pair shall be a range");
static_assert(static_ranges::range<my_tuple>, "tuple shall be a range");
static_assert(static_ranges::range<my_array>, "array shall be a range");

static_assert(! static_ranges::view<my_pair>, "pair shall not be a view");
static_assert(! static_ranges::view<my_tuple>, "tuple shall not be a view");
static_assert(! static_ranges::view<my_array>, "array shall not be a view");

inline constexpr auto my_iota = static_views::iota<int, 10>();

static_assert(static_ranges::range<decltype(my_iota)>, "iota() shall be a range");
static_assert(static_ranges::view<decltype(my_iota)>, "iota() shall be a view");

inline constexpr auto my_static_iota = static_views::static_iota<int, 10>();

static_assert(static_ranges::range<decltype(my_static_iota)>, "static_iota() shall be a range");
static_assert(static_ranges::view<decltype(my_static_iota)>, "static_iota() shall be a view");

int main(int argc, char** argv)
{
	my_array aa;

	auto my_view0 = static_views::all(aa);
	static_assert(static_ranges::range<decltype(my_view0)>, "my_view0 shall be a range");
	static_assert(static_ranges::view<decltype(my_view0)>, "my_view0 shall be a view");

	auto my_view = static_views::transform(aa, [](auto&& x) { return 2 * x; });
	static_assert(static_ranges::range<decltype(my_view)>, "my_view shall be a range");
	static_assert(static_ranges::view<decltype(my_view)>, "my_view shall be a view");

	auto my_view2 = aa | static_views::transform([](auto&& x) { return 2 * x + 1; });
	static_assert(static_ranges::view<decltype(my_view2)>, "my_view2 shall be a view");

	return 0;
}

/**/