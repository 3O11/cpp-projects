#include "units.h"
#include "tests_common.h"
#include <iostream>

using namespace std;

int main()
{
    // Basic usage from the sample

    quantity<metre> l(2.1);
    quantity<second> t(0.9);
    auto v1(l / t);

    quantity<metre_per_second> v2{ 2.5 };

    cout << (v1 + v2).value() << endl;
    cout << is_addable<decltype(l), decltype(t)>::value << endl;
}
