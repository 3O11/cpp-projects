#include "static_ranges.hpp"
#include <functional>
#include <iostream>
#include <iomanip>

using my_pair = std::pair<int, std::string>;
using my_tuple = std::tuple<int, double, std::string>;
using my_array = std::array<int, 10>;

static_assert(static_ranges::size_v<my_pair> == 2, "static_ranges::size_v<my_pair> shall be equal to 2");
static_assert(static_ranges::size_v<my_tuple> == 3, "static_ranges::size_v<my_tuple> shall be equal to 3");
static_assert(static_ranges::size_v<my_array> == 10, "static_ranges::size_v<my_array> shall be equal to 10");

int main(int argc, char** argv)
{
	my_pair p = { 1, "thr" };

	std::cout << "p.first = " << static_ranges::element<0>(p) << std::endl;
	std::cout << "p.second = " << static_ranges::element<1>(p) << std::endl;

	my_tuple t = { 42, 3.14, "Some_quite_large_string_containing_Foo_and_Bar" };

	std::cout << "t<0> = " << static_ranges::element<0>(t) << std::endl;
	std::cout << "t<1> = " << static_ranges::element<1>(t) << std::endl;
	std::cout << "t<2> = " << static_ranges::element<2>(t) << std::endl;

	my_array a = { 40, 41, 42, 43, 44, 45, 46, 47, 48, 49 };

	std::cout << "a[0] = " << static_ranges::element<0>(a) << std::endl;
	std::cout << "a[1] = " << static_ranges::element<1>(a) << std::endl;
	std::cout << "a[2] = " << static_ranges::element<2>(a) << std::endl;
	std::cout << "a[3] = " << static_ranges::element<3>(a) << std::endl;
	std::cout << "a[4] = " << static_ranges::element<4>(a) << std::endl;
	std::cout << "a[5] = " << static_ranges::element<5>(a) << std::endl;
	std::cout << "a[6] = " << static_ranges::element<6>(a) << std::endl;
	std::cout << "a[7] = " << static_ranges::element<7>(a) << std::endl;
	std::cout << "a[8] = " << static_ranges::element<8>(a) << std::endl;
	std::cout << "a[9] = " << static_ranges::element<9>(a) << std::endl;

	std::cout << "Done." << std::endl;

	return 0;
}

/**/