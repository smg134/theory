#pragma once

//Forward Declarations
//I should make a .cpp to avoid this but meh
struct IntPair;
struct Program;
struct num_expr;
struct bool_expr;
struct int_expr;
struct arg_expr;
struct arith_expr;
struct cond_expr;
struct boolean_expr;
struct relation_expr;
struct logic_expr;


//Function Prototypes
int min(int, int);
int max(int, int);
int bheight(bool_expr*);
IntPair minMaxArgs(bool_expr*);

//Return type of minMaxArgs()
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
	or ,
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

//Program folding
Program* pFold(Program* p) {
	p->body = nFold(p->body);
	return p;
}

//Numeric expression folding
num_expr* nFold(num_expr* n) {
	switch (n->type) {
	case (num_expr_type::integer):
		return n;
		break;
	case (num_expr_type::argument):
		//nothing yet?
		break;
	case (num_expr_type::arithmetic):
		*n = arithFold(static_cast<arith_expr*>(n));
		return n;
		break;
	case (num_expr_type::conditional):
		//nothing yet?
		//put bFold here
		break;
	}
}

//Arithmetic expression folding
num_expr arithFold(arith_expr* n) {
	//if both operands are integer literals
	if (n->first->type == num_expr_type::integer && n->second->type == num_expr_type::integer) {
		num_expr* first = n->first;
		num_expr* second = n->second;
		switch (n->op) {
		case (arith_op::add):
			return int_expr(static_cast<int_expr*>(first)->val + static_cast<int_expr*>(second)->val);
			break;
		case (arith_op::sub):
			return int_expr(static_cast<int_expr*>(first)->val - static_cast<int_expr*>(second)->val);
			break;
		case (arith_op::mul):
			return int_expr(static_cast<int_expr*>(first)->val * static_cast<int_expr*>(second)->val);
			break;
		case (arith_op::divide):
			return int_expr(static_cast<int_expr*>(first)->val / static_cast<int_expr*>(second)->val);
			break;
		case (arith_op::mod):
			return int_expr(static_cast<int_expr*>(first)->val % static_cast<int_expr*>(second)->val);
			break;
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
	//work on this
}

//Numeric expression equality
bool neq(num_expr* n1, num_expr* n2) {
	if (n1->type != n2->type) return false;
	switch (n1->type) {
	case (num_expr_type::integer):
		int_expr* int1 = static_cast<int_expr*>(n1);
		int_expr* int2 = static_cast<int_expr*>(n2);
		return int1->val == int2->val;
		break;
	case (num_expr_type::argument):
		arg_expr* arg1 = static_cast<arg_expr*>(n1);
		arg_expr* arg2 = static_cast<arg_expr*>(n2);
		return arg1->args == arg2->args;
		break;
	case (num_expr_type::arithmetic):
		arith_expr* arith1 = static_cast<arith_expr*>(n1);
		arith_expr* arith2 = static_cast<arith_expr*>(n2);
		if (arith1->op != arith2->op) return false;
		if (neq(arith1->first, arith2->first) && neq(arith1->second, arith2->second)) return true;
		else return false;
		break;
	case (num_expr_type::conditional):
		cond_expr* cond1 = static_cast<cond_expr*>(n1);
		cond_expr* cond2 = static_cast<cond_expr*>(n2);
		if (!beq(cond1->test, cond2->test)) return false;
		return neq(cond1->pass, cond2->pass) && neq(cond1->fail, cond2->fail);
		break;
	}
}

//Boolean expression equality
bool beq(bool_expr* b1, bool_expr* b2) {
	if (b1->type != b2->type) return false;
	switch (b1->type) {
	case (bool_expr_type::boolean):
		boolean_expr* bool1 = static_cast<boolean_expr*>(b1);
		boolean_expr* bool2 = static_cast<boolean_expr*>(b2);
		return bool1->val = bool2->val;
		break;
	case (bool_expr_type::relational):
		relation_expr* rel1 = static_cast<relation_expr*>(b1);
		relation_expr* rel2 = static_cast<relation_expr*>(b2);
		if (rel1->op != rel2->op) return false;
		if (neq(rel1->first, rel2->first) && neq(rel1->second, rel2->second)) return true;
		else return false;
		break;
	case (bool_expr_type::logic):
		logic_expr* log1 = static_cast<logic_expr*>(b1);
		logic_expr* log2 = static_cast<logic_expr*>(b2);
		if (log1->op != log2->op) return false;
		if (beq(log1->first, log2->first) && beq(log1->second, log2->second)) return true;
		else return false;
		break;
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