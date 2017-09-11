#pragma once

struct Program;
struct num_expr;
struct bool_expr;
enum bool_expr_type;

int max(int, int);
int bheight(bool_expr*);

//Integer literal
//int

//Boolean Literal
//bool

//Arithmetic operator
enum arith_op {
	add,
	sub,
	mul,
	divide,
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
	num_expr* body;

	Program(int a, num_expr* b)
		: args(a), body(b) {}
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

	num_expr(num_expr_type t)
		: type(t) {}

	virtual num_expr* getFirst() { return this; }
	virtual num_expr* getSecond() { return this; }
	virtual bool_expr* getBoolExpr();
};

//Integer numeric expression
struct int_expr : num_expr {
	int val;

	int_expr(int v)
		: num_expr(integer), val(v) {}
};

//Argument numeric expression
struct arg_expr : num_expr {
	int args;

	arg_expr(int a)
		: num_expr(argument), args(a) {}
};

//Arithmetic numeric expression
struct arith_expr : num_expr {
	arith_op op;
	num_expr* first;
	num_expr* second;

	arith_expr(arith_op o, num_expr* f, num_expr* s)
		: num_expr(arithmetic), op(o), first(f), second(s) {}

	virtual num_expr* getFirst() { return first; }
	virtual num_expr* getSecond() { return second; }
};

//Conditional numeric expression
struct cond_expr : num_expr {
	bool_expr* test;
	num_expr* pass;
	num_expr* fail;

	cond_expr(bool_expr* t, num_expr* p, num_expr* f)
		: num_expr(conditional), test(t), pass(p), fail(f) {}

	virtual num_expr* getFirst() { return pass; }
	virtual num_expr* getSecond() { return fail; }
	virtual bool_expr* getBoolExpr() { return test; }
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

	bool_expr(bool_expr_type t)
		: type(t) {}

	virtual num_expr* getFirstNum() { return &num_expr(num_expr_type::integer); }
	virtual num_expr* getSecondNum() { return &num_expr(num_expr_type::integer); }

	virtual bool_expr* getFirstBool() { return this; }
	virtual bool_expr* getSecondBool() { return this; }
};

//Bool boolean expression...
//terrible naming convention :(
struct boolean_expr : bool_expr {
	bool val;

	boolean_expr(bool v)
		: bool_expr(boolean), val(v) {}
};

//Relational boolean expression
struct relation_expr : bool_expr {
	rel_op op;
	num_expr* first;
	num_expr* second;

	relation_expr(rel_op o, num_expr* f, num_expr* s)
		: bool_expr(relational), op(o), first(f), second(s) {}

	virtual num_expr* getFirstNum() { return first; }
	virtual num_expr* getSecondNum() { return second; }
};

//Logic boolean expression
struct logic_expr : bool_expr {
	logic_op op;
	bool_expr* first;
	bool_expr* second;

	logic_expr(logic_op o, bool_expr* f, bool_expr* s)
		: bool_expr(logic), op(o), first(f), second(s) {}

	virtual bool_expr* getFirstBool() { return first; }
	virtual bool_expr* getSecondBool() { return second; }
};

//N-Height
int nheight(num_expr* ne) {
	switch (ne->type) {
	case num_expr_type::integer:
		return 0;
		break;
	case num_expr_type::argument:
		return 1;
		break;
	case num_expr_type::arithmetic:
		return 1 + max(nheight(ne->getFirst()), nheight(ne->getSecond()));
		break;
	case num_expr_type::conditional:
		return 1 + max(bheight(ne->getBoolExpr()), max(nheight(ne->getFirst()), nheight(ne->getSecond())));
		break;
	}
}

//B-Height
int bheight(bool_expr* be) {
	switch (be->type) {
	case bool_expr_type::boolean:
		return 0;
		break;
	case bool_expr_type::relational:
		return 1 + max(nheight(be->getFirstNum()), nheight(be->getSecondNum()));
		break;
	case bool_expr_type::logic:
		return 1 + max(bheight(be->getFirstBool()), bheight(be->getSecondBool()));
		break;
	}
}

int max(int a, int b) {
	if (a > b) return a;
	else return b;
}

bool_expr* num_expr::getBoolExpr() {
	return &bool_expr(bool_expr_type::boolean);
}