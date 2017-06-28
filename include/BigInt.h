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
		static const int ABS_GREATER = 1;
		static const int ABS_LESS = -1;
		static const int ABS_EQUAL = 0;

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
			_data.resize(_getBytesCount(_digits));

			size_t index = 0;
			if (temp[0] == '-')
			{
				_sign = -1;
				index++;
				_digits--;
			}
			else
				_sign = 1;

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
			_correct();
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
			if (_sign == 0)
				ss << 0;
			return ss.str();
		}

		//operators
		BigIntBase operator+ (const BigIntBase& rhs) const
		{
			int maxDigit = std::max(_digits, rhs._digits);
			bool hasCarry = false;

			BigIntBase result;
			result._sign = 1;
			result._digits = maxDigit + 1;
			result._data.resize(_getBytesCount(result._digits));
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

		BigIntBase operator- (const BigIntBase& rhs) const
		{
			if (rhs._sign == 0)
				return *this;
			if (_sign > rhs._sign)
				return *this + (-rhs);
			if (_sign < rhs._sign)
				return -(-*this + rhs);
			if (*this < rhs)
				return -(rhs - *this);

			int maxDigit = std::max(_digits, rhs._digits);
			bool hasCarry = false;

			BigIntBase result;
			result._sign = 1;
			result._digits = maxDigit;
			result._data.resize(_getBytesCount(result._digits));
			for (int i = 0; i < maxDigit; i++)
			{
				char a = i < _digits ? _getDigit(i) : 0;
				char b = i < rhs._digits ? rhs._getDigit(i) : 0;
				char temp = a - b;
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
			//remove zeros in hight bits
			result._correct();
			return result;
		}

		BigIntBase operator -() const
		{
			BigIntBase result = *this;
			result._sign = -result._sign;
			return result;
		}

		bool operator <(const BigIntBase& rhs) const
		{
			if (_sign != rhs._sign)
				return _sign < rhs._sign;

			bool abs_less = _absCompare(rhs) == ABS_LESS;
			return _sign < 0 ? !abs_less : abs_less;
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

		Byte _getValue(char c)
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

		static char _getChar(Byte value)
		{
			if (value < 10)
			{
				return '0' + value;
			}
			return 'a' + value - 10;
		}

		static bool _isNumber(char c)
		{
			return c >= '0' && c <= '9';
		}

		static bool _isAlphabet(char c)
		{
			return _isUpperCase(c) || _isLowerCase(c);
		}

		static bool _isUpperCase(char c)
		{
			return (c >= 'A' && c <= 'Z');
		}

		static bool _isLowerCase(char c)
		{
			return (c >= 'a' && c <= 'z');
		}

		int _absCompare(const BigIntBase& rhs) const
		{
			if (_digits != rhs._digits)
				return this->_digits > rhs._digits ? ABS_GREATER : ABS_LESS;

			for (int i = 0; i < _digits; i++)
			{
				Byte a = _getDigit(i);
				Byte b = rhs._getDigit(i);
				if (a != b)
					return a > b ? ABS_GREATER : ABS_LESS;
			}
			return ABS_EQUAL;
		}

		//Remove zeros in high bits
		void _correct()
		{
			int count_zeros_highbit = 0;
			for (int i = _digits - 1; i >= 0; i--)
			{
				if (_getDigit(i) != 0)
					break;
				count_zeros_highbit++;
			}
			_digits -= count_zeros_highbit;
			int bytes = _getBytesCount(_digits);
			for (int i = bytes; i < _data.size(); i++)
			{
				_data.erase(_data.begin() + i);
			}
			if (_digits == 0)
				_sign = 0;
		}

		static size_t _getBytesCount(int digitCount)
		{
			return (size_t)(ceil((float)digitCount / BaseTrait<Base>::digitPerByte));
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
