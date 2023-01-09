#pragma once
#include <vector>
#include <string>
#include "Parser.h"
#include "SymbolTable.h"


class Interpreter 
{
public:
	Interpreter(Parser* par, SymbolTable* s) { parser = par; symbol_table = s; }
	Parser* parser;
	SymbolTable* symbol_table;

	std::string interpreter(std::string mode);
	std::string ToStr();
	std::vector<std::string> expression;
private:

	Token VisitNode(AST* node);
	Token VisitNumber(Number* node);
	Token VisitBoolean(Boolean* node);
	Token VisitVar(Var* node);
	Token VisitBinOp(BinOp* node);
	Token VisitUnaryOp(UnaryOp* node);
	Token VisitAssign(Assign* node);
	Token VisitNoOp(NoOp* node);
};

