/**
 *
 * Copyright (c) 2001, Frank Bu�
 *
 * project: Formula
 * version: $Revision: 1.3 $ $Name:  $
 *
 * Evaluator class and all Action classes.
 */

#include "Evaluator.h"

using namespace std;

float NumberStack::top() throw(StackUnderflow)
{
	if (size() == 0) throw StackUnderflow();
	return m_values[m_size - 1];
}

float NumberStack::pop() throw(StackUnderflow)
{
	if (size() == 0) throw StackUnderflow();
	return m_values[--m_size];
}

void NumberStack::push(float value)
{
	m_size++;
	if (m_size > m_values.size()) {
		m_values.push_back(value);
	} else {
		m_values[m_size - 1] = value;
	}
}


void Action::checkTopStackElement(NumberStack& numberStack)
{
	if (!isfinite(numberStack.top()) || isnan(numberStack.top())) throw MathError();
}


NumberAction::NumberAction(string value)
{
	m_value = atof(value.c_str());
}

void NumberAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	numberStack.push(m_value);
	checkTopStackElement(numberStack);
}



void MulAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 * op2);
	checkTopStackElement(numberStack);
}


void DivAction::run(NumberStack& numberStack)  throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	if (op2 == 0.0f) {
		throw MathError();
	}
	numberStack.push(op1 / op2);
	checkTopStackElement(numberStack);
}


void AddAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 + op2);
	checkTopStackElement(numberStack);
}

void LessAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 < op2);
	checkTopStackElement(numberStack);
}

void GreaterAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 > op2);
	checkTopStackElement(numberStack);
}

void LessEqualAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 <= op2);
	checkTopStackElement(numberStack);
}

void GreaterEqualAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 >= op2);
	checkTopStackElement(numberStack);
}

void EqualAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 == op2);
	checkTopStackElement(numberStack);
}

void NotEqualAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 != op2);
	checkTopStackElement(numberStack);
}

void AndAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 && op2);
	checkTopStackElement(numberStack);
}

void OrAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 || op2);
	checkTopStackElement(numberStack);
}

void NotAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	numberStack.push(!numberStack.pop());
	checkTopStackElement(numberStack);
}

void SubAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(op1 - op2);
	checkTopStackElement(numberStack);
}

void NegAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	numberStack.push(-numberStack.pop());
	checkTopStackElement(numberStack);
}

void PowerAction::run(NumberStack& numberStack) throw(StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(pow(op1, op2));
	checkTopStackElement(numberStack);
}

Evaluator::~Evaluator()
{
	deleteActions();
	for (auto it = m_variables.begin(); it != m_variables.end(); it++) {
		delete it->second;
	}
}

void Evaluator::addAction(Action* action)
{
	m_actions.push_back(action);
}

float Evaluator::eval() throw(FunctionNotFound, VariableNotFound, StackUnderflow, MathError)
{
	if (m_actions.size() == 0) return 0;
	m_numberStack.clear();
	for (int i = 0; i < (int) m_actions.size(); i++) m_actions[i]->run(m_numberStack);
	return m_numberStack.pop();
}

void Evaluator::removeAllActions()
{
	deleteActions();
	m_actions.clear();
}

void Evaluator::setVariable(string name, float value)
{
	auto i = m_variables.find(name);
	if (i == m_variables.end()) {
		m_variables[name] = new float;
	}
	*getVariableAddress(name) = value;
}

float Evaluator::getVariable(string name) throw(VariableNotFound)
{
	return *getVariableAddress(name);
}

float* Evaluator::getVariableAddress(string name) throw(VariableNotFound)
{
	auto i = m_variables.find(name);
	if (i != m_variables.end()) {
		return i->second;
	} else {
		throw VariableNotFound(name);
	}
}

void Evaluator::setFunction(string name, float(*function)())
{
	m_noArgumentFunctions[name] = function;
}

void Evaluator::setFunction(string name, float(*function)(float))
{
	m_oneArgumentFunctions[name] = function;
}

void Evaluator::setFunction(string name, float(*function)(float, float))
{
	m_twoArgumentsFunctions[name] = function;
}

void Evaluator::removeFunction(string name)
{
	map<string, float(*)()>::iterator no = m_noArgumentFunctions.find(name);
	if (no != m_noArgumentFunctions.end()) {
		m_noArgumentFunctions.erase(no);
	}
	map<string, float(*)(float)>::iterator one = m_oneArgumentFunctions.find(name);
	if (one != m_oneArgumentFunctions.end()) {
		m_oneArgumentFunctions.erase(one);
	}
	map<string, float(*)(float, float)>::iterator two = m_twoArgumentsFunctions.find(name);
	if (two != m_twoArgumentsFunctions.end()) {
		m_twoArgumentsFunctions.erase(two);
	}
}

void Evaluator::removeAllFunctions()
{
	m_noArgumentFunctions.clear();
	m_oneArgumentFunctions.clear();
	m_twoArgumentsFunctions.clear();
}

float Evaluator::evalFunction(string name) throw(FunctionNotFound, StackUnderflow, MathError)
{
	float(*function)() = m_noArgumentFunctions[name];
	if (function) {
		return function();
	} else {
		throw FunctionNotFound(name);
	}
	return 0;
}

float Evaluator::evalFunction(string name, float argument) throw(FunctionNotFound, StackUnderflow, MathError)
{
	float(*function)(float) = m_oneArgumentFunctions[name];
	if (function) {
		return function(argument);
	} else {
		throw FunctionNotFound(name);
	}
	return 0;
}

float Evaluator::evalFunction(string name, float argument1, float argument2) throw(FunctionNotFound, StackUnderflow, MathError)
{
	float(*function)(float, float) = m_twoArgumentsFunctions[name];
	if (function) {
		return function(argument1, argument2);
	} else {
		throw FunctionNotFound(name);
	}
	return 0;
}


void Evaluator::deleteActions()
{
	for (int i = 0; i < (int) m_actions.size(); i++) delete m_actions[i];
}


void VariableAction::run(NumberStack& numberStack) throw(VariableNotFound, MathError)
{
	if (!m_variableAddress) m_variableAddress = m_evaluator->getVariableAddress(m_name);
	numberStack.push(*m_variableAddress);
	checkTopStackElement(numberStack);
}


void NoArgumentFunctionAction::run(NumberStack& numberStack) throw(FunctionNotFound, MathError)
{
	numberStack.push(m_evaluator->evalFunction(m_name));
	checkTopStackElement(numberStack);
}

void OneArgumentFunctionAction::run(NumberStack& numberStack) throw(FunctionNotFound, StackUnderflow, MathError)
{
	numberStack.push(m_evaluator->evalFunction(m_name, numberStack.pop()));
	checkTopStackElement(numberStack);
}

void TwoArgumentsFunctionAction::run(NumberStack& numberStack) throw(FunctionNotFound, StackUnderflow, MathError)
{
	float op2 = numberStack.pop();
	float op1 = numberStack.pop();
	numberStack.push(m_evaluator->evalFunction(m_name, op1, op2));
	checkTopStackElement(numberStack);
}
