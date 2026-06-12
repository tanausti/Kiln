#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../../src/parser.h"
#include "../../src/ir.h"
#include "ir_test.h"

#define MAX_LINE_LENGTH 1024


int main(){


	FILE* input1 = fopen("input1.c", "r");
	FILE* ans1 = fopen("ans1.ll", "r");
	
	test_llvm(input1, ans1, 1);

	fclose(input1);
	fclose(ans1);



}




bool test_llvm(FILE* input, FILE* ans, int test_num){

	FILE* text_llvm_stream = init_memstream();

	ast_node_t ast = build_ast(input);
	generate_llvm_to_file(text_llvm_stream, ast);

	bool test_pass = true;
	test_pass = test_pass & test_llvm_length(text_llvm_stream, ans);
	rewind(text_llvm_stream);
	rewind(ans);
	test_pass = test_pass & test_llvm_contents(text_llvm_stream, ans);

	if(!test_pass){

		printf("Test %d failed!\n", test_num);

		printf("\n");

		rewind(ans);
		printf("Expected LLVM:\n");
		print_file_with_line_numbers(ans);


		rewind(text_llvm_stream);
		printf("Actual LLVM:\n");
		print_file_with_line_numbers(text_llvm_stream);

	}
	else{

		printf("Test %d passed!\n", test_num);


	}


	fclose(text_llvm_stream);

	return test_pass;


}




FILE* init_memstream(){


	char* buffer = NULL;
	size_t size = 0;

	FILE* stream = open_memstream(&buffer, &size);

	fflush(stream);

	return stream;
	


}

bool test_llvm_length(FILE* input, FILE* ans){

	int input_line_num = count_lines(input);
	int ans_line_num = count_lines(ans);

	bool pass = (input_line_num == ans_line_num);

	if(!pass){

		printf("Actual number of lines (%d) did not match expected (%d). \n", input_line_num, ans_line_num);	

	}


	return pass;


}


int count_lines(FILE* input){

	
	int line_num = 1;

	char line[MAX_LINE_LENGTH];

	while(fgets(line, MAX_LINE_LENGTH, input)){

		line_num++;

	}

	return line_num;

}





bool test_llvm_contents(FILE* input, FILE* ans){


	bool test_pass = true;

	char input_line[MAX_LINE_LENGTH];
	char ans_line[MAX_LINE_LENGTH];

	int line_num = 1;
	
	while(fgets(input_line, MAX_LINE_LENGTH, input) && fgets(ans_line, MAX_LINE_LENGTH, ans)){
					

		if(strncmp(input_line, ans_line, MAX_LINE_LENGTH) != 0){

			printf("Expected LLVM did not match actual on line %d.\n", line_num);
			
			test_pass = false;

		}



		line_num++;



	}

	return test_pass;

}




void print_file_with_line_numbers(FILE* file){


	char line[MAX_LINE_LENGTH];

	int line_num = 1;

	while(fgets(line, MAX_LINE_LENGTH, file)){

		printf("%d: %s", line_num, line);
		line_num++;

	}


}
