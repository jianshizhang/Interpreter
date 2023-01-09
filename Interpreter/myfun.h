#pragma once
#include <string>
#include <sstream>
#include <iomanip>

//自己重新写的一个简单的浮点数转string的函数，可以保留10位有效数字
//to_string()对于浮点数转字符串只保留6位.
std::string DecimalToString(double decimal);