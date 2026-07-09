#ifndef FREE_AST_H
#define FREE_AST_H


void free_ast(ast_node_t ast);
void free_ast_child(ast_node_t* ast);
void free_program(program_t program);
void free_function_list(function_list_t function_list);
void free_function(function_t function);
void free_function_prototype(function_prototype_t function_prototype);
void free_statement_list(statement_list_t statement_list);
void free_if_statement(if_statement_t if_statement);
void free_statement(statement_t statement);
void free_binary_expression(binary_expression_t binary_expression);
void free_primary(primary_t primary);
void free_func_call(func_call_t func_call);
void free_variable(variable_t variable);


#endif
