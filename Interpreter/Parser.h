#pragma once
#include <sstream>
#include <string>
#include "Token.h"
#include "Lexer.h"

class AST {
public:
	AST() = default;
	virtual std::string GetId() = 0;
};


class Assign : public AST {
public:
	Assign(AST* l, Token t, AST* r) : token(t), left(l), right(r) {}
	Token token;
	AST* left, * right;
	std::string GetId() { return "Assign"; }
};


class BinOp : public AST {
public:
	BinOp(AST* l, Token t, AST* r) : token(t), left(l), right(r) {}
	Token token;
	AST* left, * right;
	std::string GetId() { return "BinOp"; }
};


class UnaryOp : public AST {
public:
	UnaryOp(Token t, AST* child) : token(t), child(child) {}
	Token token;
	AST* child;
	std::string GetId() { return "UnaryOp"; }
};


class Number : public AST {
public:
	Number(Token t) : token(t) {}
	Token token;
	std::string GetId() { return "Number"; }
};


class Boolean : public AST {
public:
	Boolean(Token t) : token(t) {}
	Token token;
	std::string GetId() { return "Boolean"; }
};


class Var : public AST {
public:
	Var(Token t) : token(t), val(t.value) {}
	Token token;
	std::string val;
	std::string GetId() { return "Var"; }
};


class NoOp : public AST {
public:
	NoOp() = default;
	std::string GetId() { return "NoOp"; }
};


class Parser
{
public:
	Parser(Lexer* le) : lexer{ le }, current_token{ lexer->GetNextToken() } {}
	Lexer* lexer;
	Token  current_token;

	AST* parser();
	std::string ToStr();
	std::vector<std::string> induction;
private:
	
	void  Eat(TokenType token_type);
	AST* Stat();
	AST* AssignStat();
	AST* Exp();
	AST* AndExp();
	AST* XorExp();
	AST* EqExp();
	AST* RelExp();
	AST* AddExp();
	AST* MulExp();
	AST* UnaryExp();
	AST* PrimaryExp();
	AST* LVariable();
	AST* RVariable();
	AST* Empty();
};

