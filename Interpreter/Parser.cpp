#include "Parser.h"


std::string Parser::ToStr()
{
	std::string res;
	res += "[ ";
	for (auto& a : induction) {
		res += (a + ", ");
	}
	res += "]";
	return res;
}

AST* Parser::parser()
{
	AST* res = Stat();
	if (current_token.type != END) {
		std::stringstream out;
		out << "Last token type not END";
		throw(out.str());
	}
	return res;
}

void Parser::Eat(TokenType token_type)
{
	if (current_token.type == token_type) {
		current_token = lexer-> GetNextToken();
	}
	else {
		std::stringstream out;
		out << "Error input. Wanted: " << TokenTypeString[token_type];
		throw(out.str());
	}
}

AST* Parser::Stat()
{
	AST* res;
	if (current_token.type == LID) {
		induction.push_back("Stat -> AssignStat");
		res = AssignStat();
	}
	else {
		induction.push_back("Stat -> Exp");
		res = Exp();
	}
	return res;
}

AST* Parser::AssignStat()
{
	induction.push_back("AssignStat -> LVariable = Exp");
	AST* var = LVariable();
	Token t = current_token;
	Eat(ASSIGN);
	AST* expr = Exp();
	return new Assign(var, t, expr);
}
// Exp -> AndExp ( '||' AndExp )*
AST* Parser::Exp()
{
	induction.push_back("Exp -> AndExp");
	AST* res = AndExp();
	while (current_token.type == OR) {
		if (current_token.type == OR) {
			induction.push_back("Exp -> Exp || AndExp");
			Eat(OR);
			AST* right = AndExp();
			res = new BinOp(res, Token(OR, "||"), right);
		}
	}
	return res;
}

AST* Parser::AndExp()
{
	induction.push_back("AndExp -> XorExp");
	AST* res = XorExp();
	while (current_token.type == AND) {
		if (current_token.type == AND) {
			induction.push_back("AndExp -> AndExp && XorExp");
			Eat(AND);
			AST* right = XorExp();
			res = new BinOp(res, Token(AND, "&&"), right);
		}
	}
	return res;
}

AST* Parser::XorExp()
{
	induction.push_back("XorExp -> EqExp");
	AST* res = EqExp();
	while (current_token.type == XOR) {
		if (current_token.type == XOR) {
			induction.push_back("XorExp -> XorExp ^ EqExp");
			Eat(XOR);
			AST* right = EqExp();
			res = new BinOp(res, Token(XOR, "^"), right);
		}
	}
	return res;
}

AST* Parser::EqExp()
{
	induction.push_back("EqExp -> RelExp");
	AST* res = RelExp();
	while (current_token.type == EQUAL || current_token.type == NOTEQUAL) {
		if (current_token.type == EQUAL) {
			induction.push_back("EqExp -> EqExp == RelExp");
			Eat(EQUAL);
			AST* right = RelExp();
			res = new BinOp(res, Token(EQUAL, "=="), right);
		}
		else if (current_token.type == NOTEQUAL) {
			induction.push_back("EqExp -> EqExp != RelExp");
			Eat(NOTEQUAL);
			AST* right = RelExp();
			res = new BinOp(res, Token(NOTEQUAL, "!="), right);
		}
	}
	return res;
}

AST* Parser::RelExp()
{
	induction.push_back("RelExp -> AddExp");
	AST* res = AddExp();
	while (current_token.type == LESS
		|| current_token.type == LEQUAL
		|| current_token.type == GREATER
		|| current_token.type == GEQUAL) {
		if (current_token.type == LESS) {
			induction.push_back("RelExp -> RelExp < AddExp");
			Eat(LESS);
			AST* right = AddExp();
			res = new BinOp(res, Token(LESS, "<"), right);
		}
		else if (current_token.type == LEQUAL) {
			induction.push_back("RelExp -> RelExp <= AddExp");
			Eat(LEQUAL);
			AST* right = AddExp();
			res = new BinOp(res, Token(LEQUAL, "<="), right);
		}
		else if (current_token.type == GREATER) {
			induction.push_back("RelExp -> RelExp > AddExp");
			Eat(GREATER);
			AST* right = AddExp();
			res = new BinOp(res, Token(GREATER, ">"), right);
		}
		else if (current_token.type == GEQUAL) {
			induction.push_back("RelExp -> RelExp >= AddExp");
			Eat(GEQUAL);
			AST* right = AddExp();
			res = new BinOp(res, Token(GEQUAL, ">="), right);
		}
	}
	return res;
}

