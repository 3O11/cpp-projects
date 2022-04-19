#ifndef _STATIC_VECTOR_H
#define _STATIC_VECTOR_H 1

#include <cstdint>

template <int ... Values>
struct static_vector
{};

// Add an element to the end of the vector
template <typename Vector, int Added>
struct push_back
{};

template <int Added, int ... Values>
struct push_back<static_vector<Values ...>, Added>
{
    using type = static_vector<Values ..., Added>;
};

template <typename Vector, int Added>
using push_back_t = typename push_back<Vector, Added>::type;

// Get value at an index
template <typename Vector, std::size_t Index>
struct at
{};

template <int Head, int ... Tail>
struct at<static_vector<Head, Tail ...>, 0>
{
    static constexpr auto value = Head;
};

template <std::size_t Index, int Head, int ... Tail>
struct at<static_vector<Head, Tail ...>, Index>
{
    static constexpr auto value = at<static_vector<Tail ...>, Index - 1>::value;
};

// Join two vectors together
template <typename Vector1, typename Vector2>
struct join
{};

template <int ... Values1, int ... Values2>
struct join<static_vector<Values1 ...>, static_vector<Values2 ...>>
{
    using type = static_vector<Values1 ..., Values2 ...>;
};

template <typename Vector1, typename Vector2>
using join_t = typename join<Vector1, Vector2>::type;

// Create a vector with specific size and initial value
namespace detail
{
    template <std::size_t Size, int Value, int ... Values>
    struct vector_with_size_impl
    {
        using type = typename vector_with_size_impl<Size - 1, Value, Value, Values ...>::type;
    };

    template <int Value, int ... Values>
    struct vector_with_size_impl<0, Value, Values ...>
    {
        using type = static_vector<Value, Values ...>;
    };
}

template <std::size_t Size, int Value>
struct vector_with_size
{
    using type = typename detail::vector_with_size_impl<Size - 1, Value>::type;
};

template <std::size_t Size>
struct vector_with_size<Size, 0>
{
    using type = typename detail::vector_with_size_impl<Size - 1, 0>::type;
};

template <std::size_t Size, int Value>
using vector_with_size_t = typename vector_with_size<Size, Value>::type;

// Set a value at a specific index of the static_vector
namespace detail
{
    template <typename OldVector, typename NewVector, std::size_t Index, int Value>
    struct set_impl
    {};

    template <typename NewVector, std::size_t Index, int Value, int OldHead, int ... OldTail>
    struct set_impl<static_vector<OldHead, OldTail ...>, NewVector, Index, Value>
    {
        using type = typename set_impl<static_vector<OldTail ...>, push_back_t<NewVector, OldHead>, Index - 1, Value>::type;
    };

    template <typename NewVector, int Value, int OldHead, int ... OldTail>
    struct set_impl<static_vector<OldHead, OldTail ...>, NewVector, 0, Value>
    {
        using type = join_t<push_back_t<NewVector, Value>, static_vector<OldTail ...>>;
    };
}

template <typename Vector, std::size_t Index, int Value>
struct set
{
    using type = typename detail::set_impl<Vector, static_vector<>, Index, Value>::type;
};

template <typename Vector, std::size_t Index, int Value>
using set_t = typename set<Vector, Index, Value>::type;

// Add/Subtract two vectors
namespace detail
{
    template <typename Vector1, typename Vector2, typename AddedVector>
    struct add_impl
    {};

    template <typename AddedVector>
    struct add_impl<static_vector<>, static_vector<>, AddedVector>
    {
        using type = AddedVector;
    };

    template <int Head1, int ... Tail1, int Head2, int ... Tail2, typename AddedVector>
    struct add_impl<static_vector<Head1, Tail1 ...>, static_vector<Head2, Tail2 ...>, AddedVector>
    {
        using type = typename add_impl<static_vector<Tail1 ...>, static_vector<Tail2 ...>, push_back_t<AddedVector, (Head1 + Head2)>>::type;
    };

    template <typename Vector1, typename Vector2, typename SubtractedVector>
    struct sub_impl
    {};

    template <typename SubtractedVector>
    struct sub_impl<static_vector<>, static_vector<>, SubtractedVector>
    {
        using type = SubtractedVector;
    };

    template <int Head1, int ... Tail1, int Head2, int ... Tail2, typename SubtractedVector>
    struct sub_impl<static_vector<Head1, Tail1 ...>, static_vector<Head2, Tail2 ...>, SubtractedVector>
    {
        using type = typename sub_impl<static_vector<Tail1 ...>, static_vector<Tail2 ...>, push_back_t<SubtractedVector, (Head1 - Head2)>>::type;
    };
}

template <typename Vector1, typename Vector2>
struct add
{
    using type = typename detail::add_impl<Vector1, Vector2, static_vector<>>::type;
};

template <typename Vector1, typename Vector2>
using add_t = typename add<Vector1, Vector2>::type;

template <typename Vector1, typename Vector2>
struct sub
{
    using type = typename detail::sub_impl<Vector1, Vector2, static_vector<>>::type;
};

template <typename Vector1, typename Vector2>
using sub_t = typename sub<Vector1, Vector2>::type;

#endif // _STATIC_VECTOR_H
