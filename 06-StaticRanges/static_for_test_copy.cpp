#include "static_for_test_struct.hpp"

#include <iostream>
#include <iomanip>

int main(int argc, char** argv)
{
	range_three r = { 42, 3.14, "Some_quite_large_string_containing_Foo_and_Bar" };

	range_three r2;

	static_ranges::copy(r, r2);

	std::cout << "r2.x = " << r2.x << std::endl;
	std::cout << "r2.y = " << r2.y << std::endl;
	std::cout << "r2.z = " << r2.z << std::endl;

	std::cout << "Done." << std::endl;

	return 0;
}

/**/