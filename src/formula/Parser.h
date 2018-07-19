/**
 *
 * Copyright (c) 2001, Frank Buﬂ
 *
 * project: Formula
 * version: $Revision: 1.3 $ $Name:  $
 *
 * Parser class.
 */

#ifndef PARSER_H
#define PARSER_H

#include "Evaluator.h"
#include <string>
#include <vector>
#include <stack>

using namespace std;

class Token;

class Parser
{
	friend class OperatorToken;
	friend class AddToken;
	friend class SubToken;
	friend class NumberToken;
	friend class OpenBracketToken;
	friend class CloseBracketToken;
	friend class IdentifierToken;
	friend class CommaToken;

public:
	Parser(string expression);
	~Parser();
	void setExpression(string expression) throw(SyntaxError, TooManyArgumentsError, FunctionNotFound);
	void setVariable(string name, float value) {
		m_evaluator.setVariable(name, value);
	}
	float* getVariableAddress(string name) {
		return m_evaluator.getVariableAddress(name);
	}
	void setFunction(string name, float(*function)());
	void setFunction(string name, float(*function)(float));
	void setFunction(string name, float(*function)(float, float));
	float(*getNoArgumentFunction(string name))() throw(FunctionNotFound);
	float(*getOneArgumentFunction(string name))(float) throw(FunctionNotFound);
	float(*getTwoArgumentsFunction(string name))(float, float) throw(FunctionNotFound);
	
	string getPostfix() {
		return m_postfix;
	}
	float eval() throw(FunctionNotFound, VariableNotFound, StackUnderflow, MathError) {
		return m_evaluator.eval();
	}


private:
	void deleteTokens();
	string parseNumber(char c) throw(SyntaxError);
	string parseIdentifier(char c);
	char peekChar();
	void skipChar();
	char skipAndPeekChar();
	Token* peekToken();
	Token* peekNextToken();
	Token* peekLastToken();
	void skipToken();

	string m_expression;
	int m_currentIndex;
	int m_currentTokenIndex;
	string m_postfix;
	Evaluator m_evaluator;
	stack<Token*> m_operators;
	vector<Token*> m_tokens;
	stack<int> m_functionArgumentCountStack;
	map<string, float(*)()> m_noArgumentFunctions;
	map<string, float(*)(float)> m_oneArgumentFunctions;
	map<string, float(*)(float, float)> m_twoArgumentsFunctions;
};


#endif
