#define CATCH_CONFIG_MAIN

#include <BigInt.h>
#include <iostream>
#include "catch.hpp"

using namespace szsilence06;

TEST_CASE("add and sub", "[add and sub]")
{
	BigInt a;
	BigInt b;

	SECTION("Simple case")
	{
		a = 0;
		REQUIRE(a.toString() == "0");

		a = 12;
		b = 10;
		REQUIRE((a - b).toString() == "2");

		a = "21390909023810543675868575343";
		b = "3914289857865008977206457942289";
		REQUIRE((a + b).toString() == "3935680766888819520882326517632");
		REQUIRE((a - b).toString() == "-3892898948841198433530589366946");
	}

	SECTION("zero result")
	{
		a = 100;
		REQUIRE((a - 100).toString() == "0");
	}

	SECTION("Adding and subing negative numbers")
	{
		a = -100;
		b = 428;
		REQUIRE((a + b).toString() == "328");
		REQUIRE((a - 25555).toString() == "-25655");

		b = -428;
		REQUIRE((a + b).toString() == "-528");
		REQUIRE((a - b).toString() == "328");

		a = 100;
		REQUIRE((a + b).toString() == "-328");
		REQUIRE((a - b).toString() == "528");

		a = 0;
		b = -1;
		REQUIRE((a + b).toString() == "-1");
		REQUIRE((a - b).toString() == "1");
	}
}

TEST_CASE("mul", "[multiply]")
{
	BigInt a, b;

	SECTION("Simple case")
	{
		a = 10;
		b = 15;
		REQUIRE((a*b).toString() == "150");

		a = "128991324758789870647908267676";
		b = -107976;
		REQUIRE((a*b).toString() == "-13927967282155095073078543110583776");

		a = 172;
		b = 14;
		REQUIRE((a*b).toString() == "2408");

		a = 0;
		b = -12;
		REQUIRE((a*b).toString() == "0");
	}
}

TEST_CASE("div", "[divide]")
{
	BigInt a, b;

	SECTION("Simple case")
	{
		a = 10;
		b = 15;
		REQUIRE((a / b).toString() == "0");

		a = "128";
		b = -10;
		REQUIRE((a / b).toString() == "-12");

		a = 0;
		b = -12;
		REQUIRE((a / b).toString() == "0");

		a = 528;
		b = 22;
		REQUIRE((a / b).toString() == "24");

		a = "21374877698635581831263128830768679898";
		b = 32728749094207;
		REQUIRE((a / b).toString() == "653091801251241301975441");

		a = 2225;
		b = -22222;
		REQUIRE((a / b).toString() == "0");
	}
}