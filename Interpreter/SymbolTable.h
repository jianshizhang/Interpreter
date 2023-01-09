#pragma once
#include <unordered_map>
#include <string>
#include "Token.h"

class SymbolTable
{
public:
	std::unordered_map<std::string, Token> table;
	std::string ToStr();
};

