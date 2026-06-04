#include "parser.h"

void print_ast(ast_node_t ast){

	
	switch(ast.type){

		case AST_FUNCTION_LIST:
			{
				printf("(Function List (temp):\n)");
				for(int i = 0; i < ast.as.function_list.vector_tree.size; i++){
					print_ast(*ast.as.function_list.vector_tree.children[i]);
				}
				break;
			}
		case AST_FUNCTION:
			{
				
				char* name = "bababooey";
				function_prototype_t function_prototype = ast.as.function.function_prototype;
				printf("ast.as.function.function_prototype.function_name: %s\n", function_prototype.function_name);
				printf("ast.as.function.function_prototype.primtive_type %s\n", name);

				for(int i = 0; i < ast.as.function.statement_list.vector_tree.size; i++){
					print_ast(*ast.as.function.statement_list.vector_tree.children[i]);
				}
				break;
			}
		case AST_STATEMENT:
			{

				for(int i = 0; i < ast.as.statement.vector_tree.size; i++){
					print_ast(*ast.as.statement.vector_tree.children[i]);
				}
				break;
			}
		case AST_BINARY_EXPRESSION:
			{

				print_ast(*ast.as.binary_expression.left);
				printf("ast.as.binary_expression.operator: %c\n", ast.as.binary_expression.operator);
				print_ast(*(ast.as.binary_expression.right));
				break;

			}
		case AST_KEYWORD:
			{
				break;
			}
		case AST_PRIMARY:
			{
				//assume only literals for now
				printf("ast.as.primary.as.literal.as.integer: %d\n", ast.as.primary.as.literal.as.integer);			
				break;
			}

	}	

}

