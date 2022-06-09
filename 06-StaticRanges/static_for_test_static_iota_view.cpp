#include "static_ranges.hpp"

#include <type_traits>
#include <iostream>
#include <iomanip>

auto iota_view = static_ranges::views::static_iota<int, 5>();

static_assert(static_ranges::size_v<decltype(iota_view)> == 5, "size_v<decltype(iota_view)> shall be equal to 5");

template<std::size_t I>
constexpr auto iota_view_value = std::remove_cvref_t<decltype(static_ranges::element<I>(iota_view))>::value;

static_assert(std::is_same_v< decltype(iota_view_value<0>), const int>, "iota_view<0> shall be of type int");

static_assert(iota_view_value<0> == 0, "iota_view<0> shall be 0");
static_assert(iota_view_value<1> == 1, "iota_view<1> shall be 1");
static_assert(iota_view_value<2> == 2, "iota_view<2> shall be 2");
static_assert(iota_view_value<3> == 3, "iota_view<3> shall be 3");
static_assert(iota_view_value<4> == 4, "iota_view<4> shall be 4");

auto char_view = static_ranges::views::static_iota<char, 127>();

static_assert(static_ranges::size_v<decltype(char_view)> == 127, "size_v<decltype(char_view)> shall be equal to 127");

template<std::size_t I>
constexpr auto char_view_value = std::remove_cvref_t<decltype(static_ranges::element<I>(char_view))>::value;

static_assert(std::is_same_v< decltype(char_view_value<65>), const char>, "char_view<65> shall be of type char");

static_assert(char_view_value<65> == 'A', "char_view<65> shall be 'A'");
static_assert(char_view_value<66> == 'B', "char_view<66> shall be 'B'");
static_assert(char_view_value<67> == 'C', "char_view<67> shall be 'C'");

int main(int argc, char** argv)
{
	std::cout << "iota_view[0] = " << static_ranges::element<0>(iota_view) << std::endl;
	std::cout << "iota_view[1] = " << static_ranges::element<1>(iota_view) << std::endl;
	std::cout << "iota_view[2] = " << static_ranges::element<2>(iota_view) << std::endl;
	std::cout << "iota_view[3] = " << static_ranges::element<3>(iota_view) << std::endl;
	std::cout << "iota_view[4] = " << static_ranges::element<4>(iota_view) << std::endl;

	std::cout << "char_view[65] = " << static_ranges::element<65>(char_view) << std::endl;
	std::cout << "char_view[66] = " << static_ranges::element<66>(char_view) << std::endl;
	std::cout << "char_view[67] = " << static_ranges::element<67>(char_view) << std::endl;

	std::cout << "Done." << std::endl;

	return 0;
}

/**/