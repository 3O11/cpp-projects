#include "static_ranges.hpp"
#include <functional>
#include <iostream>
#include <iomanip>

using my_pair = std::pair<int, std::string>;
using my_tuple = std::tuple<int, double, std::string>;
using my_array = std::array<int, 10>;

inline auto my_iota = static_views::iota<int, 10>();

inline auto my_static_iota = static_views::static_iota<int, 10>();

int main(int argc, char** argv)
{
	auto printer = [](auto&& x) {
		std::cout << " " << x;
	};
	auto print1 = [printer]<typename T>(T && x) {
		static_ranges::for_each(std::forward<T>(x), printer);
		std::cout << std::endl;
	};
	auto print = [print1]<typename T>(auto l, T && x) {
		std::cout << l << "=";
		print1(std::forward<T>(x));
	};
	auto print2 = [print1]<typename T>(auto l, T && x) {
		std::cout << l << ":" << std::endl;
		static_ranges::for_each(std::forward<T>(x), print1);
	};
	my_pair ap, bp = { 1, "thr" }, cp = { 2, "ee" };
	static_ranges::transform(bp, cp, ap, [](auto&& x, auto&& y) {
		return x + y;
		});
	print("ap", ap);

	my_array aa, bb;
	static_ranges::copy(my_iota, aa);
	print("aa", aa);

	static_ranges::copy(my_static_iota, aa);
	print("aa", aa);

	auto my_view = static_views::transform(aa, [](auto&& x) { return 2 * x; });
	static_ranges::copy(std::move(my_view), bb);
	print("bb", bb);

	auto my_view2 = aa | static_views::transform([](auto&& x) { return 2 * x + 1; });
	print("my_view2", my_view2);

	print("my_static_iota", my_static_iota);

	auto incrementlambda = [](auto&& x) {
		return std::integral_constant<int, x.value + 1>();
	};

	auto incrementator = static_views::transform(incrementlambda);

	auto vectorlambda = [](auto&& x) {
		std::array<int, x.value> rv;
		static_ranges::copy(static_views::iota<int, x.value>(), rv);
		return rv;
	};

	auto vectorator = static_views::transform(vectorlambda);

	auto my_static_iota_1 = my_static_iota | incrementator;

	print("my_static_iota_1", my_static_iota_1);

	auto iota_1_tuple = static_ranges::to_tuple(my_static_iota_1);

	print("iota_1_tuple", iota_1_tuple);

	auto triangle = my_static_iota_1 | vectorator;

	print2("triangle", triangle);

	auto triangle_tuple = static_ranges::to_tuple(triangle);

	print2("triangle_tuple", triangle_tuple);

	auto my_static_pair = static_ranges::to_pair(static_views::static_iota<int, 2>() | incrementator);
	print("my_static_pair", my_static_pair);

	auto zeroone = static_views::iota<int, 2>();
	auto my_pair0 = static_ranges::to_pair(zeroone);
	print("my_pair0", my_pair0);

	//auto incrlambda = [](auto&& x) { return x + 1; };
//	auto incrlambda = std::bind(std::plus<int>(), std::placeholders::_1, 1);

//	std::cout << incrlambda(729) << std::endl;

	auto onetwo = static_views::iota<int, 2>() | static_views::transform(std::bind(std::plus(), std::placeholders::_1, 1));
//	auto onetwo = static_views::transform(static_views::iota<int, 2>(), incrlambda);
	print("onetwo", onetwo);
	
	auto my_pair = static_ranges::to_pair(onetwo);
	print("my_pair", my_pair);

	auto my_array = static_ranges::to_array<int>(my_iota);
	print("my_array", my_array);
	
	return 0;
}

/**/