AST* Parser::AddExp()
{
	induction.push_back("AddExp -> MulExp");
	AST* res = MulExp();
	while (current_token.type == ADD
		|| current_token.type == SUB) {
		if (current_token.type == ADD) {
			induction.push_back("AddExp -> AddExp + MulExp");
			Eat(ADD);
			AST* right = MulExp();
			res = new BinOp(res, Token(ADD, "+"), right);
		}
		else if (current_token.type == SUB) {
			induction.push_back("AddExp -> AddExp - MulExp");
			Eat(SUB);
			AST* right = MulExp();
			res = new BinOp(res, Token(SUB, "-"), right);
		}
	}
	return res;
}

AST* Parser::MulExp()
{
	induction.push_back("MulExp -> UnaryExp");
	AST* res = UnaryExp();
	while (current_token.type == MUL
		|| current_token.type == DIV
		|| current_token.type == MOD) {
		if (current_token.type == MUL) {
			induction.push_back("MulExp -> MulExp * UnaryExp");
			Eat(MUL);
			AST* right = UnaryExp();
			res = new BinOp(res, Token(MUL, "*"), right);
		}
		else if (current_token.type == DIV) {
			induction.push_back("MulExp -> MulExp / UnaryExp");
			Eat(DIV);
			AST* right = UnaryExp();
			res = new BinOp(res, Token(DIV, "/"), right);
		}
		else if (current_token.type == MOD) {
			induction.push_back("MulExp -> MulExp % UnaryExp");
			Eat(MOD);
			AST* right = UnaryExp();
			res = new BinOp(res, Token(MOD, "%"), right);
		}
	}
	return res;
}

AST* Parser::UnaryExp()
{
	AST* res;
	if (current_token.type == ADD) {
		induction.push_back("UnaryExp -> + UnaryExp");
		Token token = current_token;
		Eat(ADD);
		res = new UnaryOp(token, UnaryExp());
	}
	else if (current_token.type == SUB) {
		induction.push_back("UnaryExp -> - UnaryExp");
		Token token = current_token;
		Eat(SUB);
		res = new UnaryOp(token, UnaryExp());
	}
	else if (current_token.type == NOT) {
		induction.push_back("UnaryExp -> ! UnaryExp");
		Token token = current_token;
		Eat(NOT);
		res = new UnaryOp(token, UnaryExp());
	}
	else {
		induction.push_back("UnaryExp -> PrimaryExp");
		res = PrimaryExp();
	}
	return res;
}

AST* Parser::PrimaryExp()
{
	AST* res;
	if (current_token.type == INTEGER) {
		induction.push_back("PrimaryExp -> INTEGER");
		res = new Number(current_token);
		Eat(INTEGER);
	}
	else if (current_token.type == DECIMAL) {
		induction.push_back("PrimaryExp -> DECIMAL");
		res = new Number(current_token);
		Eat(DECIMAL);
	}
	else if (current_token.type == BOOLEAN) {
		induction.push_back("PrimaryExp -> BOOLEAN");
		res = new Boolean(current_token);
		Eat(BOOLEAN);
	}
	else if (current_token.type == LPAREN) {
		Eat(LPAREN);
		induction.push_back("PrimaryExp -> ( Exp )");
		res = Exp();
		Eat(RPAREN);
	}
	else if (current_token.type == RID) {
		induction.push_back("PrimaryExp -> RVariable");
		res = RVariable();
	}
	else {
		std::stringstream out;
		out << "Input incomplete!";
		throw(out.str());
	}
	return res;
}

AST* Parser::LVariable()
{
	Token t = current_token;
	if (current_token.type == LID) {
		induction.push_back("LVariable -> LID");
		Eat(LID);
		Var* res = new Var(t);
		return res;
	}
}

AST* Parser::RVariable()
{
	Token t = current_token;
	if (current_token.type == RID) {
		induction.push_back("RVariable -> RID");
		Eat(RID);
		Var* res = new Var(t);
		return res;
	}
}

AST* Parser::Empty()
{
	return new NoOp();
}
