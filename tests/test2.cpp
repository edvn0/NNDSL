#pragma once

#include <catch2/catch.hpp>
template <std::size_t N> constexpr auto factorial2() { return N * factorial2<N - 1>(); }

template <> constexpr auto factorial2<0>() { return 1; }

TEST_CASE("factorials are computed again", "[factorial2]")
{
	REQUIRE(factorial2<1>() == 1);
	REQUIRE(factorial2<2>() == 2);
	REQUIRE(factorial2<3>() == 6);
	REQUIRE(factorial2<10>() == 3628800);
}
