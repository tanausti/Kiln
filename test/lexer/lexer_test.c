#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/lexer.h"


void test(FILE* input, FILE* ans);
void tokens_equal(token_t actual_token, token_t expected_token, int token_count);

int main(){

	FILE* input1 = fopen("input1.c", "r");
	FILE* ans1 = fopen("ans1.txt", "r");

	test(input1, ans1);

}


void test(FILE* input, FILE* ans){
	

	int lc[2] = {1, 1};
	int token_count = 0;
	int pass = 1;

	
	
	token_t actual_token;
	actual_token.type = -1;

	token_t expected_token;

	char buffer[128];
	expected_token.string = buffer;

		

int x = 0;
	
	while(actual_token.type != TOK_EOF && fscanf(ans, "%d %127s %d %d", &expected_token.type, expected_token.string, &expected_token.line, &expected_token.column) == 4)
	{

		tokens_equal(actual_token, expected_token, token_count);

		token_count++;

	}


	if(actual_token.type != TOK_EOF){
		//TODO add seprate token counts
		printf("Test failed! Actual number of tokens greater than expected.\n");

	}

	//{
		/*
		//TODO check seond to last edge case
	if(fscanf(ans, "%d %128s %d %d", expected_token.type, expected_token.string, expected_token.line, expected_token.column != 4));

		printf("Test failed! Actual number of tokens less than expected.\n");

	}*/

	fclose(input);
	fclose(ans);


	





}






void tokens_equal(token_t actual_token, token_t expected_token, int token_count){


	int wrong_type = 0;
	int wrong_string = 0;
	int wrong_line = 0;
	int wrong_column = 0;


	if(actual_token.type != expected_token.type){

		wrong_type = 1;

	}
	/*
	if(!strcmp(actual_token.string, expected_token.string)){

		wrong_string = 1;

	}*/
	if(actual_token.line != expected_token.line){

		wrong_line = 1;

	}
	if(actual_token.column != expected_token.column){

		wrong_column = 1;

	}

	
	if(wrong_type || wrong_string || wrong_line || wrong_column){

		printf("Test failed! Token %d did not match expected.\n", token_count);
		

		if(wrong_type){

			printf("Expected token_type_t: %d | Actual: %d\n", expected_token.type, actual_token.type);

		}
		/*
		if(wrong_string){

			printf("Expected string: %s | Actual: %s\n", expected_token.string, actual_token.string);

		}*/
		if(wrong_line){

			printf("Expected line: %d | Actual: %d\n", expected_token.line, actual_token.line);

		}
		if(wrong_column){

			printf("Expected column: %d | Actual: %d\n", expected_token.column, actual_token.column);

		}
	}


}
