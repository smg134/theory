#pragma once

#include "stdafx.h"
#include <string>
///////////////////
///////////////////

enum exprType : int {
	ref,
	abs,
	app,
}

struct variable {
	
	std::string name;

	variable(const std::string n)
		: name(n) {}
}

struct expr {
	
	exprType type;
	
	expr(exprType t)
		: type(t) {}
}

struct reference : expression {
	
	variable* ref;
	
	reference(variable* v)
		: expr(exprType::ref), ref(v) {}
}

struct abstraction : expression {
	
	variable* param;
	expr* body;
	
	abstraction(variable* p, expr* b)
		: expr(exprType::abs), param(p), body(b) {}
}

