#pragma once

#include "./template.h"
#include "./environment.h"

NAGISA_BUILD_LIB_DETAIL_BEGIN

struct empty{};

template <class Tuple>
	requires (!::std::is_volatile_v<Tuple>)
struct tuple_size;

template <class Tuple>
struct tuple_size<const Tuple> : ::std::conditional_t<requires{ { tuple_size<Tuple>::value }; }, tuple_size<Tuple>, empty> {}; // ignore cv

template<class Tuple>
inline constexpr auto tuple_size_v = tuple_size<Tuple>::value;

template <::std::size_t Index, class Tuple>
	requires (!::std::is_volatile_v<Tuple>)
struct
#if defined(_MSC_VER) && !defined(__clang__)
	[[msvc::known_semantics]]
#endif
tuple_element;

template <::std::size_t Index, class Tuple>
struct
#if defined(_MSC_VER) && !defined(__clang__)
	[[msvc::known_semantics]]
#endif
tuple_element<Index, const Tuple> : tuple_element<Index, Tuple>
{
	using base_type = tuple_element<Index, Tuple>;
	using type = ::std::add_const_t<typename base_type::type>;
};

template<::std::size_t I, class Tuple>
using tuple_element_t = typename tuple_element<I, Tuple>::type;


namespace detail_get
{
	//for ADL
	void get();

	template<class T, ::std::size_t I>
	concept get_has_member = requires(T tuple) { NAGISA_STL_FREESTANDING_UTILITY_FORWARD(tuple).template get<I>(); };

	template<class T, ::std::size_t I>
	concept get_has_adl = requires(T tuple) { get<I>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(tuple)); };

	template<::std::size_t I>
	struct get_cpo
	{
		constexpr decltype(auto) operator()(auto&& tuple)const noexcept requires get_has_member<decltype(tuple), I> || get_has_adl<decltype(tuple), I>
		{
			if constexpr (get_has_member<decltype(tuple), I>)
			{
				return NAGISA_STL_FREESTANDING_UTILITY_FORWARD(tuple).template get<I>();
			}
			else if constexpr (get_has_adl<decltype(tuple), I>)
			{
				return get<I>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(tuple));
			}
			else
			{
				static_assert(
					concepts::none<decltype(tuple)>,
					"tuple does not have get method"
					);
			}
		}
	};
}

template<::std::size_t I>
inline constexpr detail_get::get_cpo<I> get{};

NAGISA_BUILD_LIB_DETAIL_END