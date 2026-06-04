#ifndef PRINT_AST_H
#define PRINT_AST_H

#include "parser.h"


void print_ast(ast_node_t ast);

void print_function_node(function_t function);
void print_function_prototype(function_prototype_t function_prototype);
void print_statement_list(statement_list_t statement_list);
void print_statement_node(statement_t statement);
void print_binary_expression_node(binary_expression_t binary_expression);
void print_keyword_node(keyword_t keyword);
void print_primary_node(primary_t primary);
void print_literal(literal_t literal);
void print_func_call(func_call_t func_call);



#endif
