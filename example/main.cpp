#include <nagisa/stl_freestanding/tuple/tuple.h>

int main()
{
	using namespace ::ngs::stl_freestanding::tuples;
	using tp1 = static_tuple<int, float>;
	using tp2 = static_tuple<char, unsigned long, double>;

	static_assert(::std::same_as<tuple_element_t<0, tp1>, int>);
	static_assert(::std::same_as<tuple_element_t<1, tp1>, float>);
	static_assert(tuple_size_v<tp1> == 2);

	static_assert(::std::same_as<tuple_element_t<0, tp2>, char>);
	static_assert(::std::same_as<tuple_element_t<1, tp2>, unsigned long>);
	static_assert(::std::same_as<tuple_element_t<2, tp2>, double>);
	static_assert(tuple_size_v<tp2> == 3);

	static_assert(::std::same_as<static_tuple<int, float, char, unsigned long, double>, tuple_cat_t<tp1, tp2>>);

	return 0;
}