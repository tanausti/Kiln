#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>
#include "lexer.h"
#include "token_stack.h"



typedef struct ast_node ast_node_t;


typedef struct vector_tree{

	ast_node_t** children;

	int size;
	int capacity;

}vector_tree_t;


typedef enum primitive_type{

	PRIMITIVE_INT

}primitive_type_t;


typedef struct literal{


	primitive_type_t primitive_type;

	union{

		int integer;

	}as;

}literal_t;



typedef struct func_call{

	primitive_type_t primitive_type;
	char* callee;

}func_call_t;


typedef enum primary_type{

	PRIMARY_LITERAL,
	PRIMARY_FUNC_CALL,
	PRIMARY_VARIABLE


}primary_type_t;



typedef struct variable{


	primitive_type_t primitive_type;
	char* name;


}variable_t;


typedef struct primary{

	primary_type_t type;

	union{

		literal_t literal;
		func_call_t func_call;
		variable_t variable;

	}as;


}primary_t;


typedef struct function_list{

	vector_tree_t vector_tree;

}function_list_t;


typedef struct function_prototype{

	primitive_type_t primitive_type;
	char* function_name;

}function_prototype_t;


typedef struct statement_list{

	vector_tree_t vector_tree;

}statement_list_t;


typedef struct statement{

	vector_tree_t vector_tree;

}statement_t;


typedef struct function{

	function_prototype_t function_prototype;
	statement_list_t statement_list;

}function_t;


typedef enum keyword{

	KEYW_RETURN,
	KEYW_INT

}keyword_t;


typedef struct binary_expression{

	ast_node_t* left;
	char operator;
	ast_node_t* right;

	primitive_type_t primitive_type;

}binary_expression_t;



typedef enum ast_node_type{


	AST_PROGRAM,
	AST_FUNCTION_LIST,
	AST_FUNCTION,
	AST_STATEMENT,
	AST_BINARY_EXPRESSION,	
	AST_KEYWORD,
	AST_PRIMARY


}ast_node_type_t;

typedef struct program{

	function_list_t function_list;


}program_t;


typedef struct ast_node{


	ast_node_type_t type;

	union{

		program_t program;

		function_t function;

		statement_list_t statement_list;

		statement_t statement;

		keyword_t keyword;

		binary_expression_t binary_expression;

		primary_t primary;

	}as;

}ast_node_t;




ast_node_t build_ast(FILE* cF);
void vec_tree_add_right_child(vector_tree_t* parent_tree, ast_node_t child);

ast_node_t program(token_stack_node_t** curr);
function_list_t function_list(token_stack_node_t** curr);
ast_node_t function(token_stack_node_t** curr);
function_prototype_t function_prototype(token_stack_node_t** curr);
statement_list_t statement_list(token_stack_node_t** curr);
ast_node_t statement(token_stack_node_t** curr);
void parse_keyword(token_stack_node_t** curr, ast_node_t* statement_node);
ast_node_t expression(token_stack_node_t** curr);
primitive_type_t get_primitive_type();
ast_node_t equality(token_stack_node_t** curr);
ast_node_t comparison(token_stack_node_t** curr);
ast_node_t term(token_stack_node_t** curr);
void parse_binary_expression(token_stack_node_t** curr, ast_node_t* left);
ast_node_t factor(token_stack_node_t** curr);
ast_node_t unary(token_stack_node_t** curr);
ast_node_t primary(token_stack_node_t** curr);
void parse_func_call(char* identifier_name, ast_node_t* ast);
void parse_variable(char* identifier_name, ast_node_t* ast);
void parse_literal(char* token_string, ast_node_t* ast);

int str_to_int(char* str);

bool check_token(token_stack_node_t* curr, token_type_t type);
bool match_token(token_stack_node_t** curr, int n, ...);
void consume_token(token_stack_node_t** curr, token_type_t type, char* error_message);

ast_node_t init_program_node();
ast_node_t init_keyword_node();
ast_node_t init_primary_node();
ast_node_t init_function_node();
ast_node_t init_statement_node();
ast_node_t init_binary_expression_node();
vector_tree_t init_vector_tree();


#endif


