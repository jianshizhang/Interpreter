#pragma once
#include <string>

enum TokenType {
	INTEGER, DECIMAL, BOOLEAN, LID, RID, END,
	LPAREN, RPAREN,
	ADD, SUB, MUL, DIV, MOD,
	LESS, GREATER, LEQUAL, GEQUAL, EQUAL, NOTEQUAL,
	AND, OR, NOT, XOR,
	ASSIGN
};

static const std::string TokenTypeString[] =
{ "INTEGER", "DECIMAL", "BOOLEAN", "LID", "RID", "END",
  "LPAREN", "RPAREN", "ADD", "SUB", "MUL", "DIV", "MOD",
  "LESS", "GREATER", "LEQUAL", "GEQUAL", "EQUAL", "NOTEQUAL",
  "AND", "OR", "NOT", "XOR", "ASSIGN" };

class Token
{
public:
	Token() = default;
	Token(TokenType t, std::string v) :type{ t }, value{ v }{}

	TokenType type;
	std::string value;

	std::string ToStr();
};

