#pragma once

#include <nagisa/type_trait/type_trait.h>
#include <nagisa/concept/concept.h>
#include <nagisa/stl_freestanding/utility/utility.h>
#include <concepts>
#if !defined(__cpp_lib_freestanding_utility) && !__has_include(<utility>)
#	error "This library requires <utility> to be available."
#else
#	include <utility>
#endif
//
//

#define NAGISA_BUILD_LIB_NAME NAGISA_NS::stl_freestanding::tuples
#define NAGISA_BUILD_LIB_CONFIG_VERSION (0,0,0)
#include <nagisa/build_lib/construct.h>