#include "static_for_test_struct.hpp"

#include <iostream>
#include <iomanip>

static_assert(static_ranges::size_v<range_three> == 3, "static_ranges::size_v<range_three> shall equal to 3");

int main(int argc, char** argv)
{
	range_three r = { 42, 3.14, "Some_quite_large_string_containing_Foo_and_Bar" };

	const range_three& r1 = r;

	std::cout << "r1.x = " << static_ranges::element<0>(r1) << std::endl;
	std::cout << "r1.y = " << static_ranges::element<1>(r1) << std::endl;
	std::cout << "r1.z = " << static_ranges::element<2>(r1) << std::endl;

	range_three r2;
	static_ranges::element<0>(r2) = static_ranges::element<0>(std::move(r));
	static_ranges::element<1>(r2) = static_ranges::element<1>(std::move(r));
	static_ranges::element<2>(r2) = static_ranges::element<2>(std::move(r));

	std::cout << "r2.x = " << static_ranges::element<0>(r2) << " stolen from r.x = " << static_ranges::element<0>(r) << std::endl;
	std::cout << "r2.y = " << static_ranges::element<1>(r2) << " stolen from r.y = " << static_ranges::element<1>(r) << std::endl;
	std::cout << "r2.z = " << static_ranges::element<2>(r2) << " stolen from r.z = " << static_ranges::element<2>(r) << std::endl;

	std::cout << "Done." << std::endl;

	return 0;
}

/**/