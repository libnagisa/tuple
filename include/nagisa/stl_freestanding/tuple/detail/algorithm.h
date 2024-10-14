#pragma once

#include "./trait.h"
#include "./concept.h"
#include "./static_tuple.h"
#include "./environment.h"

NGS_BUILD_LIB_DETAIL_BEGIN


template <class T, class KxArg, class IxArg, ::std::size_t IxNext, class... Sequences>
struct tuple_cat_impl;

template <class T, ::std::size_t... Kx, ::std::size_t... Ix, ::std::size_t IxNext>
struct tuple_cat_impl<T, ::std::index_sequence<Kx...>, ::std::index_sequence<Ix...>, IxNext>
{
	using return_type = static_tuple<tuple_element_t<Kx, ::std::remove_cvref_t<tuple_element_t<Ix, T>>>...>;
	using kx_sequence_type = ::std::index_sequence<Kx...>;
	using ix_sequence_type = ::std::index_sequence<Ix...>;
};

template <class T, ::std::size_t... Kx, ::std::size_t... Ix, ::std::size_t IxNext, ::std::size_t... KxNext, class... Rest>
struct tuple_cat_impl<T, ::std::index_sequence<Kx...>, ::std::index_sequence<Ix...>, IxNext, ::std::index_sequence<KxNext...>, Rest...>
	: tuple_cat_impl<T, ::std::index_sequence<Kx..., KxNext...>, ::std::index_sequence<Ix..., (IxNext + 0 * KxNext)...>, IxNext + 1, Rest...>
{};

template <tuple_like... Tuples>
using tuple_cat_t = typename tuple_cat_impl<
	static_tuple<Tuples...>
	, ::std::index_sequence<>
	, ::std::index_sequence<>
	, 0
	, ::std::make_index_sequence<tuple_size_v<::std::remove_cvref_t<Tuples>>>...
>::return_type;


NGS_BUILD_LIB_DETAIL_END