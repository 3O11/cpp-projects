#include "static_ranges.hpp"

#include <type_traits>
#include <iostream>
#include <iomanip>

auto iota_view = static_ranges::views::static_iota<int, 5>();

static_assert(static_ranges::size_v<decltype(iota_view)> == 5, "size_v<decltype(iota_view)> shall be equal to 5");

using iota_view_0_t = decltype(static_ranges::element<0>(iota_view));
static_assert(std::is_same_v< std::remove_cvref_t<decltype(iota_view_0_t::value)>, int>, "iota_view<0> shall be of type int");

static_assert(static_ranges::element<0>(iota_view).value == 0, "iota_view<0> shall be 0");
static_assert(static_ranges::element<1>(iota_view).value == 1, "iota_view<1> shall be 1");
static_assert(static_ranges::element<2>(iota_view).value == 2, "iota_view<2> shall be 2");
static_assert(static_ranges::element<3>(iota_view).value == 3, "iota_view<3> shall be 3");
static_assert(static_ranges::element<4>(iota_view).value == 4, "iota_view<4> shall be 4");

auto char_view = static_ranges::views::static_iota<char, 127>();

static_assert(static_ranges::size_v<decltype(char_view)> == 127, "size_v<decltype(char_view)> shall be equal to 127");

using char_view_65_t = decltype(static_ranges::element<65>(char_view));
static_assert(std::is_same_v< std::remove_cvref_t<decltype(char_view_65_t::value)>, char>, "char_view<65> shall be of type char");

static_assert(static_ranges::element<65>(char_view).value == 'A', "char_view<65> shall be 'A'");
static_assert(static_ranges::element<66>(char_view).value == 'B', "char_view<66> shall be 'B'");
static_assert(static_ranges::element<67>(char_view).value == 'C', "char_view<67> shall be 'C'");

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