#include "static_ranges.hpp"

#include <iostream>
#include <iomanip>

int main(int argc, char** argv)
{
	auto iota_view = static_ranges::views::iota<int,5>();

	static_assert(static_ranges::size_v<decltype(iota_view)> == 5, "size_v<decltype(iota_view)> shall be equal to 5");

	std::cout << "iota_view[0] = " << static_ranges::element<0>(iota_view) << std::endl;
	std::cout << "iota_view[1] = " << static_ranges::element<1>(iota_view) << std::endl;
	std::cout << "iota_view[2] = " << static_ranges::element<2>(iota_view) << std::endl;
	std::cout << "iota_view[3] = " << static_ranges::element<3>(iota_view) << std::endl;
	std::cout << "iota_view[4] = " << static_ranges::element<4>(iota_view) << std::endl;

	auto char_view = static_ranges::views::iota<char, 127>();

	static_assert(static_ranges::size_v<decltype(char_view)> == 127, "size_v<decltype(char_view)> shall be equal to 127");

	std::cout << "char_view[65] = " << static_ranges::element<65>(char_view) << std::endl;
	std::cout << "char_view[66] = " << static_ranges::element<66>(char_view) << std::endl;
	std::cout << "char_view[67] = " << static_ranges::element<67>(char_view) << std::endl;

	std::cout << "Done." << std::endl;

	return 0;
}

/**/