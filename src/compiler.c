#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ir.h"
#include "parser.h"
#include "validate_ast.h"

int main(int argc, char* argv[]){

	if(argc == 0){

		fprintf(stderr, "Error: Please provide the input file that you would like to compile!\n");
		return 1;

	}
	else if(argc > 2){

		fprintf(stderr, "Error: Too many command line arguments! Please provide an input file and optionally an output file name.\n");
		return 1;

	}


	char* out_filename;

	if(argc == 2){

		out_filename = argv[2];

	}
	else{

		out_filename = "out";
	}

	FILE* output = fopen(out_filename, "w");

	
	
	int len = strlen(argv[1]);

	if(argv[1][len - 1] != '.' && argv[1][len] != 'c'){

		fprintf(stderr, "Error: Input file must end in "".c"".\n");
		return 1;

	}

	FILE* input = fopen(argv[1], "r");

	ast_node_t ast = build_ast(input);
	
	if(validate_ast(ast)){

		generate_llvm_to_file(out_filename, ast);

	}

	return 0;



