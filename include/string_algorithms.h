#ifndef SZSILENCE06_STRING_ALGORITHMS_H
#define SZSILENCE06_STRING_ALGORITHMS_H

#include <string>
#include <algorithm>
#include <functional>
#include <cctype>

namespace szsilence06
{
	// trim from start
	static inline std::string &trim_left(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	static inline std::string &trim_right(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	// trim from both ends
	static inline std::string &trim(std::string &s)
	{
		return trim_left(trim_right(s));
	}
}

#endif
