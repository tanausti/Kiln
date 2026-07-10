#include <stdbool.h>
#include <stdlib.h>
#include "ast.h"
#include "validate_ast.h"


bool validate_ast(ast_node_t ast){

	ast_node_t* ast_ptr = malloc(sizeof(ast_node_t));
	*ast_ptr = ast;

	bool valid = true;

	validate_ast_child(ast_ptr, &valid);

	return valid;

}


void validate_ast_child(ast_node_t* ast, bool* valid){
	
	switch(ast->type){

		case AST_PROGRAM:
			{
				validate_program(ast->as.program, valid);
				break;
			}
		case AST_FUNCTION:
			{
				validate_function(ast->as.function, valid);	
				break;
			}

		case AST_IF_STATEMENT:
			{
				validate_if_statement(ast->as.if_statement, valid);	
				break;
			}
		case AST_STATEMENT:
			{
				validate_statement(ast->as.statement, valid);
				break;
			}
		case AST_BINARY_EXPRESSION:
			{
				validate_binary_expression(ast->as.binary_expression, valid);
				break;
			}
		case AST_KEYWORD:
			{
				break;
			}
		case AST_PRIMARY:
			{
				break;
			}
		case AST_ERROR:
			{
				*valid = false;
				break;
			}
		default:
			{
				*valid = false;
				break;
			}

	}

}


void validate_program(program_t program, bool* valid){

	validate_function_list(program.function_list, valid);

}


void validate_function_list(function_list_t function_list, bool* valid){

	for(int i = 0; i < function_list.vector_tree.size; i++){
		validate_ast_child(function_list.vector_tree.children[i], valid);
	}

}


void validate_function(function_t function, bool* valid){

	validate_statement_list(function.statement_list, valid);

}


void validate_statement_list(statement_list_t statement_list, bool* valid){
	
	for(int i = 0; i < statement_list.vector_tree.size; i++){

		validate_ast_child(statement_list.vector_tree.children[i], valid);

	}


}


void validate_if_statement(if_statement_t if_statement, bool* valid){

	validate_ast_child(if_statement.condition_node, valid);

	validate_statement_list(if_statement.statement_list, valid);

}


void validate_statement(statement_t statement, bool* valid){
	
	for(int i = 0; i < statement.vector_tree.size; i++){

		validate_ast_child(statement.vector_tree.children[i], valid);

	}

}


void validate_binary_expression(binary_expression_t binary_expression, bool* valid){

	validate_ast_child(binary_expression.left, valid);
	validate_ast_child(binary_expression.right, valid);

}


