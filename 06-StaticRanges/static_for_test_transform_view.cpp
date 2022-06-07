#include "static_for_test_struct.hpp"

#include <iostream>
#include <iomanip>

int main(int argc, char** argv)
{
	range_three r = { 42, 3.14, "Some_quite_large_string_containing_Foo_and_Bar" };

	auto lambda = [](auto&& a) {
		if constexpr (std::is_arithmetic_v<std::remove_cvref_t<decltype(a)>>)
		{
			return "(" + std::to_string(a) + ")";
		}
		else
		{
			return a.size();
		}
	};

	auto r2 = static_views::transform(r, lambda);

	std::cout << "r2.x = " << static_ranges::element<0>(r2) << std::endl;
	std::cout << "r2.y = " << static_ranges::element<1>(r2) << std::endl;
	std::cout << "r2.z = " << static_ranges::element<2>(r2) << std::endl;

	std::cout << "Done." << std::endl;

	return 0;
}

/**/