#pragma once

struct Program;
struct num_expr;
struct bool_expr;

//Integer literal
//int

//Boolean Literal
//bool

//Arithmetic operator
enum arith_op {
	add,
	sub,
	mul,
	div,
	mod,
};

//Relational operator
enum rel_op {
	less,
	equal,
	greater,
};

//Logic operator
enum logic_op {
	and,
	or,
};

//Program
struct Program {
	int args;
	num_expr body;
};

//Numeric expression
struct num_expr {

};

//Boolean expression
struct bool_expr {

};