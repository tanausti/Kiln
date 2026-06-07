#ifndef PRINT_AST_H
#define PRINT_AST_H

#include <stdio.h>
#include <stdbool.h>
#include "parser.h"


void print_ast(ast_node_t ast);
void indent(int indent_level);

void print_program_node(program_t program, int indent_level);
void print_function_list(function_list_t function_list, int indent_level);
void print_function_node(function_t function, int indent_level);
void print_function_prototype(function_prototype_t function_prototype, int indent_level);
void print_statement_list(statement_list_t statement_list, int indent_level);
void print_statement_node(statement_t statement, int indent_level);
void print_binary_expression_node(binary_expression_t binary_expression, int indent_level);
void print_keyword_node(keyword_t keyword, int indent_level);
void print_primary_node(primary_t primary, int indent_level);
void print_literal(literal_t literal, int indent_level);
void print_func_call(func_call_t func_call, int indent_level);



#endif
