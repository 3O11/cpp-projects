#ifndef TESTS_COMMON_H
#define TESTS_COMMON_H


// Helper templates for static testing
 

template <typename T1, typename T2>
struct is_addable
{
    static constexpr bool value = false;
};

template <typename T1, typename T2>
    requires requires (T1 x, T2 y) { x + y; }
struct is_addable<T1, T2>
{
    static constexpr bool value = true;
};


template <typename T1, typename T2>
struct is_subtractable
{
    static constexpr bool value = false;
};

template <typename T1, typename T2>
    requires requires (T1 x, T2 y) { x - y; }
struct is_subtractable<T1, T2>
{
    static constexpr bool value = true;
};


template <typename T1, typename T2>
struct is_multipliable
{
    static constexpr bool value = false;
};

template <typename T1, typename T2>
    requires requires (T1 x, T2 y) { x * y; }
struct is_multipliable<T1, T2>
{
    static constexpr bool value = true;
};


template <typename T1, typename T2>
struct is_divisible
{
    static constexpr bool value = false;
};

template <typename T1, typename T2>
    requires requires (T1 x, T2 y) { x / y; }
struct is_divisible<T1, T2>
{
    static constexpr bool value = true;
};


template <typename TUnit>
struct unit_traits
{};

template <typename TEnum, typename TPowers>
struct unit_traits<unit<TEnum, TPowers>>
{
    using enum_type = TEnum;
    using powers_type = TPowers;
};


// Default usage


enum class si_units
{
    second,
    metre,
    kilogram,
    ampere,
    kelvin,
    mole,
    candela,
    _count
};

using second = basic_unit<si_units, si_units::second>;
using metre = basic_unit<si_units, si_units::metre>;
using kilogram = basic_unit<si_units, si_units::kilogram>;
using ampere = basic_unit<si_units, si_units::ampere>;
using kelvin = basic_unit<si_units, si_units::kelvin>;
using mole = basic_unit<si_units, si_units::mole>;
using candela = basic_unit<si_units, si_units::candela>;

using metre_per_second = divided_unit<metre, second>;
using cubic_metre = multiplied_unit<metre, metre, metre>;

#endif
