#include "parser_test.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024

int main(){

	
	FILE* input1 = fopen("input1.txt", "r");
	FILE* ans1 = fopen("ans1.txt", "r");
	test_ast(input1, ans1, 1);


	fclose(input1);
	fclose(ans1);

}





bool test_ast(FILE* input, FILE* ans, int test_num){


	bool test_pass = true;

	char exp_line[MAX_LINE_LENGTH];
	char ans_line[MAX_LINE_LENGTH];


	int line_num = 1;
	
	while(fgets(exp_line, MAX_LINE_LENGTH, input) && fgets(ans_line, MAX_LINE_LENGTH, ans)){
			
			

		if(strncmp(exp_line, ans_line, MAX_LINE_LENGTH) != 0){

			printf("Expected AST string representation did not match actual on line %d.\n", line_num);
			
			test_pass = false;

		}



		line_num++;



	}

	if(!test_pass){

		printf("\n");

		rewind(ans);
		printf("Expected AST string representation:\n");
		print_file_line_numbers(ans);


		rewind(input);
		printf("Actual AST string representation:\n");
		print_file_line_numbers(input);

	}
	else{

		printf("Test %d passed!\n", test_num);


	}


	return test_pass;

}



void print_file_line_numbers(FILE* file){


	char line[MAX_LINE_LENGTH];

	int line_num = 1;

	while(fgets(line, MAX_LINE_LENGTH, file)){

		printf("%d: %s", line_num, line);
		line_num++;

	}


}
