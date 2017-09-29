#pragma once
#include "stdafx.h"
#include "EL_AST.hpp"

//IntPair '==' Overload
bool IntPair::operator==(IntPair a) {
	if (this->x == a.x && this->y == a.y) return true;
	return false;
}

//N-Height
int nheight(num_expr* ne) {
	switch (ne->type) {
	case num_expr_type::integer: {
		return 0;
	}
	case num_expr_type::argument: {
		return 1;
	}
	case num_expr_type::arithmetic: {
		return 1 + max(nheight(static_cast<arith_expr*>(ne)->first), nheight(static_cast<arith_expr*>(ne)->second));
	}
	case num_expr_type::conditional: {
		return 1 + max(bheight(static_cast<cond_expr*>(ne)->test),
			max(nheight(static_cast<cond_expr*>(ne)->pass), nheight(static_cast<cond_expr*>(ne)->fail)));
	}
	}
}

//B-Height
int bheight(bool_expr* be) {
	switch (be->type) {
	case bool_expr_type::boolean: {
		return 0;
	}
	case bool_expr_type::relational: {
		return 1 + max(nheight(static_cast<relation_expr*>(be)->first),
			nheight(static_cast<relation_expr*>(be)->second));
	}
	case bool_expr_type::logic: {
		return 1 + max(bheight(static_cast<logic_expr*>(be)->first),
			bheight(static_cast<logic_expr*>(be)->second));
	}
	}
}

//Min-Max Arguments
//Numeric expression
IntPair minMaxArgs(num_expr* ne) {
	IntPair first, second, boolean;
	switch (ne->type) {
	case num_expr_type::integer: {
		return IntPair(0, 0);
	}
	case num_expr_type::argument: {
		return IntPair(static_cast<arg_expr*>(ne)->args, static_cast<arg_expr*>(ne)->args);
	}
	case num_expr_type::arithmetic: {
		first = minMaxArgs(static_cast<arith_expr*>(ne)->first);
		second = minMaxArgs(static_cast<arith_expr*>(ne)->second);
		return IntPair(min(first.x, second.x), max(first.y, second.y));
	}
	case num_expr_type::conditional: {
		first = minMaxArgs(static_cast<cond_expr*>(ne)->pass);
		second = minMaxArgs(static_cast<cond_expr*>(ne)->fail);
		boolean = minMaxArgs(static_cast<cond_expr*>(ne)->test);
		return IntPair(min(boolean.x, min(first.x, second.x)), max(boolean.y, max(first.y, second.y)));
	}
	}
}

//Bool expression
IntPair minMaxArgs(bool_expr* be) {
	IntPair first, second;
	switch (be->type) {
	case bool_expr_type::boolean: {
		return IntPair(0, 0);
	}
	case bool_expr_type::relational: {
		first = minMaxArgs(static_cast<relation_expr*>(be)->first);
		second = minMaxArgs(static_cast<relation_expr*>(be)->second);
		return IntPair(min(first.x, second.x), max(first.y, second.y));
	}
	case bool_expr_type::logic: {
		first = minMaxArgs(static_cast<logic_expr*>(be)->first);
		second = minMaxArgs(static_cast<logic_expr*>(be)->second);
		return IntPair(min(first.x, second.x), max(first.y, second.y));
	}
	}
}

//Program folding
Program* pFold(Program* p) {
	p->body = nFold(p->body);
	return p;
}

//Numeric expression folding
num_expr* nFold(num_expr* n) {
	switch (n->type) {
	case num_expr_type::integer: {
		return n;
	}
	case num_expr_type::argument: {
		return n;
	}
	case num_expr_type::arithmetic: {
		*n = arithFold(static_cast<arith_expr*>(n));
		return n;
	}
	case num_expr_type::conditional: {
		cond_expr* a = static_cast<cond_expr*>(n);
		a->test = bFold(a->test);
		a->pass = nFold(a->pass);
		a->fail = nFold(a->fail);
		return a;
	}
	}
}

