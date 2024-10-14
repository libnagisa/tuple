#pragma once

#include "./template.h"
#include "./trait.h"
#include "./environment.h"

NAGISA_BUILD_LIB_DETAIL_BEGIN

template<class Tuple, class = ::std::make_index_sequence<tuple_size_v<Tuple>>>
struct check_tuple_args : ::std::false_type {};

template<class Tuple, ::std::size_t... I>
struct check_tuple_args<Tuple, ::std::index_sequence<I...>>
	: ::std::bool_constant<
	((::std::same_as<::std::add_lvalue_reference_t<tuple_element_t<I, Tuple>>&&, ::std::invoke_result_t<decltype(get<I>), ::std::add_lvalue_reference_t<Tuple>>>) && ...)
	> {
};

template<class T>
concept tuple_like = check_tuple_args<::std::remove_cvref_t<T>>::value;

template<class T>
concept pair_like = tuple_like<T> && (tuple_size_v<T> == 2);

NAGISA_BUILD_LIB_DETAIL_END