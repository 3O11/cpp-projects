#include "static_for_test_struct.hpp"

#include <iostream>
#include <iomanip>

int main(int argc, char** argv)
{
	range_three r = { 42, 3.14, "Some_quite_large_string_containing_Foo_and_Bar" };

	auto printer = [](auto && v){
		std::cout << "(" << v << ")" << std::endl;
		};

	static_ranges::for_each(r, printer);

	std::cout << "Done." << std::endl;

	return 0;
}

/**/