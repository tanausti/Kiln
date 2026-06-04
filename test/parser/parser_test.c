#include "../../src/parser.h"
#include "../../src/print_ast.h"
#include <stdlib.h>
#include <stdbool.h>

int main(){

	
	FILE* input1 = fopen("input1.c", "r");
	

	ast_node_t ast = build_ast(input1);

	print_ast(ast);



	fclose(input1);

}




bool test_ast(){



	bool test_pass = true;


	return test_pass;








}
