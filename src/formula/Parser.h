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
	void setExpression(string expression) throw(SyntaxError, TooManyArgumentsError);
	void setVariable(string name, float value) { m_evaluator.setVariable(name, value); }
	void removeVariable(string name) { m_evaluator.removeVariable(name); }
	void removeAllVariables() { m_evaluator.removeAllVariables(); }
	void setFunction(string name, float(*function)()) { m_evaluator.setFunction(name, function); }
	void setFunction(string name, float(*function)(float)) { m_evaluator.setFunction(name, function); }
	void setFunction(string name, float(*function)(float, float)) { m_evaluator.setFunction(name, function); }
	void removeFunction(string name) { m_evaluator.removeFunction(name); }
	void removeAllFunctions() { m_evaluator.removeAllFunctions(); }
	string getPostfix() { return m_postfix; }
	float eval() throw(FunctionNotFound, VariableNotFound, StackUnderflow, MathError) { return m_evaluator.eval(); }


private:
	void setExpressionFinally();
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
};


#endif
