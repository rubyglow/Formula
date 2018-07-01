/**
 *
 * Copyright (c) 2001, Frank Buﬂ
 *
 * project: Formula
 * version: $Revision: 1.3 $ $Name:  $
 *
 * Evaluator class and all Action classes.
 */

#include "Evaluator.h"

using namespace std;

float NumberStack::popWithReturn()
{
	float value = top();
	pop();
	return value;
}


void Action::checkTopStackElement(NumberStack& numberStack)
{
	if (!isfinite(numberStack.top()) || isnan(numberStack.top())) throw MathError();
}


NumberAction::NumberAction(string value)
{
	m_value = atof(value.c_str());
}

void NumberAction::run(NumberStack& numberStack)
{
	numberStack.push(m_value);
	checkTopStackElement(numberStack);
}



void MulAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 * op2);
	checkTopStackElement(numberStack);
}


void DivAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 / op2);
	checkTopStackElement(numberStack);
}


void AddAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 + op2);
	checkTopStackElement(numberStack);
}

void LessAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 < op2);
	checkTopStackElement(numberStack);
}

void GreaterAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 > op2);
	checkTopStackElement(numberStack);
}

void LessEqualAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 <= op2);
	checkTopStackElement(numberStack);
}

void GreaterEqualAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 >= op2);
	checkTopStackElement(numberStack);
}

void EqualAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 == op2);
	checkTopStackElement(numberStack);
}

void NotEqualAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 != op2);
	checkTopStackElement(numberStack);
}

void AndAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 && op2);
	checkTopStackElement(numberStack);
}

void OrAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 || op2);
	checkTopStackElement(numberStack);
}

void NotAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 1) throw StackUnderflow();
	numberStack.push(!numberStack.popWithReturn());
	checkTopStackElement(numberStack);
}

void SubAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(op1 - op2);
	checkTopStackElement(numberStack);
}

void NegAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 1) throw StackUnderflow();
	numberStack.push(-numberStack.popWithReturn());
	checkTopStackElement(numberStack);
}

void PowerAction::run(NumberStack& numberStack)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(pow(op1, op2));
	checkTopStackElement(numberStack);
}

Evaluator::~Evaluator()
{
	deleteActions();
}

void Evaluator::addAction(Action* action)
{
	m_actions.push_back(action);
}

float Evaluator::eval() throw(FunctionNotFound, VariableNotFound, StackUnderflow, MathError)
{
	if (m_actions.size() == 0) return 0;
	NumberStack numberStack;
	for (int i = 0; i < (int) m_actions.size(); i++) m_actions[i]->run(numberStack);
	if (numberStack.size() != 1) throw StackUnderflow();
	return numberStack.popWithReturn();
}

void Evaluator::removeAllActions()
{
	deleteActions();
	m_actions.clear();
}

void Evaluator::setVariable(string name, float value)
{
	m_variables[name] = value;
}

void Evaluator::removeVariable(string name)
{
	map<string, float>::iterator i = m_variables.find(name);
	if (i != m_variables.end()) {
		m_variables.erase(i);
	}
}

void Evaluator::removeAllVariables()
{
	m_variables.clear();
}

float Evaluator::getVariable(string name) throw(VariableNotFound)
{
	map<string, float>::iterator i = m_variables.find(name);
	if (i != m_variables.end()) {
		return i->second;
	} else {
		throw VariableNotFound(name);
	}
	return 0;
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
	numberStack.push(m_evaluator->getVariable(m_name));
	checkTopStackElement(numberStack);
}


void NoArgumentFunctionAction::run(NumberStack& numberStack) throw(FunctionNotFound, MathError)
{
	numberStack.push(m_evaluator->evalFunction(m_name));
	checkTopStackElement(numberStack);
}

void OneArgumentFunctionAction::run(NumberStack& numberStack) throw(FunctionNotFound, StackUnderflow, MathError)
{
	if (numberStack.size() < 1) throw StackUnderflow();
	numberStack.push(m_evaluator->evalFunction(m_name, numberStack.popWithReturn()));
	checkTopStackElement(numberStack);
}

void TwoArgumentsFunctionAction::run(NumberStack& numberStack) throw(FunctionNotFound, StackUnderflow, MathError)
{
	if (numberStack.size() < 2) throw StackUnderflow();
	float op2 = numberStack.popWithReturn();
	float op1 = numberStack.popWithReturn();
	numberStack.push(m_evaluator->evalFunction(m_name, op1, op2));
	checkTopStackElement(numberStack);
}
