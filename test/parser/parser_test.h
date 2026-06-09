

#ifndef PARSER_TEST_H
#define PARSER_TEST_H

#include <stdio.h>
#include <stdbool.h>


bool test_ast(FILE* input, FILE* ans, int test_num);
FILE* get_stream(FILE* input);
bool test_ast_length(FILE* input, FILE* ans, int test_num);
int count_lines(FILE* input);
bool test_ast_contents(FILE* input, FILE* ans, int test_num);
void print_file_with_line_numbers(FILE* file);


#endif
