#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdnoreturn.h>

#include "container.h"
#include "tokenizer.h"
#include "parser.h"

// unitily.c
noreturn void error(char *fmt, ...) __attribute__((format(printf,1,2)));
void debug(char *fmt, ...) __attribute__((format(printf,1,2)));

// test_utility.c
void expect(int line, int expected, int actual);

// debug_util.c
void print_ast(Node *node);

// codegen.c
void generate_assembly_code(Node* node);



