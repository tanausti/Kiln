#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "parser.h"
#include "print_ast.h"


void print_ast(FILE* out, ast_node_t ast){



	switch(ast.type){

		case AST_PROGRAM:
			{
				print_program_node(out, ast.as.program, 0);
				break;
			}
		case AST_FUNCTION:
			{
				print_function_node(out, ast.as.function, 2);	
				break;
			}
		case AST_STATEMENT:
			{
				print_statement_node(out, ast.as.statement, 4);
				break;
			}
		case AST_BINARY_EXPRESSION:
			{
				print_binary_expression_node(out, ast.as.binary_expression, 5);
				break;
			}
		case AST_KEYWORD:
			{
				print_keyword_node(out, ast.as.keyword, 5);
				break;
			}
		case AST_PRIMARY:
			{
				print_primary_node(out, ast.as.primary, 6);
				break;
			}
		default:
			{
				fprintf(out, "Invalid token\n");
				break;
			}

	}


}



void indent(FILE* out, int indent_level){


	
	for(int i = 0; i < indent_level; i++){


		fprintf(out, "\t");


	}
	
	

}



void print_program_node(FILE* out, program_t program, int indent_level){

	indent(out, indent_level);
	fprintf(out, "-program:\n");

	print_function_list(out, program.function_list, indent_level + 1);


}

void print_function_list(FILE* out, function_list_t function_list, int indent_level){

	indent(out, indent_level);
	fprintf(out, "+function_list:\n");

	for(int i = 0; i < function_list.vector_tree.size; i++){
		print_ast(out, *function_list.vector_tree.children[i]);
	}


}


void print_function_node(FILE* out, function_t function, int indent_level){

	indent(out, indent_level);
	fprintf(out, "-function:\n");

	function_prototype_t function_prototype = function.function_prototype;
	print_function_prototype(out, function_prototype, indent_level + 1);

	statement_list_t statement_list = function.statement_list;
	print_statement_list(out, statement_list, indent_level + 1);


}


void print_function_prototype(FILE* out, function_prototype_t function_prototype, int indent_level){

	indent(out, indent_level);
	fprintf(out, "+function_prototype:\n");

	char* primitive_type_string = "";

	switch(function_prototype.primitive_type){

		case PRIMITIVE_INT:
			primitive_type_string = "int";
			break;
		default:
			primitive_type_string = "invalid primitive_type";
			break;
	}

	indent(out, indent_level + 1);
	fprintf(out, "+primitive_type: %s\n", primitive_type_string);


	indent(out, indent_level + 1);
	fprintf(out, "+function_name: %s\n", function_prototype.function_name);


}



void print_statement_list(FILE* out, statement_list_t statement_list, int indent_level){
	
	indent(out, indent_level);

	fprintf(out, "+statement_list:\n");

	for(int i = 0; i < statement_list.vector_tree.size; i++){

		print_ast(out, *statement_list.vector_tree.children[i]);

	}


}


void print_statement_node(FILE* out, statement_t statement, int indent_level){

	indent(out, indent_level);

	fprintf(out, "-statement:\n");

	for(int i = 0; i < statement.vector_tree.size; i++){
		print_ast(out, *statement.vector_tree.children[i]);
	}



}



void print_binary_expression_node(FILE* out, binary_expression_t binary_expression, int indent_level){

	indent(out, indent_level);
	fprintf(out, "-binary_expression:\n");


	indent(out, indent_level + 1);
	fprintf(out, "+primitive_type: ");
	switch(binary_expression.primitive_type){

		case PRIMITIVE_INT:
			fprintf(out, "int\n");
			break;
		default:
			fprintf(out, "invalid primitive type enum. Enum value: %d\n", binary_expression.primitive_type);
			break;

	}

	indent(out, indent_level + 1);
	fprintf(out, "+operator: %c\n", binary_expression.operator);

	print_ast(out, *binary_expression.left);
	print_ast(out, *binary_expression.right);


}




void print_keyword_node(FILE* out, keyword_t keyword, int indent_level){

	indent(out, indent_level);
	fprintf(out, "-keyword:\n");


	indent(out, indent_level + 1);
	switch(keyword){

		case KEYW_RETURN:
			fprintf(out, "keyword: return\n");
			break;
		case KEYW_INT:
			fprintf(out, "keyword: int\n");
			break;
		default:
			fprintf(out, "invalid keyword enum. Enum value: %d\n", keyword);
			break;

	}



}




void print_primary_node(FILE* out, primary_t primary, int indent_level){

	indent(out, indent_level);
	fprintf(out, "-primary:\n");


	switch(primary.type){

		case PRIMARY_LITERAL:
			{
				print_literal(out, primary.as.literal, indent_level + 1);
				break;
			}
		case PRIMARY_FUNC_CALL:
			{
				print_func_call(out, primary.as.func_call, indent_level + 1);
				break;
			}

	}




}





void print_literal(FILE* out, literal_t literal, int indent_level){

	indent(out, indent_level);
	fprintf(out, "+literal:\n");



	indent(out, indent_level + 1);
	switch(literal.primitive_type){

		case PRIMITIVE_INT:
			fprintf(out, "+integer: %d\n", literal.as.integer);
			break;
		default:
			fprintf(out, "invalid primitive type enum. Enum value: %d\n", literal.primitive_type);
			break;

	}

}




void print_func_call(FILE* out, func_call_t func_call, int indent_level){

	indent(out, indent_level);
	fprintf(out, "+func_call:\n");


	indent(out, indent_level + 1);
	switch(func_call.primitive_type){

		case PRIMITIVE_INT:
			fprintf(out, "+primtive_type: int\n");
			break;
		default:
			fprintf(out, "invalid primitive type enum. Enum value: %d\n", func_call.primitive_type);
			break;


	}

	indent(out, indent_level + 1);
	fprintf(out, "+callee %s\n:", func_call.callee);

}

