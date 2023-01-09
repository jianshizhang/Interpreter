#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include "myfun.h"
#include "Lexer.h"

std::string Lexer::ToStr()
{
	std::string res;
	Token token = GetNextToken();
	while (token.type != END) {
		res += (token.ToStr() + "\n");
		token = GetNextToken();
	}
	return res;
}



Token Lexer::GetNextToken()
{
	while (current_char != '\0') {
		if (!(current_char >= 0 && current_char <= 255)) {
			std::ostringstream out;
			out << "Invalid input : " << current_char << Peek() << " !";
			throw(out.str());
		}
		if (current_char == ' ') {
			SkipSpace();
			continue;
		}
		if ((current_char >= 'a' && current_char <= 'z')
			|| (current_char >= 'A' && current_char <= 'Z')
			|| (current_char == '_')) {
			return IdBoolean();
		}
		if (current_char >= '0' && current_char <= '9') {
			return Number();
		}
		if (current_char == '*') {
			Advance();
			return Token(MUL, "*");
		}
		if (current_char == '/') {
			Advance();
			return Token(DIV, "/");
		}
		if (current_char == '+') {
			Advance();
			return Token(ADD, "+");
		}
		if (current_char == '-') {
			Advance();
			return Token(SUB, "-");
		}
		if (current_char == '(') {
			Advance();
			return Token(LPAREN, "(");
		}
		if (current_char == ')') {
			Advance();
			return Token(RPAREN, ")");
		}
		if (current_char == '%') {
			Advance();
			return Token(MOD, "%");
		}
		if (current_char == '^') {
			Advance();
			return Token(XOR, "^");
		}
		if (current_char == '<' && Peek() == '=') {
			Advance(2);
			return Token(LEQUAL, "<=");
		}
		if (current_char == '<' && Peek() != '=') {
			Advance();
			return Token(LESS, "<");
		}
		if (current_char == '>' && Peek() == '=') {
			Advance(2);
			return Token(GEQUAL, ">=");
		}
		if (current_char == '>' && Peek() != '=') {
			Advance();
			return Token(GREATER, ">");
		}
		if (current_char == '=' && Peek() == '=') {
			Advance(2);
			return Token(EQUAL, "==");
		}
		if (current_char == '=' && Peek() != '=') {
			Advance();
			return Token(ASSIGN, "=");
		}
		if (current_char == '!' && Peek() == '=') {
			Advance(2);
			return Token(NOTEQUAL, "!=");
		}
		if (current_char == '!' && Peek() != '=') {
			Advance();
			return Token(NOT, "!");
		}
		if (current_char == '&' && Peek() == '&') {
			Advance(2);
			return Token(AND, "&&");
		}
		if (current_char == '|' && Peek() == '|') {
			Advance(2);
			return Token(OR, "||");
		}
		std::ostringstream out;
		out << "Invalid input : " << current_char << " !";
		throw(out.str());
	}
	return Token(END, "\0");
}

//std::string Lexer::DecimalToString(double decimal)
//{
//	std::stringstream ss;
//	ss << std::setiosflags(std::ios::fixed)  << std::setprecision(10) << decimal;
//	return ss.str();
//}

Token Lexer::Number()
{
	std::string res;
	while (current_char != '\0' && (current_char >= '0' && current_char <= '9')) {
		res += current_char;
		Advance();
	}
	if (current_char == '.') {
		res += current_char;
		Advance();
		while (current_char != '\0' && (current_char >= '0' && current_char <= '9')) {
			res += current_char;
			Advance();
		}
		return Token(DECIMAL, DecimalToString(stod(res)));
	}
	else {
		return Token(INTEGER, std::to_string(stol(res)));
	}
}

Token Lexer::IdBoolean()
{
	std::string res;
	while (current_char != '\0'
		&& ((current_char >= 'a' && current_char <= 'z')
			|| (current_char >= 'A' && current_char <= 'Z')
			|| (current_char >= '0' && current_char <= '9')
			|| (current_char == '_'))) {
		res += current_char;
		Advance();
	}
	if (res == "true" || res == "false") {
		return Token(BOOLEAN, res);
	}
	else {
		SkipSpace();
		if (current_char == '=' && Peek() != '=') {
			return Token(LID, res);
		}
		else {
			return Token(RID, res);
		}
	}
}

void Lexer::Advance(int step)
{
	pos += step;
	if (pos > text.length() - 1) {
		current_char = '\0';
	}
	else {
		current_char = text[pos];
	}
}

char Lexer::Peek()
{
	if (pos + 1 > text.length() - 1) {
		return '\0';
	}
	else {
		return text[pos + 1];
	}
}

void Lexer::SkipSpace()
{
	while (current_char != '\0' && (current_char == ' ')) {
		Advance();
	}
}
