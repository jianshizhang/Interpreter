#include <iostream>
#include <string>
#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "SymbolTable.h"

void TestLexer() {
	std::string in = "a = aname+1-2.3*4/5%  6 (1+2)==!=<<=>>=true false && || ! ^";
	std::cout << "TestLexer:\n" << in << "\n";
	Lexer l(in);
	try {
		std::cout << l.ToStr() << std::endl;
	}
	catch (std::string error) {
		std::cerr << error << std::endl;
	}
}

void TestParser() {
	std::string in = "a=1+2.3-4*5/6";
	std::cout << "TestParser:\n" << in << "\n";
	Lexer l(in);
	Parser p(&l);
	p.parser();
	std::cout << p.ToStr() << std::endl;
}

void TestInterpreter() {
	std::string text = "a=(1+2)*3/1.5";
	std::cout << "TestInterpreter:\n" << text << "\n";
	std::string mode = "$type";
	SymbolTable symbol_table;
	try
	{
		Lexer lexer(text);
		Parser pa(&lexer);
		Interpreter interp(&pa, &symbol_table);
		std::cout << interp.interpreter(mode) << std::endl;
		std::cout << interp.ToStr() ;
	}
	catch (std::string error)
	{
		std::cerr << error << std::endl;
	}
}

void Calculator() {
	std::string text;
	std::string help = "Enter $help to view help.\n\
Enter $symnols to view the symbol table.\n\
Four output modes to choose:\n\
 $default   : Output value only. \n\
 $type      : Output type and value. \n\
 $induction : Output type, value and induction.\n\
 $expression: Output type, value and expression.\n\
Enter your expression.(enter ## to quit.)\n\
";
	std::cout << help;
	std::string mode = "$default";
	SymbolTable symbol_table;
	while (std::cin) {
		std::cout << ">>> ";
		getline(std::cin, text);
		try
		{
			if (text == "$help") {
				std::cout << help;
			}
			else if (text == "$symbols") {
				std::cout << symbol_table.ToStr();
			}
			else if (text == "$default" 
				|| text == "$type" 
				|| text == "$induction"
				|| text == "$expression") {
				mode = text;
			}
			else if (text == "##") {
				break;
			}
			else {
				Lexer lexer(text);
				Parser pa(&lexer);
				Interpreter interp(&pa, &symbol_table);
				std::cout << interp.interpreter(mode) << std::endl;
			}
		}
		catch (std::string error)
		{
			std::cerr << error << std::endl;
		}
	}
}
int main() {
	//TestLexer();
	//TestParser();
	//TestInterpreter();
	Calculator();
}