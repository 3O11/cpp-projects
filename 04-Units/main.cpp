#include <iostream>

#include "units.h"

enum class si_units
{
    second,
    metre,
    kilogram,
    apmere,
    kelvin,
    mole,
    candela,
    _count,
};

using second = basic_unit<si_units, si_units::second>;
using metre = basic_unit<si_units, si_units::metre>;
using kilogram = basic_unit<si_units, si_units::kilogram>;

using metre_per_second = divided_unit<metre, second>;
using newton = divided_unit<multiplied_unit<kilogram, metre>, multiplied_unit<second, second>>;

using vec = static_vector<1, 2, 3, 4, 5>;

int main()
{

}