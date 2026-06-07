#ifndef PRINT_AST_H
#define PRINT_AST_H

#include "parser.h"


void ast_to_str(ast_node_t ast);

void indent(int indent_level);

void program_node_to_str(program_t program, int indent_level);
void function_list_to_str(function_list_t function_list, int indent_level);
void function_node_to_str(function_t function, int indent_level);
void function_prototype_to_str(function_prototype_t function_prototype, int indent_level);
void statement_list_to_str(statement_list_t statement_list, int indent_level);
void statement_node_to_str(statement_t statement, int indent_level);
void binary_expression_node_to_str(binary_expression_t binary_expression, int indent_level);
void keyword_node_to_str(keyword_t keyword, int indent_level);
void primary_node_to_str(primary_t primary, int indent_level);
void literal_to_str(literal_t literal, int indent_level);
void func_call_to_str(func_call_t func_call, int indent_level);



#endif
