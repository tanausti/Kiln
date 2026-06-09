#ifndef IR_TEST_H
#define IR_TEST_H


#include <stdio.h>
#include <stdbool.h>

bool test_llvm(FILE* input, FILE* ans, int test_num);
FILE* init_memstream();
bool test_llvm_length(FILE* input, FILE* ans);
int count_lines(FILE* input);
bool test_llvm_contents(FILE* input, FILE* ans);
void print_file_with_line_numbers(FILE* file);

#endif
