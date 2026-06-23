#ifndef COMPARE_FILES_C
#define COMPARE_FILES_C

#include <stdbool.h>
#include <stdio.h>


void test_all_inputs(FILE* (*get_expected_out)(int), FILE* (*get_actual_out)(int), int num_tests);
void test_input(FILE* (*get_expected_out)(int), FILE* (*get_actual_out)(int), int test_num);
bool test_expected_vs_actual_out(FILE* expected_out, FILE* actual_out, int test_num);
bool test_expected_vs_actual_out_length(FILE* expected_out, FILE* actual_out);
bool test_expected_vs_actual_contents(FILE* expected_out, FILE* actual_out);
int count_lines(FILE* file);
void print_file_with_line_numbers(FILE* file);
int get_num_tests(int argc, char* argv[]);


#endif
