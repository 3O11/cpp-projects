#include "units.h"
#include "tests_common.h"
#include <iostream>

using namespace std;

enum class my_units
{
    czk,
    beer,
    coffee,
    absinth,
    pizza,
    chip,
    baguette,
    _count
};

using czk = basic_unit<my_units, my_units::czk>;
using beer = basic_unit<my_units, my_units::beer>;
using coffee = basic_unit<my_units, my_units::coffee>;
using absinth = basic_unit<my_units, my_units::absinth>;
using pizza = basic_unit<my_units, my_units::pizza>;
using chip = basic_unit<my_units, my_units::chip>;
using baguette = basic_unit<my_units, my_units::baguette>;

using beer_price = divided_unit<beer, czk>;

int main()
{
    // It can't be possible to combine together units from different systems
    // (even with the same enum size)

    cout << is_same<
        typename unit_traits<second>::enum_type,
        typename unit_traits<metre>::enum_type
    >::value << endl;

    cout << is_same<
        typename unit_traits<czk>::enum_type,
        typename unit_traits<beer>::enum_type
    >::value << endl;

    cout << is_same<
        typename unit_traits<second>::enum_type,
        typename unit_traits<beer>::enum_type
    >::value << endl;

    cout << is_same<
        typename unit_traits<metre_per_second>::enum_type,
        typename unit_traits<beer_price>::enum_type
    >::value << endl;

    cout << is_addable<quantity<czk>, quantity<second>>::value << endl;
    cout << is_subtractable<quantity<czk>, quantity<second>>::value << endl;
    cout << is_multipliable<quantity<czk>, quantity<second>>::value << endl;
    cout << is_divisible<quantity<czk>, quantity<second>>::value << endl;
}