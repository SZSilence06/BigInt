#ifndef SZSILENCE06_BASE_TRAITS_H
#define SZSILENCE06_BASE_TRAITS_H

namespace szsilence06
{
	template<int Base>
	struct BitPerDigit
	{
		enum { value = 4 };
	};

	//specilization
	template<>
	struct BitPerDigit<2>
	{
		enum { value = 1 };
	};

	template<>
	struct BitPerDigit<3>
	{
		enum { value = 2 };
	};

	template<>
	struct BitPerDigit<4>
	{
		enum { value = 2 };
	};

	template<int Base>
	struct BaseTrait
	{
		static const int bitPerDigit = BitPerDigit<Base>::value;
		static const int digitPerByte = 8 / bitPerDigit;
		static const unsigned char mask = 255 >> (8 - bitPerDigit);
	};
}

#endif