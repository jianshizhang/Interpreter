#pragma once
#include <string>
#include <vector>

#include "Token.h"

class Lexer
{
public:
	Lexer(std::string t) :text{ t }, pos{ 0 }, current_char{ text[pos] }{}
	Token GetNextToken();
	std::string ToStr();
private:
	std::string text;
	unsigned long pos;
	char current_char;

	Token Number();
	Token IdBoolean();
	void Advance(int step = 1);
	char Peek();
	void SkipSpace();
};

