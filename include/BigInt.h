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
			_init(number);
		}

		BigIntBase(long long number) : BigIntBase()
		{
			_init(number);
		}

		BigIntBase& operator=(long long number)
		{
			_init(number);
			return *this;
		}

		BigIntBase& operator= (const std::string& number)
		{
			_init(number);
			return *this;
		}

		std::string toString() const
		{
			std::stringstream ss;
			if (_sign < 0)
			{
				ss << '-';
			}
			for (int i = (int)(_digits - 1); i >= 0; i--)
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
			if (*this < 0)
				return rhs - (-*this);
			if (rhs < 0)
				return *this - (-rhs);

			size_t maxDigit = std::max(_digits, rhs._digits);
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
			if (_sign < 0)
				return -rhs - (-*this);
			if (*this < rhs)
				return -(rhs - *this);

			size_t maxDigit = std::max(_digits, rhs._digits);
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

		BigIntBase operator *(const BigIntBase& rhs) const
		{
			BigIntBase result;
			result._digits = _digits + rhs._digits;
			result._sign = _sign * rhs._sign;
			result._data.resize(_getBytesCount(result._digits));

			Byte carry = 0;
			for (size_t i = 0; i < _digits; i++)
			{
				for (size_t j = 0; j < rhs._digits; j++)
				{
					Byte product = _getDigit(i) * rhs._getDigit(j) + carry;
					carry = product / Base;
					product %= Base;
					Byte temp = result._getDigit(i + j) + product;
					carry += temp / Base;
					temp %= Base;
					result._setDigit(i + j, temp);
				}
				result._setDigit(i + rhs._digits, result._getDigit(i + rhs._digits) + carry);
				carry = 0;
			}
			result._setDigit(result._digits - 1, result._getDigit(result._digits - 1) + carry);
			carry = 0;
			result._correct();
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
		void _init(const std::string& number)
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

			int digit = (int)(_digits - 1);
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

		void _init(long long number)
		{
			if (number > 0)
				_sign = 1;
			else if (number == 0)
				_sign = 0;
			else
			{
				_sign = -1;
				number = -number;
			}

			_digits = 0;
			if (number == 0)
			{
				_digits = 1;
				_data.resize(1);
				_setDigit(0, 0);
				return;
			}

			long long another = number;
			while (another)
			{
				another /= Base;
				_digits++;
			}

			_data.resize(_getBytesCount(_digits));
			another = number;
			int i = 0;
			while (another)
			{
				_setDigit(i, another % Base);
				another /= Base;
				i++;
			}
		}

		Byte _getDigit(size_t digit) const
		{
			size_t index = digit / BaseTrait<Base>::digitPerByte;
			size_t posInByte = digit % BaseTrait<Base>::digitPerByte;
			Byte c = _data[index];
			c >>= (posInByte * BaseTrait<Base>::bitPerDigit);
			c &= BaseTrait<Base>::mask;
			return c;
		}

		void _setDigit(size_t digit, Byte value)
		{
			size_t index = digit / BaseTrait<Base>::digitPerByte;
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
			for (int i = (int)(_digits - 1); i >= 0; i--)
			{
				if (_getDigit(i) != 0)
					break;
				count_zeros_highbit++;
			}
			_digits -= count_zeros_highbit;
			size_t bytes = _getBytesCount(_digits);
			for (size_t i = bytes; i < _data.size(); i++)
			{
				_data.erase(_data.begin() + i);
			}
			if (_digits == 0)
				_sign = 0;
		}

		static size_t _getBytesCount(size_t digitCount)
		{
			return (size_t)(ceil((float)digitCount / BaseTrait<Base>::digitPerByte));
		}

	private:
		std::vector<Byte> _data;
		size_t _digits = 0;
		char _sign = 0;
		bool _isValid = true;
	};

	using BigInt = BigIntBase<10>;
	using BigInt16 = BigIntBase<16>;
}

#endif
