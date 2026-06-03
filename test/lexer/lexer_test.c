#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer_test.h"
#include "../../src/lexer.h"



int main(){


	bool all_pass = true;

	FILE* input1 = fopen("input1.c", "r");
	FILE* ans1 = fopen("ans1.txt", "r");

	all_pass = all_pass & test_lex(input1, ans1, 1);

	fclose(input1);
	fclose(ans1);

	return (int) all_pass;

}



bool test_lex(FILE* input, FILE* ans, int test_num){
	

	bool test_pass = true;


	token_t start_actual_token;
	start_actual_token.type = -1;

	token_t start_expected_token;
	char buffer[128];
	start_expected_token = (token_t){-1, buffer, -1, -1};


	test_pass = test_pass & test_token_amount(input, ans, start_actual_token, start_expected_token);	
	test_pass = test_pass & test_all_tokens_equal(input, ans, start_actual_token, start_expected_token, test_num);


	if(!test_pass){
		output_expected_vs_actual_strings(input, ans, start_actual_token, start_expected_token);
	}


	return test_pass;

}


bool test_token_amount(FILE* input, FILE* ans, token_t current_actual_token, token_t current_expected_token){


	bool test_pass = true;

	int current_actual_token_count = count_actual_tokens(input, current_actual_token);
	int current_expected_token_count = count_expected_tokens(ans, current_expected_token);


	if(current_actual_token_count < current_expected_token_count){


		printf("Test failed! Actual number of tokens (%d) less than expected (%d).\n", current_actual_token_count, current_expected_token_count);
		test_pass = false;


	}
	else if(current_actual_token_count > current_expected_token_count){


		printf("Test failed! Actual number of tokens (%d) greated than expected (%d).\n", current_actual_token_count, current_expected_token_count);
		test_pass = false;


	}

	rewind(ans);
	rewind(input);


	return test_pass;

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


	//Do not set test_pass to false. It must be passed in as is.

	bool wrong_type = false;
	bool wrong_string = false;
	bool wrong_line = false;
	bool wrong_column = false;



	if(current_actual_token.type != current_expected_token.type){

		wrong_type = true;

	}
	if(strcmp(current_actual_token.string, current_expected_token.string)){

		wrong_string = true;

	}
	if(current_actual_token.line != current_expected_token.line){

		wrong_line = true;

	}
	if(current_actual_token.column != current_expected_token.column){

		wrong_column = true;

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
