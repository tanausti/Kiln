#ifndef VALIDATE_AST_H
#define VALIDATE_AST_H

#include <stdbool.h>
#include "ast.h"

bool validate_ast(ast_node_t ast);
void validate_ast_child(ast_node_t* ast, bool* valid);
void validate_program(program_t program, bool* valid);
void validate_function_list(function_list_t function_list, bool* valid);
void validate_function(function_t function, bool* valid);
void validate_statement_list(statement_list_t statement_list, bool* valid);
void validate_if_statement(if_statement_t if_statement, bool* valid);
void validate_statement(statement_t statement, bool* valid);
void validate_binary_expression(binary_expression_t binary_expression, bool* valid);

#endif
