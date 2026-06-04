#include "parser.h"
#include "print_ast.h"


void print_ast(ast_node_t ast){

	
	switch(ast.type){

		case AST_PROGRAM:
			{
				print_program_node(ast.as.program);
				break;
			}
		case AST_FUNCTION:
			{			
				print_function_node(ast.as.function);	
				break;
			}
		case AST_STATEMENT:
			{
				print_statement_node(ast.as.statement);
				break;
			}
		case AST_BINARY_EXPRESSION:
			{

				print_binary_expression_node(ast.as.binary_expression);
				break;

			}
		case AST_KEYWORD:
			{
				print_keyword_node(ast.as.keyword);
				break;
			}
		case AST_PRIMARY:
			{

				print_primary_node(ast.as.primary);
				break;
			}

	}	

}


void print_program_node(program_t program){

	print_function_list(program.function_list);


}

void print_function_list(function_list_t function_list){


	for(int i = 0; i < function_list.vector_tree.size; i++){
		print_ast(*function_list.vector_tree.children[i]);
	}


}


void print_function_node(function_t function){


	function_prototype_t function_prototype = function.function_prototype;
	print_function_prototype(function_prototype);

	statement_list_t statement_list = function.statement_list;
	print_statement_list(statement_list);


}


void print_function_prototype(function_prototype_t function_prototype){


	char* primitive_type_string = "invalid primitive_type\n";

	switch(function_prototype.primitive_type){

		case PRIMITIVE_INT:
			primitive_type_string = "int";
			break;

	}

	printf("function.function_prototype.function_name: %s\n", function_prototype.function_name);
	printf("function.function_prototype.primtive_type %s\n", primitive_type_string);



}



void print_statement_list(statement_list_t statement_list){
	

	for(int i = 0; i < statement_list.vector_tree.size; i++){

		print_ast(*statement_list.vector_tree.children[i]);

	}


}


void print_statement_node(statement_t statement){


	for(int i = 0; i < statement.vector_tree.size; i++){
		print_ast(*statement.vector_tree.children[i]);
	}



}



void print_binary_expression_node(binary_expression_t binary_expression){


	print_ast(*binary_expression.left);
	printf("binary_expression.operator: %c\n", binary_expression.operator);
	print_ast(*binary_expression.right);



}




void print_keyword_node(keyword_t keyword){



	switch(keyword){

		case KEYW_RETURN:
			printf("keyword: KEYW_RETURN");
			break;
		case KEYW_INT:
			printf("keyword: KEYW_INT");
			break;



	}



}




void print_primary_node(primary_t primary){



	switch(primary.type){

		case PRIMARY_LITERAL:
			print_literal(primary.as.literal);
			break;
		case PRIMARY_FUNC_CALL:
			print_func_call(primary.as.func_call);
			break;


	}




}





void print_literal(literal_t literal){

	switch(literal.primitive_type){

		case PRIMITIVE_INT:
			printf("literal.as.integer: %d\n", literal.as.integer);
			break;


	}

}




void print_func_call(func_call_t func_call){

	switch(func_call.primitive_type){

		case PRIMITIVE_INT:
			printf("func_call.primtive_type: PRIMTIVE_INT\n");
			break;


	}

	printf("callee %s\n:", func_call.callee);

}





