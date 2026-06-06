#include "parser.h"
#include "print_ast.h"


void print_ast(ast_node_t ast){

	
	switch(ast.type){

		case AST_PROGRAM:
			{
				print_program_node(ast.as.program, 0);
				break;
			}
		case AST_FUNCTION:
			{
				print_function_node(ast.as.function, 2);	
				break;
			}
		case AST_STATEMENT:
			{
				print_statement_node(ast.as.statement, 4);
				break;
			}
		case AST_BINARY_EXPRESSION:
			{
				print_binary_expression_node(ast.as.binary_expression, 5);
				break;
			}
		case AST_KEYWORD:
			{
				print_keyword_node(ast.as.keyword, 5);
				break;
			}
		case AST_PRIMARY:
			{
				print_primary_node(ast.as.primary, 6);
				break;
			}
		default:
			{
				fprintf(stderr, "Invalid token\n");
				break;
			}

	}	

}


void indent(int indent_level){


	
	for(int i = 0; i < indent_level; i++){


		printf("\t");


	}
	
	

}



void print_program_node(program_t program, int indent_level){

	indent(indent_level);
	printf("-program:\n");

	print_function_list(program.function_list, indent_level + 1);


}

void print_function_list(function_list_t function_list, int indent_level){

	indent(indent_level);
	printf("+function_list:\n");

	for(int i = 0; i < function_list.vector_tree.size; i++){
		print_ast(*function_list.vector_tree.children[i]);
	}


}


void print_function_node(function_t function, int indent_level){

	indent(indent_level);
	printf("-function:\n");

	function_prototype_t function_prototype = function.function_prototype;
	print_function_prototype(function_prototype, indent_level + 1);

	statement_list_t statement_list = function.statement_list;
	print_statement_list(statement_list, indent_level + 1);


}


void print_function_prototype(function_prototype_t function_prototype, int indent_level){

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



void print_statement_list(statement_list_t statement_list, int indent_level){
	
	indent(indent_level);

	printf("+statement_list:\n");

	for(int i = 0; i < statement_list.vector_tree.size; i++){

		print_ast(*statement_list.vector_tree.children[i]);

	}


}


void print_statement_node(statement_t statement, int indent_level){

	indent(indent_level);

	printf("-statement:\n");

	for(int i = 0; i < statement.vector_tree.size; i++){
		print_ast(*statement.vector_tree.children[i]);
	}



}



void print_binary_expression_node(binary_expression_t binary_expression, int indent_level){

	indent(indent_level);
	printf("-binary_expression:\n");


	indent(indent_level + 1);
	printf("+primitive_type: %c\n", binary_expression.primtive_type);

	indent(indent_level + 1);
	printf("+operator: %c\n", binary_expression.operator);

	print_ast(*binary_expression.left);
	print_ast(*binary_expression.right);


}




void print_keyword_node(keyword_t keyword, int indent_level){

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




void print_primary_node(primary_t primary, int indent_level){

	indent(indent_level);
	printf("-primary:\n");


	switch(primary.type){

		case PRIMARY_LITERAL:
			{
				print_literal(primary.as.literal, indent_level + 1);
				break;
			}
		case PRIMARY_FUNC_CALL:
			{
				print_func_call(primary.as.func_call, indent_level + 1);
				break;
			}

	}




}





void print_literal(literal_t literal, int indent_level){

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




void print_func_call(func_call_t func_call, int indent_level){

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





