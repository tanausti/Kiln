#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_harness.h"


#define MAX_LINE_LENGTH 1024






void test_all_inputs(FILE* (*get_expected_out)(int), FILE* (*get_actual_out)(int), int num_tests){


	for(int test_num = 1; test_num <= num_tests; test_num++){

		test_input(get_expected_out, get_actual_out, test_num);

	}

}



void test_input(FILE* (*get_expected_out)(int), FILE* (*get_actual_out)(int), int test_num){


	FILE* expected_out = get_expected_out(test_num);
	FILE* actual_out = get_actual_out(test_num);

	rewind(expected_out);
	rewind(actual_out);


	bool test_pass = test_expected_vs_actual_out(expected_out, actual_out, test_num);


	fclose(expected_out);
	fclose(actual_out);



}



bool test_expected_vs_actual_out(FILE* expected_out, FILE* actual_out, int test_num){

		
	bool length_pass = test_expected_vs_actual_out_length(expected_out, actual_out);
	bool content_pass = test_expected_vs_actual_contents(expected_out, actual_out);

	bool test_pass = length_pass & content_pass;


	if(!test_pass){

		printf("Test %d failed!\n\n", test_num);
		
		if(!length_pass){

			printf("Actual length did not match expected.\n");

		}
		if(!content_pass){

			printf("Actual contents did not match expected.\n");

		}

		printf("\n");

		printf("Expected:\n");
		print_file_with_line_numbers(expected_out);

		printf("Actual:\n");
		print_file_with_line_numbers(actual_out);

	}
	else{

		printf("Test %d passed!\n", test_num);


	}

	printf("\n");

	return test_pass;

}



bool test_expected_vs_actual_out_length(FILE* expected_out, FILE* actual_out){

	int expected_line_count = count_lines(expected_out);
	int actual_line_count = count_lines(actual_out);

	bool test_pass = (expected_line_count == actual_line_count);


	return test_pass;


}





bool test_expected_vs_actual_contents(FILE* expected_out, FILE* actual_out){


	bool test_pass = true;

	char expected_line[MAX_LINE_LENGTH];
	char actual_line[MAX_LINE_LENGTH];

	int line_num = 1;
	
	while(fgets(expected_line, MAX_LINE_LENGTH, expected_out) && fgets(actual_line, MAX_LINE_LENGTH, actual_out)){
					

		if(strncmp(expected_line, actual_line, MAX_LINE_LENGTH) != 0){

			test_pass = false;

		}

		line_num++;



	}

	rewind(expected_out);
	rewind(actual_out);

	return test_pass;

}




int count_lines(FILE* file){

	
	int line_count = 0;

	char line[MAX_LINE_LENGTH];

	while(fgets(line, MAX_LINE_LENGTH, file)){

		line_count++;

	}

	rewind(file);

	return line_count;

}



void print_file_with_line_numbers(FILE* file){


	char line[MAX_LINE_LENGTH];

	int line_num = 1;

	while(fgets(line, MAX_LINE_LENGTH, file)){

		printf("%d: %s", line_num, line);
		line_num++;

	}

	rewind(file);


}




int get_num_tests(int argc, char* argv[]){


	if(argc == 1){

		printf("Please provide number of input files in the command line!\n");
		return -1;


	}
	else if(argc > 2){

		printf("Only one argument accepted!\n");
		return -1;

	}

	int num_tests = atoi(argv[1]);

	return num_tests;

}




