#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int token;				// current token
char *src, *old_src;	// pointer to source code string
int poolsize;			// defaults size of text/data/stack
int line;				// line number

int *text,				// text segment
	*old_text,			// for dump text segment
	*stack;				// stack
char *data;				// data segment

/// 用于词法分析，获取下一个标记，它将自动忽略空白字符
void next() {
	token = *src++;
	return;
}

/// 用于解析一个表达式
void expression(int level) {
	// do nothing
}

/// 语法分析的入口，分析整个 C 语言程序
void program() {
	next();
	while (token > 0) {
		printf("token is %c\n", token);
		next();
	}
}

/// 虚拟机的入口，用于解释目标代码
int eval() { // do nothing
	return 0;
}

int main(int argc, char *argv[]) {
	int i, fd;
	argc--;
	argv++;
	
	poolsize = 256 * 1024; // arbitrary size;
	line = 1;
	
	if ((fd = open(*argv, 0)) < 0) {
		printf("could not open(%s)\n", *argv);
		return -1;
	}
	
	if (!(src = old_src = malloc(poolsize))) {
		printf("could not malloc(%d) for source area\n", poolsize);
		return -1;
	}
	
	if ((i = read(fd, src, poolsize-1)) <= 0) {
		printf("read() return %d\n", i);
		return -1;
	}
	
	src[i] = 0; // add EOF character
	close(fd);
	
	// allocate memory for virtual machine
	if (!(text = old_text = malloc(poolsize))) {
		printf("could not malloc(%d) for text area\n", poolsize);
		return -1;
	}
	
	if (!(data = malloc(poolsize))) {
		printf("could not malloc(%d) for data area\n", poolsize);
		return -1;
	}
	
	if (!(stack = malloc(poolsize))) {
		printf("could not malloc(%d) for stack area\n", poolsize);
		return -1;
	}
	
	memset(text, 0, poolsize);
	memset(data, 0, poolsize);
	memset(stack, 0, poolsize);
	
	program();
	return eval();
}