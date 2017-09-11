// AST.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "EL_AST.hpp"
#include <iostream>

void printPass(int n) {
	std::cout << n << ". Pass\n";
}

void printFail(int n) {
	std::cout << n << ". Fail\n";
}

int main()
{
	//Integer literal numeric expression
	{
		int_expr* testVar = new int_expr(1);
		if (nheight(testVar) == 0) printPass(1);
		else printFail(1);
	}

	//Argument numeric expression
	{
		arg_expr* testVar = new arg_expr(1);
		if (nheight(testVar) == 1) printPass(2);
		else printFail(2);
	}

	//Arithmetic numeric expression
	{
		int_expr* one = new int_expr(1);
		arg_expr* two = new arg_expr(1);
		arith_expr* testVar = new arith_expr(arith_op::add, one, two);
		if (nheight(testVar) == 2) printPass(3);
		else printFail(3);
	}

	//Conditional numeric expression
	{
		int_expr* one = new int_expr(1);
		arg_expr* two = new arg_expr(1);
		bool_expr* boolean = new boolean_expr(true);
		cond_expr* testVar = new cond_expr(boolean, one, two);
		if (nheight(testVar) == 2) printPass(4);
		else printFail(4);
	}

	//Boolean bool expression...
	{
		bool_expr* testVar = new boolean_expr(true);
		if (bheight(testVar) == 0) printPass(5);
		else printFail(5);
	}

	//Relational bool expression
	{
		int_expr* one = new int_expr(1);
		int_expr* two = new int_expr(2);
		bool_expr* testVar = new relation_expr(rel_op::less, one, two);
		if (bheight(testVar) == 1) printPass(6);
		else printFail(6);
	}

	//Logic bool expression
	{
		bool_expr* one = new boolean_expr(true);
		bool_expr* two = new boolean_expr(false);
		bool_expr* testVar = new logic_expr(logic_op::and, one, two);
		if (bheight(testVar) == 1) printPass(7);
		else printFail(7);
	}

	return 0;
}