//Arithmetic expression folding
num_expr arithFold(arith_expr* n) {
	//if both operands are integer literals
	if (n->first->type == num_expr_type::integer && n->second->type == num_expr_type::integer) {
		num_expr* first = n->first;
		num_expr* second = n->second;
		switch (n->op) {
		case arith_op::add: {
			return int_expr(static_cast<int_expr*>(first)->val + static_cast<int_expr*>(second)->val);
		}
		case arith_op::sub: {
			return int_expr(static_cast<int_expr*>(first)->val - static_cast<int_expr*>(second)->val);
		}
		case arith_op::mul: {
			return int_expr(static_cast<int_expr*>(first)->val * static_cast<int_expr*>(second)->val);
		}
		case arith_op::divide: {
			return int_expr(static_cast<int_expr*>(first)->val / static_cast<int_expr*>(second)->val);
		}
		case arith_op::mod: {
			return int_expr(static_cast<int_expr*>(first)->val % static_cast<int_expr*>(second)->val);
		}
		}
	}
	//if one of the two operands is not an integer literal
	else {
		n->first = nFold(n->first);
		n->second = nFold(n->second);
		return arithFold(n);
	}
}

//Boolean expression folding
bool_expr* bFold(bool_expr* b) {
	switch (b->type) {
	case bool_expr_type::boolean: {
		return b;
	}
	case bool_expr_type::relational: {
		relation_expr* a = static_cast<relation_expr*>(b);
		a->first = nFold(a->first);
		a->second = nFold(a->second);
	}
	case bool_expr_type::logic: {
		logic_expr* a = static_cast<logic_expr*>(b);
		a->first = bFold(a->first);
		a->second = bFold(a->second);
	}
	}
	return b;
}

//Numeric expression equality
bool neq(num_expr* n1, num_expr* n2) {
	if (n1->type != n2->type) return false;
	switch (n1->type) {
	case num_expr_type::integer: {
		int_expr* a = static_cast<int_expr*>(n1);
		int_expr* b = static_cast<int_expr*>(n2);
		return a->val == b->val;
	}
	case num_expr_type::argument: {
		arg_expr* a = static_cast<arg_expr*>(n1);
		arg_expr* b = static_cast<arg_expr*>(n2);
		return a->args == b->args;
	}
	case num_expr_type::arithmetic: {
		arith_expr* a = static_cast<arith_expr*>(n1);
		arith_expr* b = static_cast<arith_expr*>(n2);
		if (a->op != b->op) return false;
		if (neq(a->first, b->first) && neq(a->second, b->second)) return true;
		else return false;
	}
	case num_expr_type::conditional: {
		cond_expr* a = static_cast<cond_expr*>(n1);
		cond_expr* b = static_cast<cond_expr*>(n2);
		if (!beq(a->test, b->test)) return false;
		return neq(a->pass, b->pass) && neq(a->fail, b->fail);
	}
	}
}

//Boolean expression equality
bool beq(bool_expr* b1, bool_expr* b2) {
	if (b1->type != b2->type) return false;
	switch (b1->type) {
	case bool_expr_type::boolean: {
		boolean_expr* bool1 = static_cast<boolean_expr*>(b1);
		boolean_expr* bool2 = static_cast<boolean_expr*>(b2);
		return bool1->val = bool2->val;
	}
	case bool_expr_type::relational: {
		relation_expr* rel1 = static_cast<relation_expr*>(b1);
		relation_expr* rel2 = static_cast<relation_expr*>(b2);
		if (rel1->op != rel2->op) return false;
		if (neq(rel1->first, rel2->first) && neq(rel1->second, rel2->second)) return true;
		else return false;
	}
	case bool_expr_type::logic: {
		logic_expr* log1 = static_cast<logic_expr*>(b1);
		logic_expr* log2 = static_cast<logic_expr*>(b2);
		if (log1->op != log2->op) return false;
		if (beq(log1->first, log2->first) && beq(log1->second, log2->second)) return true;
		else return false;
	}
	}
}

//Minimum
int min(int a, int b) {
	if (a < b) return a;
	else return b;
}

//Maximum
int max(int a, int b) {
	if (a > b) return a;
	else return b;
}