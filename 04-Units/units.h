#ifndef _UNITS_H
#define _UNITS_H 1

#include <cstdint>
#include <type_traits>

#include "static_vector.h"

template <typename TEnum, typename TPowers>
struct unit;

//////////////////////////////////////////////////////////////////////////////
// basic_unit
//////////////////////////////////////////////////////////////////////////////

template <typename TEnum, TEnum index>
using basic_unit = unit<TEnum, set_t<vector_with_size_t<(std::size_t)TEnum::_count, 0>, (std::size_t)index, 1>>;

//////////////////////////////////////////////////////////////////////////////
// multiplied_unit and divided_unit
//////////////////////////////////////////////////////////////////////////////

namespace detail
{
    template <typename TFirstUnit, typename ... TOtherUnits>
    struct multiplied_unit_impl
    {};

    template <typename TFirstUnit>
    struct multiplied_unit_impl<TFirstUnit>
    {
        using type = TFirstUnit;
    };

    template <typename TUnitEnum, typename TFirstPowers, typename TSecondPowers, typename ... TOtherUnits>
    struct multiplied_unit_impl<unit<TUnitEnum, TFirstPowers>, unit<TUnitEnum, TSecondPowers>, TOtherUnits ...>
    {
        //static_assert(std::is_same_v<TFirstEnum, TSecondEnum>);
        using type = typename multiplied_unit_impl<unit<TUnitEnum, add_t<TFirstPowers, TSecondPowers>>, TOtherUnits ...>::type;
    };

    template <typename TUnitEnum, typename TFirstPowers, typename TSecondPowers>
    struct multiplied_unit_impl<unit<TUnitEnum, TFirstPowers>, unit<TUnitEnum, TSecondPowers>>
    {
        //static_assert(std::is_same_v<TFirstEnum, TSecondEnum>);
        using type = unit<TUnitEnum, add_t<TFirstPowers, TSecondPowers>>;
    };

    template <typename TDividendUnit, typename TDivisorUnit>
    struct divided_unit_impl
    {};

    template <typename TUnitEnum, typename TDividendPowers, typename TDivisorPowers>
    struct divided_unit_impl<unit<TUnitEnum, TDividendPowers>, unit<TUnitEnum, TDivisorPowers>>
    {
        using type = unit<TUnitEnum, sub_t<TDividendPowers, TDivisorPowers>>;
    };
}

template <typename TFirstUnit, typename ... TOtherUnits>
using multiplied_unit = typename detail::multiplied_unit_impl<TFirstUnit, TOtherUnits ...>::type;

template <typename TDividendUnit, typename TDivisorUnit>
using divided_unit = typename detail::divided_unit_impl<TDividendUnit, TDivisorUnit>::type;

//////////////////////////////////////////////////////////////////////////////
// quantity and its operators
//////////////////////////////////////////////////////////////////////////////

template <typename TUnit, typename TValue = double>
struct quantity
{
    explicit quantity(TValue value) : _value(value) {}
    TValue value() const { return _value; }
private:
    TValue _value;
};

//
// I'm not sure if the const& is necessary for the quantities, I'd think that it isn't, because
// most TValue types aren't going to be large enough to make a performance impact when copied.
// I'm still leaving the const& there, just in case.
//

template <typename TUnit, typename TValue>
inline quantity<TUnit, TValue> operator+(const quantity<TUnit, TValue>& q1, const quantity<TUnit, TValue>& q2)
{
    return quantity<TUnit, TValue>(q1.value() + q2.value());
}

template <typename TUnit, typename TValue>
inline quantity<TUnit, TValue> operator-(const quantity<TUnit, TValue>& q1, const quantity<TUnit, TValue>& q2)
{
    return quantity<TUnit, TValue>(q1.value() - q2.value());
}

//
// I don't like this implementation, I think it's unnecessarily verbose for what it's doing,
// but I wasn't able to find a way to make it more compact and at the same time compile for
// the the is_multipliable/is_divisible checks.
//

template <typename TUnitEnum, typename TFirstPowers, typename TSecondPowers, typename TValue>
inline auto operator*(const quantity<unit<TUnitEnum, TFirstPowers>, TValue>& q1, const quantity<unit<TUnitEnum, TSecondPowers>, TValue>& q2)
{
    return quantity<multiplied_unit<unit<TUnitEnum, TFirstPowers>, unit<TUnitEnum, TSecondPowers>>, TValue>(q1.value() * q2.value());
}

template <typename TUnitEnum, typename TFirstPowers, typename TSecondPowers, typename TValue>
inline auto operator/(const quantity<unit<TUnitEnum, TFirstPowers>, TValue>& q1, const quantity<unit<TUnitEnum, TSecondPowers>, TValue>& q2)
{
    return quantity<divided_unit<unit<TUnitEnum, TFirstPowers>, unit<TUnitEnum, TSecondPowers>>, TValue>(q1.value() / q2.value());
}

/*
template <typename TUnit1, typename TUnit2, typename TValue>
inline auto operator/(quantity<TUnit1, TValue> q1, quantity<TUnit2, TValue> q2)
{
    return quantity<divided_unit<TUnit1, TUnit2>, TValue>(q1.value() / q2.value());
}

template <typename TUnit1, typename TUnit2, typename TValue>
inline auto operator*(quantity<TUnit1, TValue> q1, quantity<TUnit2, TValue> q2)
{
    return quantity<multiplied_unit<TUnit1, TUnit2>, TValue>(q1.value() * q2.value());
}
*/

#endif // _UNITS_H
