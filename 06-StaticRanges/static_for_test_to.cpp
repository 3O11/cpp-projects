#include "static_ranges.hpp"

#include "static_for_test_struct.hpp"

#include <iostream>
#include <iomanip>

int main(int argc, char** argv)
{
	range_two var_two = { 42, 3.14 };
	auto result_two = static_ranges::to_pair(var_two);

	std::cout << "result_two.first = " << result_two.first << std::endl;
	std::cout << "result_two.second = " << result_two.second << std::endl;

	range_three var_three = { 42, 3.14, "Some_quite_large_string_containing_Foo_and_Bar" };
	auto result_three = static_ranges::to_tuple(var_three);

	std::cout << "result_three<0> = " << std::get<0>(result_three) << std::endl;
	std::cout << "result_three<1> = " << std::get<1>(result_three) << std::endl;
	std::cout << "result_three<2> = " << std::get<2>(result_three) << std::endl;

	range_ten var_ten = { 40, 41, 42, 43, 44, 45, 46, 47, 48, 49 };
	auto result_ten = static_ranges::to_array<int>(var_ten);
	
	std::cout << "result_ten[0] = " << result_ten[0] << std::endl;
	std::cout << "result_ten[1] = " << result_ten[1] << std::endl;
	std::cout << "result_ten[2] = " << result_ten[2] << std::endl;
	std::cout << "result_ten[3] = " << result_ten[3] << std::endl;
	std::cout << "result_ten[4] = " << result_ten[4] << std::endl;
	std::cout << "result_ten[5] = " << result_ten[5] << std::endl;
	std::cout << "result_ten[6] = " << result_ten[6] << std::endl;
	std::cout << "result_ten[7] = " << result_ten[7] << std::endl;
	std::cout << "result_ten[8] = " << result_ten[8] << std::endl;
	std::cout << "result_ten[9] = " << result_ten[9] << std::endl;

	std::cout << "Done." << std::endl;

	return 0;
}

/**/