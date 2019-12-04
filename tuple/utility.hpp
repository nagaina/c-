#ifndef UTILITY_HPP
#define UTILITY_HPP

template <class _T>
struct RemoveReference
{
	using type = _T;
};

template <class _T>
struct RemoveReference<_T&>
{
	//	remove refernce 
	using type = _T;
};

template <class _T>
struct RemoveReference<_T&&>
{
	//remove rvalue reference
	using type = _T;
};

template<class _T>
using RemoveReference_T = typename RemoveReference<_T>::type;

//	forward function
template <typename _Type>
constexpr _Type&& Forward(RemoveReference_T<_Type>& _T) noexcept
{
	//	forward lvalue as an lvalue/rvalue
	return (static_cast<_Type&&>(_T));
}

template <typename _Type>
constexpr _Type&& Forward(RemoveReference_T<_Type>&& _T) noexcept
{
	//	formward rvalue as an rvalue
	return (static_cast<_Type&&>(_T));
}

//	move function
template <typename _Type>
constexpr RemoveReference_T<_Type>&& Move(_Type&& _T) noexcept
{
	return (static_cast<RemoveReference_T<_Type>&&>(_T));
}

//typename <class T1, class T2, class This = void>
//struct _Property
//{
//};
//
//template <class T1, class T2, class... Types>
//struct Property
//{
//	typedef _Property<T1, T2, typename _Property<Types...>>::type type;
//};
//
//template <class T1, class T2>
//struct Property
//{
//	typedef _Property<T1, T2> type;
//};

#endif