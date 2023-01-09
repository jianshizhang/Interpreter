#include <iostream>
#include <sstream>
#include <iomanip>
#include "myfun.h"
#include "Interpreter.h"

std::string Interpreter::interpreter(std::string mode)
{
	AST* node = parser->parser();
	if (mode == "$default") {
		return VisitNode(node).value;
	}
	else if (mode == "$type") {
		std::stringstream out;
		out << VisitNode(node).ToStr();
		return out.str();
	}
	else if (mode == "$induction") {
		std::stringstream out;
		out << VisitNode(node).ToStr() << "\n";
		out << parser->ToStr();
		return out.str();
	}
	else if (mode == "$expression") {
		std::stringstream out;
		out << VisitNode(node).ToStr() << "\n";
		out << ToStr();
		return out.str();
	}
}

std::string Interpreter::ToStr()
{
	std::string res;
	for (auto& a : expression) {
		res += (a + ", ");
	}
	return res;
}

Token Interpreter::VisitNode(AST* node)
{
	std::string type = node->GetId();
	if (type == "Number") {
		return VisitNumber(static_cast<Number*>(node));
	}
	else if (type == "Boolean") {
		return VisitBoolean(static_cast<Boolean*>(node));
	}
	else if (type == "UnaryOp") {
		return VisitUnaryOp(static_cast<UnaryOp*>(node));
	}
	else if (type == "BinOp") {
		return VisitBinOp(static_cast<BinOp*>(node));
	}
	else if (type == "Assign") {
		return VisitAssign(static_cast<Assign*>(node));
	}
	else if (type == "Var") {
		return VisitVar(static_cast<Var*>(node));
	}
	else if (type == "NoOp") {
		return VisitNoOp(static_cast<NoOp*>(node));
	}
	else {
		std::stringstream out;
		out << "Invalid node type";
		throw(out.str());
	}
}

Token Interpreter::VisitNumber(Number* node)
{
	return node->token;
}

Token Interpreter::VisitBoolean(Boolean* node)
{
	return node->token;
}

Token Interpreter::VisitVar(Var* node)
{
	if (node->token.type == LID) {
		std::string name = node->val;
		symbol_table->table[name] = Token(END, "\0");
		return node->token;
	}
	else if (node->token.type == RID) {
		std::string variable = node->val;
		if (symbol_table->table.find(variable) == symbol_table->table.end()) {
			std::stringstream out;
			out << variable << " not found!";
			throw(out.str());
		}
		else {
			expression.push_back(variable + " = " + symbol_table->table[variable].value);
			return symbol_table->table[variable];
		}
	}
}

