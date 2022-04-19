#include "units.h"
#include "tests_common.h"
#include <iostream>

using namespace std;

int main()
{
    // Different ways of combining units must lead to the same type

    cout << is_same<
        multiplied_unit<metre, metre, metre>,
        multiplied_unit<metre, multiplied_unit<metre, metre>>
    >::value << endl;

    cout << is_same<
        multiplied_unit<metre, metre, metre>,
        multiplied_unit<multiplied_unit<metre, metre>, metre>
    >::value << endl;


    using newton_direct =
        divided_unit<
            multiplied_unit<kilogram, metre>,
            multiplied_unit<second, second>
        >;

    using newton_indirect = 
        multiplied_unit<
            kilogram,
            divided_unit<
                divided_unit<metre, second>,
                second
            >
        >;

    cout << is_same<newton_direct, newton_indirect>::value << endl;

    using scalar = divided_unit<second, second>;

    cout << is_same<
        multiplied_unit<metre, scalar>,
        multiplied_unit<metre>
    >::value << endl;

    cout << is_same<
        multiplied_unit<metre, scalar>,
        multiplied_unit<scalar, metre>
    >::value << endl;
}