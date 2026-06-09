#include "../../src/parser.h"
#include "../../src/print_ast.h"

#include "parser_test.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024

int main(){

	
	FILE* input1 = fopen("input1.c", "r");
	FILE* ans1 = fopen("ans1.txt", "r");

	test_ast(input1, ans1, 1);


	fclose(input1);
	fclose(ans1);

}


FILE* get_stream(FILE* input){


	char* buffer = NULL;
	size_t size = 0;

	FILE* stream = open_memstream(&buffer, &size);

	fflush(stream);

	return stream;
	


}


bool test_ast(FILE* input, FILE* ans, int test_num){

	ast_node_t ast = build_ast(input);

	FILE* text_ast_stream = get_stream(input);
	print_ast(text_ast_stream, ast);
	rewind(text_ast_stream);



	bool test_pass = true;

	char input_line[MAX_LINE_LENGTH];
	char ans_line[MAX_LINE_LENGTH];


	int line_num = 1;

	
	while(fgets(input_line, MAX_LINE_LENGTH, text_ast_stream) && fgets(ans_line, MAX_LINE_LENGTH, ans)){
		
		//TODO:	
		//add thing like tests 1..20 dont work. not 20 lines
		//if 1 ends before other unqiue message
			

		if(strncmp(input_line, ans_line, MAX_LINE_LENGTH) != 0){

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


		rewind(text_ast_stream);
		printf("Actual AST string representation:\n");
		print_file_line_numbers(text_ast_stream);

	}
	else{

		printf("Test %d passed!\n", test_num);


	}


	fclose(text_ast_stream);

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
