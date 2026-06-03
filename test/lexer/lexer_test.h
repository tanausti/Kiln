#ifndef LEXER_TEST_H
#define LEXER_TEST_H

#include <stdbool.h>

bool test_lex(FILE* input, FILE* ans, int test_num);
bool test_token_amount(FILE* input, FILE* ans, token_t current_actual_token, token_t current_expected_token);
bool test_all_tokens_equal(FILE* input, FILE* ans, token_t current_actual_token, token_t current_expected_token, int test_num);
bool tokens_equal(token_t current_actual_token, token_t current_expected_token, bool test_pass, int token_num, int test_num);
int count_actual_tokens(FILE* input, token_t current_actual_token);
int count_expected_tokens(FILE* ans, token_t current_expected_token);
void output_expected_vs_actual_strings(FILE* input, FILE* ans, token_t current_actual_token, token_t current_expected_token);


#endif
