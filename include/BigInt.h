#ifndef SZSILENCE06_BIG_INT_H
#define SZSILENCE06_BIG_INT_H

#include "string_algorithms.h"
#include "base_traits.h"
#include <vector>
#include <sstream>

namespace szsilence06
{
	template<int Base>
	class BigIntBase
	{
	public:
		using Byte = unsigned char;

		static const int MAX_BASE = 16;
		static const Byte INVALID_VALUE = 255;

	public:
		BigIntBase()
		{
			static_assert(Base >= 2 && Base <= 16, "Not supported Base.");
		}

		BigIntBase(const std::string& number) : BigIntBase()
		{
			std::string temp = number;
			trim(temp);

			_digits = temp.length();
			_data.resize(ceil((float)_digits / BaseTrait<Base>::digitPerByte));

			int index = 0;
			if (temp[0] == '-')
			{
				_sign = -1;
				index++;
				_digits--;
			}

			int digit = _digits - 1;
			for (; index < number.length(); index++)
			{
				Byte value = _getValue(number[index]);
				if (value == INVALID_VALUE || value >= Base)
				{
					_isValid = false;
					return;
				}
				_setDigit(digit, value);
				digit--;
			}
		}

		std::string toString() const
		{
			std::stringstream ss;
			if (_sign < 0)
			{
				ss << '-';
			}
			for (int i = _digits - 1; i >= 0; i--)
			{
				ss << _getChar(_getDigit(i));
			}
			return ss.str();
		}

		//operators
		BigIntBase operator+ (const BigIntBase& rhs)
		{
			int maxDigit = std::max(_digits, rhs._digits);
			bool hasCarry = false;

			BigIntBase result;
			result._digits = maxDigit + 1;
			result._data.resize(ceil((float)result._digits / BaseTrait<Base>::digitPerByte));
			for (int i = 0; i < maxDigit; i++)
			{
				Byte a = i < _digits ? _getDigit(i) : 0;
				Byte b = i < rhs._digits ? rhs._getDigit(i) : 0;
				Byte temp = a + b;
				if (hasCarry)
					temp++;
				if (temp >= Base)
				{
					temp -= Base;
					hasCarry = true;
				}
				else
				{
					hasCarry = false;
				}
				result._setDigit(i, temp);
			}
			if (hasCarry)
			{
				result._setDigit(result._digits - 1, 1);
			}
			else
			{
				result._digits--;
				if (result._data[result._data.size() - 1] == 0)
					result._data.erase(result._data.end() - 1);
			}
			return result;
		}

		BigIntBase operator- (const BigIntBase& rhs)
		{
			int maxDigit = std::max(_digits, rhs._digits);
			bool hasCarry = false;

			BigIntBase result;
			result._digits = maxDigit;
			result._data.resize(ceil((float)result._digits / BaseTrait<Base>::digitPerByte));
			for (int i = 0; i < maxDigit; i++)
			{
				Byte a = i < _digits ? _getDigit(i) : 0;
				Byte b = i < rhs._digits ? rhs._getDigit(i) : 0;
				Byte temp = a - b;
				if (hasCarry)
					temp--;
				if (temp < 0)
				{
					temp += Base;
					hasCarry = true;
				}
				else
				{
					hasCarry = false;
				}
				result._setDigit(i, temp);
			}
			if (hasCarry)
			{
				result._setDigit(result._digits - 1, 1);
			}
			else
			{
				result._digits--;
				if (result._data[result._data.size() - 1] == 0)
					result._data.erase(result._data.end() - 1);
			}
			return result;
		}

	private:
		Byte _getDigit(int digit) const
		{
			int index = digit / BaseTrait<Base>::digitPerByte;
			int posInByte = digit % BaseTrait<Base>::digitPerByte;
			Byte c = _data[index];
			c >>= (posInByte * BaseTrait<Base>::bitPerDigit);
			c &= BaseTrait<Base>::mask;
			return c;
		}

		void _setDigit(int digit, Byte value)
		{
			int index = digit / BaseTrait<Base>::digitPerByte;
			int posInByte = digit % BaseTrait<Base>::digitPerByte;
			Byte valueToSet = value << (posInByte * BaseTrait<Base>::bitPerDigit);
			Byte clearMask = ~(BaseTrait<Base>::mask << (posInByte * BaseTrait<Base>::bitPerDigit));
			_data[index] &= clearMask;
			_data[index] |= valueToSet;
		}

		Byte _getValue(char c) const
		{
			if (_isNumber(c))
				return c - '0';

			if (_isLowerCase(c))
				return c - 'a';

			if (_isUpperCase(c))
			{
				return c - 'A';
			}

			return INVALID_VALUE;
		}

		char _getChar(Byte value) const
		{
			if (value < 10)
			{
				return '0' + value;
			}
			return 'a' + value - 10;
		}

		bool _isNumber(char c) const
		{
			return c >= '0' && c <= '9';
		}

		bool _isAlphabet(char c) const
		{
			return _isUpperCase(c) || _isLowerCase(c);
		}

		bool _isUpperCase(char c) const
		{
			return (c >= 'A' && c <= 'Z');
		}

		bool _isLowerCase(char c) const
		{
			return (c >= 'a' && c <= 'z');
		}

	private:
		std::vector<Byte> _data;
		int _digits = 0;
		char _sign = 0;
		bool _isValid = true;
	};

	using BigInt = BigIntBase<10>;
	using BigInt16 = BigIntBase<16>;
}

#endif
