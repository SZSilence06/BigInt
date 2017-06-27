#include <BigInt.h>
#include <iostream>

using namespace szsilence06;

int main()
{
	BigInt a = "12";
	BigInt b = "5212331232312132213312324499";
	std::cout << (a + b).toString() << std::endl;
	system("pause");
	return 0;
}