#pragma once

#include "./detail/trait.h"
#include "./detail/concept.h"
#include "./detail/algorithm.h"
#include "./detail/static_tuple.h"
#include "./environment.h"
//
NAGISA_BUILD_LIB_BEGIN

// trait
using details::tuple_element;
using details::tuple_element_t;
using details::tuple_size;
using details::tuple_size_v;
using details::get;

// concept
using details::tuple_like;
using details::pair_like;

// entry
using details::static_tuple;

// algorithm
using details::tuple_cat_t;

NAGISA_BUILD_LIB_END