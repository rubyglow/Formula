/**
 *
 * Copyright (c) 2001, Frank Bu�
 *
 * project: Formula
 * version: $Revision: 1.3 $ $Name:  $
 *
 * Evaluator class and all Action classes.
 */

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <float.h>

#include "Exception.h"
#include "Exception.h"

using namespace std;

class NumberStack : public vector<float>
{
public:
	NumberStack() : m_size(0) {}
	float top() throw(StackUnderflow);
	float pop() throw(StackUnderflow);
	void push(float value);
	size_t size() { return m_size; }
private:
	vector<float> m_values;
	size_t m_size;
};


class Action
{
public:
	virtual ~Action() {};
	virtual void run(NumberStack& numberStack) = 0;
protected:
	void checkTopStackElement(NumberStack& numberStack);
};


class NumberAction : public Action
{
public:
	NumberAction(float value) : m_value(value) {}
	NumberAction(string value);

	void run(NumberStack& numberStack)  throw(StackUnderflow, MathError) override;

private:
	float m_value;
};


class MulAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};


class DivAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};


class AddAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class LessAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class GreaterAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class LessEqualAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class GreaterEqualAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class EqualAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class NotEqualAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class AndAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class OrAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class NotAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class SubAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class NegAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class PowerAction : public Action
{
public:
	void run(NumberStack& numberStack) throw(StackUnderflow, MathError) override;
};

class Evaluator
{
public:
	~Evaluator();
	void addAction(Action* action);
	float eval() throw(FunctionNotFound, VariableNotFound, StackUnderflow, MathError);
	void removeAllActions();
	void setVariable(string name, float value);
	float getVariable(string name) throw(VariableNotFound);
	float* getVariableAddress(string name) throw(VariableNotFound);

private:
	NumberStack m_numberStack;

	void deleteActions();

	vector<Action*> m_actions;
	map<string, float*> m_variables;
};


class VariableAction : public Action
{
public:
	VariableAction(Evaluator* evaluator, string name) : m_evaluator(evaluator), m_name(name), m_variableAddress(NULL) {}
	void run(NumberStack& numberStack) throw(VariableNotFound, MathError) override;

private:
	Evaluator* m_evaluator;
	string m_name;
	float* m_variableAddress;
};


class NoArgumentFunctionAction : public Action
{
public:
	NoArgumentFunctionAction(Evaluator* evaluator, float(*function)()) : m_evaluator(evaluator), m_function(function) {}
	void run(NumberStack& numberStack) throw(FunctionNotFound, MathError) override;

private:
	Evaluator* m_evaluator;
	float(*m_function)();
};

class OneArgumentFunctionAction : public Action
{
public:
	OneArgumentFunctionAction(Evaluator* evaluator, float(*function)(float)) : m_evaluator(evaluator), m_function(function) {}
	void run(NumberStack& numberStack) throw(FunctionNotFound, StackUnderflow, MathError) override;

private:
	Evaluator* m_evaluator;
	float(*m_function)(float);
};

class TwoArgumentsFunctionAction : public Action
{
public:
	TwoArgumentsFunctionAction(Evaluator* evaluator, float(*function)(float, float)) : m_evaluator(evaluator), m_function(function) {}
	void run(NumberStack& numberStack) throw(FunctionNotFound, StackUnderflow, MathError) override;

private:
	Evaluator* m_evaluator;
	float(*m_function)(float, float);
};


#endif
