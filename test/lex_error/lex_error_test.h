#ifndef LEX_ERROR_TEST_H
#define LEX_ERROR_TEST_H

#include <stdio.h>

FILE* lex_error_get_expected_out(int test_num);
FILE* lex_error_get_actual_out(int test_num);
void console_output(FILE* input);


#endif
