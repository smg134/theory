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

	bool operator==(num_expr* n) {
		if (this->type != n->type) return false;
		switch (this->type) {
		case (num_expr_type::integer):
			return *static_cast<int_expr*>(this) == static_cast<int_expr*>(n);
			break;
		case (num_expr_type::argument):
			return *static_cast<arg_expr*>(this) == static_cast<arg_expr*>(n);
			break;
		case (num_expr_type::arithmetic):
			return *static_cast<arith_expr*>(this) == static_cast<arith_expr*>(n);
			break;
		case (num_expr_type::conditional):
			return *static_cast<cond_expr*>(this) == static_cast<cond_expr*>(n);
			break;
		}
	}
};

//Integer numeric expression
struct int_expr : num_expr {
	int val;

	int_expr(int v)
		: num_expr(integer), val(v) {}

	bool operator==(int_expr* n) {
		if (this->val == n->val) return true;
		else return false;
	}
};

//Argument numeric expression
struct arg_expr : num_expr {
	int args;

	arg_expr(int a)
		: num_expr(argument), args(a) {}

	bool operator==(arg_expr* n) {
		if (this->args == n->args) return true;
		else return false;
	}
};

//Arithmetic numeric expression
struct arith_expr : num_expr {
	arith_op op;
	num_expr* first;
	num_expr* second;

	arith_expr(arith_op o, num_expr* f, num_expr* s)
		: num_expr(arithmetic), op(o), first(f), second(s) {}

	bool operator==(arith_expr* n) {
		if (this->first == n->first
			&& this->second == n->second
			&& this->op == n->op) return true;
		else return false;
	}
};

//Conditional numeric expression
struct cond_expr : num_expr {
	bool_expr* test;
	num_expr* pass;
	num_expr* fail;

	cond_expr(bool_expr* t, num_expr* p, num_expr* f)
		: num_expr(conditional), test(t), pass(p), fail(f) {}

	bool operator==(cond_expr* n) {
		if (this->test == n->test
			&& this->pass == n->pass
			&& this->fail == n->fail) return true;
		else return false;
	}
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

	bool operator==(bool_expr* b) {
		if (this->type != b->type) return false;
		switch (this->type) {
		case (bool_expr_type::boolean):
			return *static_cast<boolean_expr*>(this) == static_cast<boolean_expr*>(b);
			break;
		case (bool_expr_type::relational):
			return *static_cast<relation_expr*>(this) == static_cast<relation_expr*>(b);
			break;
		case (bool_expr_type::logic):
			return *static_cast<logic_expr*>(this) == static_cast<logic_expr*>(b);
			break;
		}
	}
};

//Bool boolean expression...
//terrible naming convention :(
struct boolean_expr : bool_expr {
	bool val;

	boolean_expr(bool v)
		: bool_expr(boolean), val(v) {}

	bool operator==(boolean_expr* b) {
		if (this->val == b->val) return true;
		else return false;
	}
};

//Relational boolean expression
struct relation_expr : bool_expr {
	rel_op op;
	num_expr* first;
	num_expr* second;

	relation_expr(rel_op o, num_expr* f, num_expr* s)
		: bool_expr(relational), op(o), first(f), second(s) {}

	bool operator==(relation_expr* b) {
		if (this->first == b->first
			&& this->second == b->second
			&& this->op == b->op) return true;
		else return false;
	}
};

//Logic boolean expression
struct logic_expr : bool_expr {
	logic_op op;
	bool_expr* first;
	bool_expr* second;

	logic_expr(logic_op o, bool_expr* f, bool_expr* s)
		: bool_expr(logic), op(o), first(f), second(s) {}

	bool operator==(logic_expr* b) {
		if (this->first == b->first
			&& this->second == b->second
			&& this->op == b->op) return true;
		else return false;
	}
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
	return n1 == n2;
}

//Boolean expression equality
bool beq(bool_expr* b1, bool_expr* b2) {
	return b1 == b2;
}

//Program identities
Program pOpt(Program* p) {
	p->body = nOpt(p->body);
	return *p;
}

//Numeric identities
num_expr* nOpt(num_expr* n) {
	if (n->type != num_expr_type::arithmetic) return n;
	*n = arithOpt(static_cast<arith_expr*>(n));
}

num_expr arithOpt(arith_expr* n) {
	switch (n->op) {
	case (arith_op::add):
		break;
	case (arith_op::sub):
		break;
	case (arith_op::mul):
		break;
	case (arith_op::divide):
		break;
	case(arith_op::mod):
		break;
	}
}

//Boolean identities
bool_expr* bOpt(bool_expr* b) {

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