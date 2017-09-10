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

//Numeric expression type
enum num_expr_type {
	integer,
	argument,
	arithmetic,
	conditional,
};

//Numeric expression
struct num_expr {
	num_expr_type type;
};

//Boolean expression type
enum bool_expr_type {
	boolean,
	relational,
	logic,
};

//Boolean expression
struct bool_expr {
	bool_expr_type type;
};



//N-Height
int nheight(const num_expr& ne) {

}

//B-Height
int bheight(const bool_expr& be) {

}