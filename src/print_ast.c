#include <string.h>
#include "parser.h"
#include "print_ast.h"

//TODO: print to buffer instead of stdout. Add print primtive funciton
void ast_to_str(ast_node_t ast){

	int max_str_length = 32767;
	char buffer[max_str_length];

	strcat(buffer, "hello");
	
	switch(ast.type){

		case AST_PROGRAM:
			{
				program_node_to_str(ast.as.program, 0);
				break;
			}
		case AST_FUNCTION:
			{
				function_node_to_str(ast.as.function, 2);	
				break;
			}
		case AST_STATEMENT:
			{
				statement_node_to_str(ast.as.statement, 4);
				break;
			}
		case AST_BINARY_EXPRESSION:
			{
				binary_expression_node_to_str(ast.as.binary_expression, 5);
				break;
			}
		case AST_KEYWORD:
			{
				keyword_node_to_str(ast.as.keyword, 5);
				break;
			}
		case AST_PRIMARY:
			{
				primary_node_to_str(ast.as.primary, 6);
				break;
			}
		default:
			{
				strcpy("Invalid token", buffer);
				break;
			}

	}

	

}


void indent(int indent_level){


	
	for(int i = 0; i < indent_level; i++){


		printf("\t");


	}
	
	

}



void program_node_to_str(program_t program, int indent_level){

	indent(indent_level);
	printf("-program:\n");

	function_list_to_str(program.function_list, indent_level + 1);


}

void function_list_to_str(function_list_t function_list, int indent_level){

	indent(indent_level);
	printf("+function_list:\n");

	for(int i = 0; i < function_list.vector_tree.size; i++){
		ast_to_str(*function_list.vector_tree.children[i]);
	}


}


void function_node_to_str(function_t function, int indent_level){

	indent(indent_level);
	printf("-function:\n");

	function_prototype_t function_prototype = function.function_prototype;
	function_prototype_to_str(function_prototype, indent_level + 1);

	statement_list_t statement_list = function.statement_list;
	statement_list_to_str(statement_list, indent_level + 1);


}


void function_prototype_to_str(function_prototype_t function_prototype, int indent_level){

	indent(indent_level);
	printf("+function_prototype:\n");

	char* primitive_type_string = "";

	switch(function_prototype.primitive_type){

		case PRIMITIVE_INT:
			primitive_type_string = "int";
			break;
		default:
			primitive_type_string = "invalid primitive_type";
			break;
	}

	indent(indent_level + 1);
	printf("+primitive_type: %s\n", primitive_type_string);


	indent(indent_level + 1);
	printf("+function_name: %s\n", function_prototype.function_name);


}



void statement_list_to_str(statement_list_t statement_list, int indent_level){
	
	indent(indent_level);

	printf("+statement_list:\n");

	for(int i = 0; i < statement_list.vector_tree.size; i++){

		ast_to_str(*statement_list.vector_tree.children[i]);

	}


}


void statement_node_to_str(statement_t statement, int indent_level){

	indent(indent_level);

	printf("-statement:\n");

	for(int i = 0; i < statement.vector_tree.size; i++){
		ast_to_str(*statement.vector_tree.children[i]);
	}



}



void binary_expression_node_to_str(binary_expression_t binary_expression, int indent_level){

	indent(indent_level);
	printf("-binary_expression:\n");


	indent(indent_level + 1);
	printf("+primitive_type: ");
	switch(binary_expression.primitive_type){

		case PRIMITIVE_INT:
			printf("int\n");
			break;
		default:
			printf("invalid primitive type enum. Enum value: %d\n", binary_expression.primitive_type);
			break;

	}

	indent(indent_level + 1);
	printf("+operator: %c\n", binary_expression.operator);

	ast_to_str(*binary_expression.left);
	ast_to_str(*binary_expression.right);


}




void keyword_node_to_str(keyword_t keyword, int indent_level){

	indent(indent_level);
	printf("-keyword:\n");


	indent(indent_level + 1);
	switch(keyword){

		case KEYW_RETURN:
			printf("keyword: return\n");
			break;
		case KEYW_INT:
			printf("keyword: int\n");
			break;
		default:
			printf("invalid keyword enum. Enum value: %d\n", keyword);
			break;

	}



}




void primary_node_to_str(primary_t primary, int indent_level){

	indent(indent_level);
	printf("-primary:\n");


	switch(primary.type){

		case PRIMARY_LITERAL:
			{
				literal_to_str(primary.as.literal, indent_level + 1);
				break;
			}
		case PRIMARY_FUNC_CALL:
			{
				func_call_to_str(primary.as.func_call, indent_level + 1);
				break;
			}

	}




}





void literal_to_str(literal_t literal, int indent_level){

	indent(indent_level);
	printf("+literal:\n");



	indent(indent_level + 1);
	switch(literal.primitive_type){

		case PRIMITIVE_INT:
			printf("+integer: %d\n", literal.as.integer);
			break;
		default:
			printf("invalid primitive type enum. Enum value: %d\n", literal.primitive_type);
			break;

	}

}




void func_call_to_str(func_call_t func_call, int indent_level){

	indent(indent_level);
	printf("+func_call:\n");


	indent(indent_level + 1);
	switch(func_call.primitive_type){

		case PRIMITIVE_INT:
			printf("+primtive_type: int\n");
			break;
		default:
			printf("invalid primitive type enum. Enum value: %d\n", func_call.primitive_type);
			break;


	}

	indent(indent_level + 1);
	printf("+callee %s\n:", func_call.callee);

}





