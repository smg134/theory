#pragma once

struct Program;
struct num_expr;
struct bool_expr;

int min(int, int);
int max(int, int);
int bheight(bool_expr*);

struct IntPair {
	int x;
	int y;

	IntPair() {}
	IntPair(int a, int b)
		: x(a), y(b) {}

	bool operator==(IntPair a) {
		if (this->x == a.x && this->y == a.y) return true;
		return false;
	}
};

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
};

//Conditional numeric expression
struct cond_expr : num_expr {
	bool_expr* test;
	num_expr* pass;
	num_expr* fail;

	cond_expr(bool_expr* t, num_expr* p, num_expr* f)
		: num_expr(conditional), test(t), pass(p), fail(f) {}
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
};

//Logic boolean expression
struct logic_expr : bool_expr {
	logic_op op;
	bool_expr* first;
	bool_expr* second;

	logic_expr(logic_op o, bool_expr* f, bool_expr* s)
		: bool_expr(logic), op(o), first(f), second(s) {}
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
		return 1 + max(nheight(static_cast<arith_expr*>(ne)->first), nheight(static_cast<arith_expr*>(ne)->second));
		break;
	case num_expr_type::conditional:
		return 1 + max(bheight(static_cast<cond_expr*>(ne)->test), 
			max(nheight(static_cast<cond_expr*>(ne)->pass), nheight(static_cast<cond_expr*>(ne)->fail)));
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
		return 1 + max(nheight(static_cast<relation_expr*>(be)->first), 
			nheight(static_cast<relation_expr*>(be)->second));
		break;
	case bool_expr_type::logic:
		return 1 + max(bheight(static_cast<logic_expr*>(be)->first), 
			bheight(static_cast<logic_expr*>(be)->second));
		break;
	}
}

IntPair minMaxArgs(bool_expr*);
//Min-Max Arguments
//Numeric expression
IntPair minMaxArgs(num_expr* ne) {
	IntPair first, second, boolean;
	switch (ne->type) {
	case num_expr_type::integer:
		return IntPair(0, 0);
		break;
	case num_expr_type::argument:
		return IntPair(static_cast<arg_expr*>(ne)->args, static_cast<arg_expr*>(ne)->args);
		break;
	case num_expr_type::arithmetic:
		first = minMaxArgs(static_cast<arith_expr*>(ne)->first);
		second = minMaxArgs(static_cast<arith_expr*>(ne)->second);
		return IntPair(min(first.x, second.x), max(first.y, second.y));
		break;
	case num_expr_type::conditional:
		first = minMaxArgs(static_cast<cond_expr*>(ne)->pass);
		second = minMaxArgs(static_cast<cond_expr*>(ne)->fail);
		boolean = minMaxArgs(static_cast<cond_expr*>(ne)->test);
		return IntPair(min(boolean.x, min(first.x, second.x)), max(boolean.y, max(first.y, second.y)));
		break;
	}
}

//Bool expression
IntPair minMaxArgs(bool_expr* be) {
	IntPair first, second;
	switch (be->type) {
	case bool_expr_type::boolean:
		return IntPair(0, 0);
		break;
	case bool_expr_type::relational:
		first = minMaxArgs(static_cast<relation_expr*>(be)->first);
		second = minMaxArgs(static_cast<relation_expr*>(be)->second);
		return IntPair(min(first.x, second.x), max(first.y, second.y));
		break;
	case bool_expr_type::logic:
		first = minMaxArgs(static_cast<logic_expr*>(be)->first);
		second = minMaxArgs(static_cast<logic_expr*>(be)->second);
		return IntPair(min(first.x, second.x), max(first.y, second.y));
		break;
	}
}

int min(int a, int b) {
	if (a < b) return a;
	else return b;
}

int max(int a, int b) {
	if (a > b) return a;
	else return b;
}