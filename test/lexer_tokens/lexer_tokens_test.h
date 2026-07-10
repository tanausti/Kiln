#ifndef LEXER_TEST_H
#define LEXER_TEST_H

#include <stdbool.h>

FILE* lexer_get_expected_out(int test_num);
FILE* lexer_get_actual_out(int test_num);
void print_tokens(FILE* input, FILE* out);
void assign_enum_str(token_t current_token, char* enum_str);
	



#endif
