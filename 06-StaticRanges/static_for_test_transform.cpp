#include "static_for_test_struct.hpp"

#include <iostream>
#include <iomanip>

int main(int argc, char** argv)
{
	const range_three r = { 42, 3.14, "Some_quite_large_string_containing_Foo_and_Bar" };

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

	range_three_2 r2;

	static_ranges::transform(r, r2, lambda);

	std::cout << "r2.u = " << r2.u << std::endl;
	std::cout << "r2.v = " << r2.v << std::endl;
	std::cout << "r2.w = " << r2.w << std::endl;

	range_three r3 = { -40, -2.0, "_plus_suffix" };
	range_three r4;
	static_ranges::transform(r, std::move(r3), r4, [](auto&& x, auto&& y) {
		return x + y;
		});

	std::cout << "r4.x = " << r4.x << std::endl;
	std::cout << "r4.y = " << r4.y << std::endl;
	std::cout << "r4.z = " << r4.z << std::endl;

	std::cout << "Done." << std::endl;

	return 0;
}

/**/