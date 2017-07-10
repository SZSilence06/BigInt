#include "catch.hpp"
#include <BigInt.h>

using namespace szsilence06;

TEST_CASE("add and sub (16)", "[add and sub]")
{
	BigInt16 a;
	BigInt16 b;

	SECTION("Simple case")
	{
		a = 0;
		REQUIRE(a.toString() == "0");

		a = 22;
		b = "-01b";
		REQUIRE((a - b).toString() == "31");

		a = "2297dc6e9d118c6a3EFb9998DF2C6B";
		b = "153cc6899ABA70279DE3652F4D6A8B52C";
		REQUIRE((a + b).toString() == "153EF00761A44140648754E8E6F87E197");
		REQUIRE((a - b).toString() == "-153A9D0BD3D09F0ED73F7575B3DC988C1");
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
		REQUIRE((a + b).toString() == "148");
		REQUIRE((a - 25555).toString() == "-6437");

		b = -428;
		REQUIRE((a + b).toString() == "-210");
		REQUIRE((a - b).toString() == "148");

		a = 100;
		REQUIRE((a + b).toString() == "-148");
		REQUIRE((a - b).toString() == "210");

		a = 0;
		b = -1;
		REQUIRE((a + b).toString() == "-1");
		REQUIRE((a - b).toString() == "1");
	}
}

TEST_CASE("mul (16)", "[multiply]")
{
	BigInt16 a, b;

	SECTION("Simple case")
	{
		a = 10;
		b = 15;
		REQUIRE((a*b).toString() == "96");

		a = "2297dc6e9d118c6a3EFb9998DF2C6B";
		b = "153cc6899ABA70279DE3652F4D6A8B52C";
		REQUIRE((a*b).toString() == "2DEAB80C97A5167BED33B71FC7C9882B42822C1D964870A6D0550EA8FB4964");

		a = 172;
		b = 14;
		REQUIRE((a*b).toString() == "968");

		a = 0;
		b = -12;
		REQUIRE((a*b).toString() == "0");
	}
}

TEST_CASE("div (16)", "[divide]")
{
	BigInt16 a, b;

	SECTION("Simple case")
	{
		a = 10;
		b = 15;
		REQUIRE((a / b).toString() == "0");

		a = "1C8";
		b = -10;
		REQUIRE((a / b).toString() == "-2D");

		a = 0;
		b = -12;
		REQUIRE((a / b).toString() == "0");

		a = 528;
		b = 22;
		REQUIRE((a / b).toString() == "18");

		a = "153cc6899ABA70279DE3652F4D6A8B52C";
		b = "2297dc6e9d118c6a3EFb9998DF2C6B";
		REQUIRE((a / b).toString() == "9D2");

		a = 2225;
		b = -22222;
		REQUIRE((a / b).toString() == "0");
	}

	SECTION("remainder (16)")
	{
		a = 10;
		b = 15;
		REQUIRE((a % b).toString() == "A");

		a = "128";
		b = -10;
		REQUIRE((a % b).toString() == "6");

		a = 0;
		b = -12;
		REQUIRE((a % b).toString() == "0");

		a = 528;
		b = 22;
		REQUIRE((a % b).toString() == "0");

		a = "153cc6899ABA70279DE3652F4D6A8B52C";
		b = "2297dc6e9d118c6a3EFb9998DF2C6B";
		REQUIRE((a % b).toString() == "1515E36930AD8E7FB3889597068266");

		a = 2225;
		b = -22222;
		REQUIRE((a % b).toString() == "8B1");
	}
}

TEST_CASE("comparison (16)")
{
	BigInt16 a, b;

	SECTION("equal(==) and no-equal(!=)")
	{
		a = 352;
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
		REQUIRE(a < b);

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

TEST_CASE("conversion to integers (16)")
{
	BigInt16 a, b;

	a = "-0000";
	REQUIRE(a.toInt32() == 0);

	a = -6728;
	REQUIRE(a.toInt32() == -6728);

	a = 5529;
	b = -5529;
	REQUIRE((a + b).toInt32() == 0);

	a = "3B5";
	REQUIRE(a.toInt32() == 949);
}

TEST_CASE("error handling (16)")
{
	BigInt16 a = "0x123445";
	REQUIRE(a.isValid() == false);
}