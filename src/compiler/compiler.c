#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../ir/ir.h"
#include "../parser/parser.h"
#include "../parser/ast.h"
#include "../parser/validate_ast.h"
#include "../parser/free_ast.h"

int main(int argc, char* argv[]){

	if(argc == 0){

		fprintf(stderr, "Error: Please provide the input file that you would like to compile!\n");
		return 1;

	}
	else if(argc > 3){

		fprintf(stderr, "Error: Too many command line arguments! Please provide an input file and optionally an output file name.\n");
		return 1;

	}


	char* output_filename;

	if(argc == 3){

		output_filename = argv[2];
		strcat(output_filename, ".ll");

	}
	else{

		output_filename = "out.ll";
	}


	
	
	int len = strlen(argv[1]);

	
	if(argv[1][len - 2] != '.' || argv[1][len - 1] != 'c'){

		fprintf(stderr, "Error: Input file must end in "".c"".\n");
		return 1;

	}
	

	FILE* input = fopen(argv[1], "r");

	ast_node_t ast = build_ast(input);
	
	if(validate_ast(ast)){

		generate_llvm_to_file(output_filename, ast);

	}

	fclose(input);
	free_ast(ast);
	return 0;


}
