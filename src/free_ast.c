#include "parser.h"
#include "free_ast.h"


void free_ast(ast_node_t ast){

	ast_node_t* ast_ptr = malloc(sizeof(ast_node_t));
	*ast_ptr = ast;

	printf("Type: %d\n", ast_ptr->type);

	free_ast_child(ast_ptr);

}

void free_ast_child(ast_node_t* ast){
	
	switch(ast->type){

		case AST_PROGRAM:
			{
				free_program(ast->as.program);
				break;
			}
		case AST_FUNCTION:
			{
				free_function(ast->as.function);	
				break;
			}

		case AST_IF_STATEMENT:
			{
				free_if_statement(ast->as.if_statement);	
				break;
			}
		case AST_STATEMENT:
			{
				free_statement(ast->as.statement);
				break;
			}
		case AST_BINARY_EXPRESSION:
			{
				free_binary_expression(ast->as.binary_expression);
				break;
			}
		case AST_KEYWORD:
			{
				break;
			}
		case AST_PRIMARY:
			{
				free_primary(ast->as.primary);
				break;
			}
		case AST_ERROR:
			{
				break;
			}
		default:
			{
				break;
			}

	}

	free(ast);

}


void free_program(program_t program){

	free_function_list(program.function_list);

}


void free_function_list(function_list_t function_list){

	for(int i = 0; i < function_list.vector_tree.size; i++){
		free_ast_child(function_list.vector_tree.children[i]);
	}

	free(function_list.vector_tree.children);

}


void free_function(function_t function){

	function_prototype_t function_prototype = function.function_prototype;
	free_function_prototype(function_prototype);

	statement_list_t statement_list = function.statement_list;
	free_statement_list(statement_list);

}


void free_function_prototype(function_prototype_t function_prototype){

	free(function_prototype.function_name);

}


void free_statement_list(statement_list_t statement_list){
	
	for(int i = 0; i < statement_list.vector_tree.size; i++){

		free_ast_child(statement_list.vector_tree.children[i]);

	}

	free(statement_list.vector_tree.children);

}


void free_if_statement(if_statement_t if_statement){

	free_ast_child(if_statement.condition_node);

	free_statement_list(if_statement.statement_list);

}


void free_statement(statement_t statement){
	
	for(int i = 0; i < statement.vector_tree.size; i++){

		free_ast_child(statement.vector_tree.children[i]);

	}

	free(statement.vector_tree.children);

}


void free_binary_expression(binary_expression_t binary_expression){

	free_ast_child(binary_expression.left);
	free_ast_child(binary_expression.right);

}


void free_primary(primary_t primary){

	switch(primary.type){

		case PRIMARY_FUNC_CALL:
			free_func_call(primary.as.func_call);
			break;
		case PRIMARY_VARIABLE:
			free_variable(primary.as.variable);
			break;
		case PRIMARY_LITERAL:
			break;
		default:
			break;

	}

}


void free_func_call(func_call_t func_call){

	free(func_call.callee);

}


void free_variable(variable_t variable){

	free(variable.name);

}
