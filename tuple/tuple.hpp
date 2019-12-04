#ifndef TUPLE_HPP
#define TUPLE_HPP

#include "utility.hpp"
#include <cstddef>
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Tuple class
template <class... Types>
class Tuple
//	writing braces in order not to write the Tuple class with empty arguments
{};

template <class T, class... Types>
class Tuple<T, Types... > : public Tuple<Types...>
{
	// recursivly call constructor 
public:
	constexpr Tuple(T type, Types... _types) noexcept
		: Tuple<Types...>(Forward<Types&&>(_types)...),
		m_type(Forward<T&&>(type)) 
	{
	}

	T m_type;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Tuple element
template <std::size_t I, class T>
struct TupleElement;

template <std::size_t I, class T, class... Types>
struct TupleElement<I, Tuple<T, Types...>> : public TupleElement<I - 1, Tuple<Types...>>
{
};

template <class T, class... Types>
struct TupleElement<0, Tuple<T, Types...>>
{
	using type = T;
	using _Tuple = Tuple<T, Types...>;
};

template <size_t I, class Types>
using TupleElement_T = typename TupleElement<I, Types>::type;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Getters
//	Index-based access
template <std::size_t I, class... Types>
constexpr TupleElement_T<I, Tuple<Types...>>& Get(Tuple<Types...>& tuple) noexcept
{
	using __Tuple = typename TupleElement<I, Tuple<Types...>>::_Tuple;
	return ((__Tuple&)tuple).m_type;
}

template <std::size_t I, class... Types>
constexpr TupleElement_T<I, Tuple<Types...>>&& Get(Tuple<Types...>&& tuple) noexcept
{
	using __Tuple = typename TupleElement<I, Tuple<Types...>>::_Tuple;
	return Forward<TupleElement<I, Tuple<Types...>>&&>(((__Tuple&)tuple).m_type);
}

template <std::size_t I, class... Types>
constexpr TupleElement_T<I, Tuple<Types...>> const& Get(const Tuple<Types...>& tuple) noexcept
{
	using __Tuple = typename TupleElement<I, Tuple<Types...>>::_Tuple;
	return ((__Tuple&)tuple).m_type;
}

template <std::size_t I, class... Types>
constexpr TupleElement_T<I, Tuple<Types...>> const&& Get(const Tuple<Types...>&& tuple) noexcept
{
	using __Tuple = typename TupleElement<I, Tuple<Types...>>::_Tuple;
	return Forward<const TupleElement<I, Tuple<Types...>>&&>(((__Tuple&)tuple).m_type);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Type-based access
template <class T, class U>
struct _TupleElement;

// recursive implementation
template <class T, class U, class... Types>
struct _TupleElement<T, Tuple<U, Types...>> : public _TupleElement<T, Tuple<Types...>>
{
};

//	class for checkType 
template <class T>
struct _TupleElement<T, Tuple<>>
{
	using CheckType = void;
};

template <class T, class... Types>
struct _TupleElement<T, Tuple<T, Types...>>
{
	using CheckType = int;
	static_assert(std::is_void<typename _TupleElement<T, Tuple<Types...>>::CheckType>::value, "redefinition");

	using type = T;
	using _Tuple = Tuple<T, Types...>;
};

template <class T, class Types>
using _TupleElement_T = typename _TupleElement<T, Types>::type;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T, class... Types>
constexpr T& Get(Tuple<Types...>& tuple) noexcept
{
	using __Tuple = typename _TupleElement<T, Tuple<Types...>>::_Tuple;
	return ((__Tuple&)tuple).m_type;
}

template <class T, class... Types>
constexpr const T& Get(const Tuple<Types...>& tuple) noexcept
{
	using __Tuple = typename _TupleElement<T, Tuple<Types...>>::_Tuple;
	return Forward<_TupleElement<T, Tuple<Types...>>&&>((__Tuple&)tuple).m_type;
}

template <class T, class... Types>
constexpr T&& Get(Tuple<Types...>&& tuple) noexcept
{
	using __Tuple = typename _TupleElement<T, Tuple<Types...>>::_Tuple;
	return ((__Tuple&)tuple).m_type;
}

template <class T, class... Types>
constexpr const T&& Get(const Tuple<Types...>&& tuple) noexcept
{
	using __Tuple = typename _TupleElement<T, Tuple<Types...>>::_Tuple;
	return Forward<const _TupleElement<T, Tuple<Types...>>&&>((__Tuple&)tuple).m_type;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	another implemention with recursion
//	get with type
template <class T, class FT, class... Types>
constexpr typename std::enable_if<std::is_same<T, FT>::value, T>::type get(const Tuple<FT, Types...>&  tuple) noexcept
{
	//static_assert(std::is_same<T, decltype(get<FT, Types...>(tuple))>::value, "redefinition");
	return tuple.m_type;
}

template <class T, class FT, class... Types>
constexpr typename std::enable_if<!std::is_same<T, FT>::value, T>::type get(const Tuple<FT, Types...>&  tuple) noexcept
{
	return get<T, Types...>(tuple);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <size_t I, class T>
struct TypeHolder;

template <size_t I, class T, class... Types>
struct TypeHolder<I, Tuple<T, Types...>> : public TypeHolder<I - 1, Tuple<Types...>>
{
	using type = typename TypeHolder<I - 1, Tuple<Types...>>::type;
};

template <class T, class... Types>
struct TypeHolder<0, Tuple<T, Types...>>
{
	using type = T;
};

template <int I, class Types>
using TypeHolder_t = typename TypeHolder<I, Types>::type;

template <int N, int C, class... Types>
constexpr typename std::enable_if<N == C, TypeHolder_t<N, Tuple<Types...>>>::type get(const Tuple<Types...>& tuple) noexcept
{
	return tuple.m_type;
}

template <int N, int C, class... Types>
constexpr typename std::enable_if<(N != C && C >=0), TypeHolder_t<N, Tuple<Types...>>>::type get(const Tuple<Types...>& tuple) noexcept
{
	return get<N, C - 1>(tuple);
}

template <int N, class... Types>
constexpr typename std::enable_if<N >= 0, TypeHolder_t<N, Tuple<Types...>>>::type get(const Tuple<Types...>& tuple) noexcept
{
	//	TODO: get actual size of tuple
	return get<N, sizeof(tuple)>(tuple);
}

#endif