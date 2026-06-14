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
	FILE* input2 = fopen("input2.c", "r");
	FILE* ans2 = fopen("ans2.ll", "r");
	FILE* input3 = fopen("input3.c", "r");
	FILE* ans3 = fopen("ans3.ll", "r");

	test_llvm(input1, ans1, 1);
	test_llvm(input2, ans2, 2);
	test_llvm(input3, ans3, 3);

	fclose(input1);
	fclose(ans1);
	fclose(input2);
	fclose(ans2);
	fclose(input3);
	fclose(ans3);



}




bool test_llvm(FILE* input, FILE* ans, int test_num){

	ast_node_t ast = build_ast(input);
 
	char filename[256];
	snprintf(filename, sizeof(filename), "test%d.ll", test_num);
	generate_llvm_to_file(filename, ast);
	FILE* generated_llvm_file = fopen(filename, "r");

	bool test_pass = true;
	test_pass = test_pass & test_llvm_length(generated_llvm_file, ans);
	rewind(generated_llvm_file);
	rewind(ans);
	test_pass = test_pass & test_llvm_contents(generated_llvm_file, ans);

	if(!test_pass){

		printf("Test %d failed!\n", test_num);

		printf("\n");

		rewind(ans);
		printf("Expected LLVM:\n");
		print_file_with_line_numbers(ans);


		rewind(generated_llvm_file);
		printf("Actual LLVM:\n");
		print_file_with_line_numbers(generated_llvm_file);

	}
	else{

		printf("Test %d passed!\n", test_num);


	}


	fclose(generated_llvm_file);

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



