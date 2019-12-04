#include "tuple.hpp"
#include <iostream>
#include <tuple>

int main()
{
	Tuple<int, double, char> myTuple(1, 2.0, 'a');
	//std::cout << Get<1>(myTuple) << std::endl;
	//std::cout << Get<int>(myTuple) << std::endl;
	//std::cout << Get<float>(myTuple) << std::endl;
	//std::cout << get<double>(myTuple) << std::endl;
	//std::cout << get<char>(myTuple) << std::endl;
//	std::cout << get<float>(myTuple) << std::endl;
	std::cout << get<1>(myTuple) << std::endl;
	return 0;
}