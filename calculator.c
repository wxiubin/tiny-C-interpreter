#include <stdio.h>
#include <ctype.h>

#include "calculator.h"

enum {
	CC_TOKEN_UNKNOWN = 0,
	CC_TOKEN_DIGITS,    // 数字
	CC_TOKEN_ADD,       // 加法
	CC_TOKEN_SUB,       // 减法
	CC_TOKEN_MINUS = CC_TOKEN_SUB,  // 负号
	CC_TOKEN_MUL,       // 乘法
	CC_TOKEN_DIV,       // 除法
	CC_TOKEN_MOD,       // 取余
	CC_TOKEN_OPEN,      // 后括号
	CC_TOKEN_CLOSE,     // 后括号
	CC_TOKEN_END        // 结束
};

typedef struct {
	int type;
	int value;
} cc_token;

typedef struct {
	cc_token token;
	const char *str;
	int err_type;
} cc_expr;

#define TRY(func) func; if (e->err_type) return 0;
#define THROW(error) { e->err_type = error; return 0; }

int stoi(cc_expr *e);
int expr(cc_expr *e);
int term(cc_expr *e);
void next(cc_expr *e);
int bracket(cc_expr *e);

void next(cc_expr *e) {
	char c = *e->str;
	while (isspace(c)) {
		c = *(++(e->str));
	}
	if (c == '\0') {
		e->token.type = CC_TOKEN_END;
		e->token.value = 0;
	} else if (isdigit(c)) {
		e->token.type = CC_TOKEN_DIGITS;
		e->token.value = 0;
		do {
			e->token.value = e->token.value * 10 + c - '0';
			c = *(++(e->str));
		} while (isdigit(c));
	} else {
		switch (c) {
			case '+': e->token.type = CC_TOKEN_ADD; c = *(e->str++); break;
			case '-': e->token.type = CC_TOKEN_SUB; c = *(e->str++); break;
			case '*': e->token.type = CC_TOKEN_MUL; c = *(e->str++); break;
			case '/': e->token.type = CC_TOKEN_DIV; c = *(e->str++); break;
			case '%': e->token.type = CC_TOKEN_MOD; c = *(e->str++); break;
			case '(': e->token.type = CC_TOKEN_OPEN; c = *(e->str++); break;
			case ')': e->token.type = CC_TOKEN_CLOSE; c = *(e->str++); break;
			default:  e->token.type = CC_TOKEN_UNKNOWN; e->err_type = CC_ERR_EXPECT_UNKNOW_TOKEN; break;
		}
	}
}

int stoi(cc_expr *e) {
	short minus = 0;
	if (e->token.type == CC_TOKEN_MINUS) {
		minus = 1;
		TRY(next(e));
	}
	if (e->token.type != CC_TOKEN_DIGITS) {
		THROW(CC_ERR_EXPECT_DIGIT)
	}
	int result = e->token.value;
	TRY(next(e));
	if (minus == 1) {
		result *= -1;
	}
	return result;
}

int bracket(cc_expr *e) {
	return stoi(e);
}

int term(cc_expr *e) {
	int result = TRY(bracket(e));

	int op = e->token.type;
	while (op == CC_TOKEN_MUL || op == CC_TOKEN_DIV || op == CC_TOKEN_MOD) {
		TRY(next(e));
		if (op == CC_TOKEN_MUL) {
			result *= TRY(bracket(e));
		} else if (op == CC_TOKEN_DIV) {
			int t = TRY(bracket(e));
			if (t == 0)
				THROW(CC_ERR_EXPECT_DIVISION_BY_ZERO);
			result /= t;
		} else if (op == CC_TOKEN_MOD) {
			int t = TRY(bracket(e));
			if (t == 0)
				THROW(CC_ERR_EXPECT_REMAINDER_BY_ZERO);
			result %= t;
		} else {
			break;
		}
		op = e->token.type;
	}
	return result;
}

int expr(cc_expr *e) {
	int result = TRY(term(e));
	int op = e->token.type;

	while (op == CC_TOKEN_ADD || op == CC_TOKEN_SUB) {
		TRY(next(e));
		if (op == CC_TOKEN_ADD) {
			result += TRY(term(e));
		} else {
			result -= TRY(term(e));
		}
		op = e->token.type;
	}
	return result;
}

int slm_eval(const char *str, int *err_type) {
	cc_expr e;
	e.err_type = CC_ERR_NONE;
	e.str = str;
	next(&e);
	int result = expr(&e);
	if (err_type) *err_type = e.err_type;
	return result;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("请输入 eg. `2*3`");
		return 0;
	}
	const char *test = argv[1];
	int error = 0;
	printf("result: %d\n", slm_eval(test, &error));
	return error;
}
