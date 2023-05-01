#pragma once
#include <ApprovalTests.hpp>
#include <array>
#include <catch2/catch.hpp>
#include <cstdint>

template <unsigned long long N> constexpr auto factorial() { return N * factorial<N - 1>(); }

template <> constexpr auto factorial<0>() { return 1; }

TEST_CASE("factorials are computed", "[factorial]")
{
	REQUIRE(factorial<1>() == 1);
	REQUIRE(factorial<2>() == 2);
	REQUIRE(factorial<3>() == 6);
	REQUIRE(factorial<10>() == 3628800);
}
