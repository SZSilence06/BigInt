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

	SECTION("remainder")
	{
		a = 10;
		b = 15;
		REQUIRE((a % b).toString() == "10");

		a = "128";
		b = -10;
		REQUIRE((a % b).toString() == "8");

		a = 0;
		b = -12;
		REQUIRE((a % b).toString() == "0");

		a = 528;
		b = 22;
		REQUIRE((a % b).toString() == "0");

		a = "21374877698635581831263128830768679898";
		b = 32728749094207;
		REQUIRE((a % b).toString() == "26842259309611");

		a = 2225;
		b = -22222;
		REQUIRE((a % b).toString() == "2225");
	}
}

TEST_CASE("comparison")
{
	BigInt a, b;

	SECTION("equal(==) and no-equal(!=)")
	{
		a = 160;
		b = "000160";
		REQUIRE(a == b);

		a = "0";
		b = "-0";
		REQUIRE(a == b);

		a = "016000";
		b = "017000";
		REQUIRE(a != b);
	}

	SECTION("greater(>) and less(<)")
	{
		a = 160;
		b = "000120";
		REQUIRE(a > b);

		a = "0";
		b = "-0";
		REQUIRE(((a > b) == false && (a < b) == false));

		a = "016000";
		b = "-017000";
		REQUIRE(b < a);

		a = "-12598";
		REQUIRE(a < 0);
	}
}

TEST_CASE("conversion to integers")
{
	BigInt a, b;

	a = "-0000";
	REQUIRE(a.toInt32() == 0);

	a = -6728;
	REQUIRE(a.toInt32() == -6728);

	a = 5529;
	b = -5529;
	REQUIRE((a + b).toInt32() == 0);
}

TEST_CASE("error handling")
{
	BigInt a = "0x123445";
	REQUIRE(a.isValid() == false);
}

TEST_CASE("converting to ohter bases")
{
	BigInt a = 1288343654;
	BigInt16 hex = a.toBase<16>();
	REQUIRE(hex.toString() == "4CCA9066");

	BigInt2 binary = a.toBase<2>();
	REQUIRE(binary.toString() == "1001100110010101001000001100110");

	BigInt8 oct = a.toBase<8>();
	REQUIRE(oct.toString() == "11462510146");
}