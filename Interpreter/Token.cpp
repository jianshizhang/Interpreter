#include "Token.h"

std::string Token::ToStr()
{
	std::string res;
	res += TokenTypeString[type];
	res += ", ";
	res += value;
	return res;
}

