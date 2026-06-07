

#ifndef PARSER_TEST_H
#define PARSER_TEST_H

#include <stdio.h>
#include <stdbool.h>

bool test_ast(FILE* input, FILE* ans, int test_num);
void print_file_line_numbers(FILE* file);

#endif
