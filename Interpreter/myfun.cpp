#include "myfun.h"

std::string DecimalToString(double decimal)
{
	std::stringstream ss;
	ss << std::setprecision(10) << decimal;
	return ss.str();
}
