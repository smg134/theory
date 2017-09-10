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

//Integer numeric expression
struct int_expr : num_expr {
	int val;
};

//Argument numeric expression
struct arg_expr : num_expr {
	int args;
};

//Arithmetic numeric expression
struct arith_expr : num_expr {
	arith_op op;
	num_expr first;
	num_expr second;
};

//Conditional numeric expression
struct cond_expr : num_expr {
	bool_expr test;
	num_expr pass;
	num_expr fail;
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

//Bool boolean expression...
//terrible naming convention :(
struct boolean_expr : bool_expr {
	bool val;
};

//Relational boolean expression
struct relation_expr : bool_expr {
	rel_op op;
	num_expr first;
	num_expr second;
};

//Logic boolean expression
struct logic_expr : bool_expr {
	logic_op op;
	bool_expr first;
	bool_expr second;
};

//N-Height
int nheight(const num_expr& ne) {

}

//B-Height
int bheight(const bool_expr& be) {

}