Token Interpreter::VisitBinOp(BinOp* node)
{
	Token left(VisitNode(node->left));
	Token right(VisitNode(node->right));
	switch ((node->token).type) {
	case ADD:
		if (left.type == INTEGER && right.type == INTEGER) {
			std::string res = std::to_string(stoi(left.value) + stoi(right.value));
			expression.push_back(res + " = " + left.value + " + " + right.value);
			return Token(INTEGER, res);
			break;
		}
		else if ((left.type == DECIMAL && right.type == DECIMAL)
			  || (left.type == INTEGER && right.type == DECIMAL)
			  || (left.type == DECIMAL && right.type == INTEGER)) {
			std::string res = DecimalToString(stod(left.value) + stod(right.value));
			expression.push_back(res + " = " + left.value + " + " + right.value);
			return Token(DECIMAL, res);
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid ADD type !";
			throw(out.str());
		}
	case SUB:
		if (left.type == INTEGER && right.type == INTEGER) {
			std::string res = std::to_string(stoi(left.value) - stoi(right.value));
			expression.push_back(res + " = " + left.value + " - " + right.value);
			return Token(INTEGER, res);
			break;
		}
		else if ((left.type == DECIMAL && right.type == DECIMAL)
			|| (left.type == INTEGER && right.type == DECIMAL)
			|| (left.type == DECIMAL && right.type == INTEGER)) {
			std::string res = DecimalToString(stod(left.value) - stod(right.value));
			expression.push_back(res + " = " + left.value + " - " + right.value);
			return Token(DECIMAL, res);
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid SUB type !";
			throw(out.str());
		}
	case MUL:
		if (left.type == INTEGER && right.type == INTEGER) {
			std::string res = std::to_string(stoi(left.value) * stoi(right.value));
			expression.push_back(res + " = " + left.value + " * " + right.value);
			return Token(INTEGER, res);
			break;
		}
		else if ((left.type == DECIMAL && right.type == DECIMAL)
			|| (left.type == INTEGER && right.type == DECIMAL)
			|| (left.type == DECIMAL && right.type == INTEGER)) {
			std::string res = DecimalToString(stod(left.value) * stod(right.value));
			expression.push_back(res + " = " + left.value + " * " + right.value);
			return Token(DECIMAL, res);
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid MUL type !";
			throw(out.str());
		}
	case DIV:
		//除数不能为0
		if (left.type == INTEGER && right.type == INTEGER) {
			if (stoi(right.value) == 0) {
				std::stringstream out;
				out << "Divide by zero !";
				throw(out.str());
			}
			std::string res = std::to_string(stoi(left.value) / stoi(right.value));
			expression.push_back(res + " = " + left.value + " / " + right.value);
			return Token(INTEGER, res);
			break;
		}
		else if ((left.type == DECIMAL && right.type == DECIMAL)
			|| (left.type == INTEGER && right.type == DECIMAL)
			|| (left.type == DECIMAL && right.type == INTEGER)) {
			if (stoi(right.value) == 0) {
				std::stringstream out;
				out << "Divide by zero !";
				throw(out.str());
			}
			std::string res = DecimalToString(stod(left.value) / stod(right.value));
			expression.push_back(res + " = " + left.value + " / " + right.value);
			return Token(DECIMAL, res);
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid MUL type !";
			throw(out.str());
		}
	case MOD:
		if (left.type == INTEGER && right.type == INTEGER) {
			std::string res = std::to_string(stoi(left.value) % stoi(right.value));
			expression.push_back(res + " = " + left.value + " % " + right.value);
			return Token(INTEGER, res);
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid MOD type !";
			throw(out.str());
		}
	case LESS:
		if ((left.type == INTEGER || left.type == DECIMAL)
			&& (right.type == INTEGER || right.type == DECIMAL)) {
			return stod(left.value) < stod(right.value)
				? Token(BOOLEAN, "true")
				: Token(BOOLEAN, "false");
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid Less than type !";
			throw(out.str());
		}
	case LEQUAL:
		if ((left.type == INTEGER || left.type == DECIMAL)
			&& (right.type == INTEGER || right.type == DECIMAL)) {
			return stod(left.value) <= stod(right.value)
				? Token(BOOLEAN, "true")
				: Token(BOOLEAN, "false");
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid Less equal type !";
			throw(out.str());
		}
	case GREATER:
		if ((left.type == INTEGER || left.type == DECIMAL)
			&& (right.type == INTEGER || right.type == DECIMAL)) {
			return stod(left.value) > stod(right.value)
				? Token(BOOLEAN, "true")
				: Token(BOOLEAN, "false");
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid Greater than type !";
			throw(out.str());
		}
	case GEQUAL:
		if ((left.type == INTEGER || left.type == DECIMAL)
			&& (right.type == INTEGER || right.type == DECIMAL)) {
			return stod(left.value) >= stod(right.value)
				? Token(BOOLEAN, "true")
				: Token(BOOLEAN, "false");
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid Greater equal type !";
			throw(out.str());
		}
	case EQUAL:
		if ((left.type == INTEGER || left.type == DECIMAL)
			&& (right.type == INTEGER || right.type == DECIMAL)) {
			return stod(left.value) == stod(right.value)
				? Token(BOOLEAN, "true")
				: Token(BOOLEAN, "false");
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid Equal type !";
			throw(out.str());
		}
	case NOTEQUAL:
		if ((left.type == INTEGER || left.type == DECIMAL)
			&& (right.type == INTEGER || right.type == DECIMAL)) {
			return stod(left.value) != stod(right.value)
				? Token(BOOLEAN, "true")
				: Token(BOOLEAN, "false");
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid Equal type !";
			throw(out.str());
		}
	case XOR:
		if (left.type == BOOLEAN && right.type == BOOLEAN) {
			return left.value != right.value
				? Token(BOOLEAN, "true")
				: Token(BOOLEAN, "false");
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid XOR type !";
			throw(out.str());
		}
	case AND:
		if (left.type == BOOLEAN && right.type == BOOLEAN) {
			return left.value == "true" && right.value == "true"
				? Token(BOOLEAN, "true")
				: Token(BOOLEAN, "false");
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid AND type !";
			throw(out.str());
		}
	case OR:
		if (left.type == BOOLEAN && right.type == BOOLEAN) {
			return left.value == "true" || right.value == "true"
				? Token(BOOLEAN, "true")
				: Token(BOOLEAN, "false");
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid OR type !";
			throw(out.str());
		}
	default:
		std::stringstream out;
		out << "Invalid token type !";
		throw(out.str());
	}
}

Token Interpreter::VisitUnaryOp(UnaryOp* node)
{
	Token child(VisitNode(node->child));
	switch ((node->token).type) {
	case ADD:
		if (child.type == INTEGER || child.type == DECIMAL) {
			expression.push_back(child.value + " = " + child.value);
			return child; break;
		}
		else {
			std::stringstream out;
			out << "Invalid ADD type !";
			throw(out.str());
		}
	case SUB:
		child = VisitNode(node->child);
		if (child.type == INTEGER) {
			std::string res = std::to_string(-1 * stoi(child.value));
			expression.push_back(res + " = " + " - " + child.value);
			return Token(child.type, res);
			break;
		}
		else if (child.type == DECIMAL) {
			std::string res = DecimalToString(-1.0 * stod(child.value));
			expression.push_back(res + " = " + " - " + child.value);
			return Token(child.type, res);
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid SUB type !";
			throw(out.str());
		}
	case NOT:
		child = VisitNode(node->child);
		if (child.type == BOOLEAN) {
			return child.value == "true"
				? Token(BOOLEAN, "false")
				: Token(BOOLEAN, "true");
			break;
		}
		else {
			std::stringstream out;
			out << "Invalid NOT type !";
			throw(out.str());
		}
	default:
		std::stringstream out;
		out << "Invalid token type !";
		throw(out.str());
	}
}

Token Interpreter::VisitAssign(Assign* node)
{
	std::string variable = static_cast<Var*>(node->left)->val;
	//这里必须要先右后左，先获取赋值号右边的值，再赋值给左边的变量，否则会造成a=a+1这类报错。
	Token right(VisitNode(node->right));
	Token left(VisitNode(node->left));
	if (left.type == LID &&
		(right.type == INTEGER || right.type == DECIMAL
			|| right.type == BOOLEAN)) {
		symbol_table->table[left.value] = right;
		expression.push_back(left.value + " = "  + right.value);
		return right;
	}
	else {
		std::stringstream out;
		out << "Invalid Assign type !";
		throw(out.str());
	}
}

Token Interpreter::VisitNoOp(NoOp* node)
{
	return Token(END, "\0");
}
