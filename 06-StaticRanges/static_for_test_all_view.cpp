#include "static_for_test_struct.hpp"

#include <iostream>
#include <iomanip>

int main(int argc, char** argv)
{
	range_three r = { 42, 3.14, "Some_quite_large_string_containing_Foo_and_Bar" };

	auto r_view = static_ranges::views::all(r);

	static_assert(static_ranges::size_v<decltype(r_view)> == 3, "size_v<decltype(r_view)> shall be equal to 3");

	std::cout << "r_view.x = " << static_ranges::element<0>(r_view) << std::endl;
	std::cout << "r_view.y = " << static_ranges::element<1>(r_view) << std::endl;
	std::cout << "r_view.z = " << static_ranges::element<2>(r_view) << std::endl;

	static_ranges::element<0>(r_view) -= 40;
	static_ranges::element<1>(r_view) -= 2.0;
	static_ranges::element<2>(r_view) += "_and_something_else_appended";

	std::cout << "r.x = " << static_ranges::element<0>(r) << std::endl;
	std::cout << "r.y = " << static_ranges::element<1>(r) << std::endl;
	std::cout << "r.z = " << static_ranges::element<2>(r) << std::endl;

	std::cout << "Done." << std::endl;

	return 0;
}

/**/