#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../src/lexer.h"


void test_lex(FILE* input, FILE* ans, int test_num);
bool test_token_amount(FILE* input, FILE* ans, token_t current_actual_token, token_t current_expected_token);
bool test_all_tokens_equal(FILE* input, FILE* ans, token_t current_actual_token, token_t current_expected_token, int test_num);
bool tokens_equal(token_t current_actual_token, token_t current_expected_token, bool test_pass, int token_num, int test_num);
int count_actual_tokens(FILE* input, token_t current_actual_token);
int count_expected_tokens(FILE* ans, token_t current_expected_token);
void output_expected_vs_actual_strings(FILE* input, FILE* ans, token_t current_actual_token, token_t current_expected_token);

int main(){

	FILE* input1 = fopen("input1.c", "r");
	FILE* ans1 = fopen("ans1.txt", "r");

	test_lex(input1, ans1, 1);

}



void test_lex(FILE* input, FILE* ans, int test_num){
	

	token_t current_actual_token;
	current_actual_token.type = -1;

	token_t current_expected_token;
	char buffer[128];
	current_expected_token = (token_t){-1, buffer, -1, -1};

	bool test_pass = true;

	test_pass = test_token_amount(input, ans, current_actual_token, current_expected_token);	
	test_pass = test_all_tokens_equal(input, ans, current_actual_token, current_expected_token, test_num);

	output_expected_vs_actual_strings(input, ans, current_actual_token, current_expected_token);


	fclose(input);
	fclose(ans);



}


bool test_token_amount(FILE* input, FILE* ans, token_t current_actual_token, token_t current_expected_token){



	int current_actual_token_count = count_actual_tokens(input, current_actual_token);
	int current_expected_token_count = count_expected_tokens(ans, current_expected_token);


	if(current_actual_token_count < current_expected_token_count){


		printf("Test failed! Actual number of tokens (%d) less than expected (%d).\n", current_actual_token_count, current_expected_token_count);


	}
	else if(current_actual_token_count > current_expected_token_count){


		printf("Test failed! Actual number of tokens (%d) greated than expected (%d).\n", current_actual_token_count, current_expected_token_count);


	}

	rewind(ans);
	rewind(input);




}

bool test_all_tokens_equal(FILE* input, FILE* ans, token_t current_actual_token, token_t current_expected_token, int test_num){

	bool test_pass = true;

	int token_num = 0;
	int lc[2] = {1, 0};

	while(current_actual_token.type != TOK_EOF && fscanf(ans, "%d %127s %d %d", &current_expected_token.type, current_expected_token.string, &current_expected_token.line, &current_expected_token.column))
	{

		token_num++;

		current_actual_token = next_token(input, lc);
		test_pass = test_pass & tokens_equal(current_actual_token, current_expected_token, test_pass, token_num, test_num);

	}

	rewind(ans);
	rewind(input);

	return test_pass;



}


bool tokens_equal(token_t current_actual_token, token_t current_expected_token, bool test_pass, int token_num, int test_num){



	int wrong_type = 0;
	int wrong_string = 0;
	int wrong_line = 0;
	int wrong_column = 0;



	if(current_actual_token.type != current_expected_token.type){

		wrong_type = 1;

	}
	if(strcmp(current_actual_token.string, current_expected_token.string)){

		wrong_string = 1;

	}
	if(current_actual_token.line != current_expected_token.line){

		wrong_line = 1;

	}
	if(current_actual_token.column != current_expected_token.column){

		wrong_column = 1;

	}


	if(wrong_type || wrong_string || wrong_line || wrong_column){


		if(test_pass){

			test_pass = false;

			printf("\n");
			printf("------------------------------------------------------\n");
			printf("Test %d failed! One or more tokens did not match expected.\n", test_num);
			printf("------------------------------------------------------\n");
			printf("\n");

		}


		printf("\n");	
		printf("Token %d did not match expected:\n", token_num);	


	}	


	if(wrong_type){

		printf("Expected token_type_t: %d | Actual: %d\n", current_expected_token.type, current_actual_token.type);

	}	
	if(wrong_string){

		printf("Expected string: \"%s\" | Actual: \"%s\"\n", current_expected_token.string, current_actual_token.string);

	}
	if(wrong_line){

		printf("Expected line: %d | Actual: %d\n", current_expected_token.line, current_actual_token.line);

	}
	if(wrong_column){

		printf("Expected column: %d | Actual: %d\n", current_expected_token.column, current_actual_token.column);

	}
	


	return test_pass;



}




int count_actual_tokens(FILE* input, token_t current_actual_token){

	int count = 0;
	
	int lc[2] = {1,0};

	while(current_actual_token.type != TOK_EOF)
	{

		current_actual_token = next_token(input, lc);

		count++;

	}

	return count;




}


int count_expected_tokens(FILE* ans, token_t current_expected_token){

	int count = 0;
	
	while(fscanf(ans, "%d %127s %d %d", &current_expected_token.type, current_expected_token.string, &current_expected_token.line, &current_expected_token.column) == 4)
	{

		count++;

	}

	return count;




}



void output_expected_vs_actual_strings(FILE* input, FILE* ans, token_t current_actual_token, token_t current_expected_token){


	int token_num = 0;
	int lc[2] = {1, 0};

	printf("\n");
	printf("ALL TOKEN STRING VALUES: \n");
	printf("\n");

	while(current_actual_token.type != TOK_EOF && fscanf(ans, "%d %127s %d %d", &current_expected_token.type, current_expected_token.string, &current_expected_token.line, &current_expected_token.column))
	{

		token_num++;

		current_actual_token = next_token(input, lc);

		printf("Token %d | Expected: %s | Actual: %s \n", token_num, current_expected_token.string, current_actual_token.string);

	}






}
