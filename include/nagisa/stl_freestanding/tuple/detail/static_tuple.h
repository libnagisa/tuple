#pragma once

#include "./trait.h"
#include "./environment.h"

NAGISA_BUILD_LIB_DETAIL_BEGIN

namespace static_tuple_adl
{
	template<class...>
	struct static_tuple
	{
		using self_type = static_tuple;
	public:
	};

	template<class First, class... Rest>
	struct static_tuple<First, Rest...> : static_tuple<Rest...>
	{
		using base_type = static_tuple<Rest...>;
		using self_type = static_tuple;
	public:
		using value_type = First;

		constexpr static_tuple() noexcept = default;
		constexpr explicit(false) static_tuple(auto&& first, auto&&... rest) noexcept
			: base_type(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(rest)...)
			, _data(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(first))
		{
		}

#if (__cpp_explicit_this_parameter >= 202110L)
		constexpr auto&& _base(this auto&& self) { return static_cast<type_traits::add_cvref_like_t<base_type, decltype(self)>>(*this); }
		constexpr auto&& _value(this auto&& self) { return NAGISA_STL_FREESTANDING_UTILITY_FORWARD(self)._data; }
		constexpr auto&& _value(this self_type&& self) { return ::std::move(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(self)._data); }
		constexpr auto&& _value(this const self_type&& self) { return static_cast<::std::add_rvalue_reference_t<::std::add_const_t<value_type>>>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(self)._data); }
#else
		constexpr auto&& _base()& { return static_cast<base_type&>(*this); }
		constexpr auto&& _base() const& { return static_cast<const base_type&>(*this); }
		constexpr auto&& _base()&& { return static_cast<base_type&&>(*this); }
		constexpr auto&& _base() const&& { return static_cast<const base_type&&>(*this); }

		constexpr auto&& _value()& { return _data; }
		constexpr auto&& _value() const& { return _data; }
		constexpr auto&& _value()&& { return ::std::move(_data); }
		constexpr auto&& _value() const&& { return _data; }
#endif
	public:
		//this `public` is for `structured type` which can be passed to non-type template parameter
		value_type _data;
	};

	template<::std::size_t I>
	constexpr decltype(auto) get_impl(auto&& p) noexcept
		requires (I < tuple_size_v<::std::remove_cvref_t<decltype(p)>>)
	{
		using tuple_type = ::std::remove_cvref_t<decltype(p)>;
		if constexpr (I == tuple_size_v<tuple_type> -1)
		{
			return NAGISA_STL_FREESTANDING_UTILITY_FORWARD(p)._value();
		}
		else
		{
			return static_tuple_adl::get_impl<I>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(p)._base());
		}
	}

	template<::std::size_t I, class... Args>
	constexpr decltype(auto) get(static_tuple<Args...>& t) { return static_tuple_adl::get_impl<sizeof...(Args) - 1 - I>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(t)); }
	template<::std::size_t I, class... Args>
	constexpr decltype(auto) get(const static_tuple<Args...>& t) { return static_tuple_adl::get_impl<sizeof...(Args) - 1 - I>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(t)); }
	template<::std::size_t I, class... Args>
	constexpr decltype(auto) get(static_tuple<Args...>&& t) { return static_tuple_adl::get_impl<sizeof...(Args) - 1 - I>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(t)); }
	template<::std::size_t I, class... Args>
	constexpr decltype(auto) get(const static_tuple<Args...>&& t) { return static_tuple_adl::get_impl<sizeof...(Args) - 1 - I>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(t)); }

#if (__cpp_deduction_guides >= 201703L)
	template<class... Args>
	static_tuple(Args&&...) -> static_tuple<Args...>;
#endif
}

using static_tuple_adl::static_tuple;

template <class... Ts>
struct
#if defined(_MSC_VER) && !defined(__clang__)
	[[msvc::known_semantics]]
#endif
tuple_size<static_tuple<Ts...>>
{
	constexpr static auto value = sizeof...(Ts);
};

template <::std::size_t Index>
struct 
#if defined(_MSC_VER) && !defined(__clang__)
	[[msvc::known_semantics]]
#endif
tuple_element<Index, static_tuple<>>
{
	static_assert(concepts::none<::std::integral_constant<::std::size_t, Index>>, "tuple index out of bounds");
};

template <class This, class... Rest>
struct
#if defined(_MSC_VER) && !defined(__clang__)
	[[msvc::known_semantics]]
#endif
tuple_element<0, static_tuple<This, Rest...>>
{
	using type = This;
};

template <::std::size_t Index, class This, class... Rest>
struct 
#if defined(_MSC_VER) && !defined(__clang__)
	[[msvc::known_semantics]]
#endif
tuple_element<Index, static_tuple<This, Rest...>> : tuple_element<Index - 1, static_tuple<Rest...>> {};


NAGISA_BUILD_LIB_DETAIL_END