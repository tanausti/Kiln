#ifndef PARSE_ERROR_TEST_H
#define PARSE_ERROR_TEST_H

#include <stdio.h>


FILE* parse_error_get_expected_out(int test_num);
FILE* parse_error_get_actual_out(int test_num);
void console_output(FILE* input);

#endif
