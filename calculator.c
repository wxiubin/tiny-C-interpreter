#include <stdio.h>

#include "calculator.h"

typedef struct {
	const char *str;
	int err_type;
} slm_expr;

#define TRY(func) func; if (e->err_type) return 0;
#define THROW(error) e->err_type = error; return 0;

int stoi(slm_expr *e);
int expr(slm_expr *e);
int term(slm_expr *e);
int bracket(slm_expr *e);

int stoi(slm_expr *e) {
	char c = *e->str;
	short minus = 0;
	if (c == '-') {
		minus = 1;
		e->str++;
		c = *e->str;
	}
	if (c < '0' || c > '9') {
		THROW(CC_ERR_EXPECT_DIGIT)
	}
	int result = c - '0';
	e->str++;
	if (minus == 1) {
		result *= -1;
	}
	return result;
}

int bracket(slm_expr *e) {
	return stoi(e);
//    if (**str != '(') {
//        return stoi(str);
//    }
//    (*str)++;
//    int result = 0;
//    do {
//        result = expr(str);
//    } while (**str == ')');
//
//    return result;
}

int term(slm_expr *e) {
	int result = bracket(e);
	char op = *e->str;
	while (op == '*' || op == '/' || op == '%') {
		(e->str)++;
		if (op == '*') {
			result *= bracket(e);
		} else if (op == '/') {
			result /= bracket(e);
		} else if (op == '%') {
			result %= bracket(e);
		} else {
			break;
		}
		break;
	}
	return result;
}

int expr(slm_expr *e) {
	int result = term(e);
	char op = *e->str;
	while (op == '+' || op == '-') {
		(e->str)++;
		if (op == '+') {
			result = result + term(e);
		} else {
			result = result - term(e);
		}
		break;
	}
	return result;
}

int slm_eval(const char *str, int *err_type) {
	slm_expr e;
	e.err_type = CC_ERR_NONE;
	e.str = str;
	int result = expr(&e);
	if (err_type) *err_type = e.err_type;
	return result;
}

int main(int argc, char *argv[]) {
	const char *test = "-5-2*3";
	int error = 0;
	printf("result: %d\n", slm_eval(test, &error));
	return error;
}
