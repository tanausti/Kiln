#ifndef IR_H
#define IR_H

#include <stdio.h>

#include <llvm-c/Core.h>
#include <stdbool.h>
#include "parser.h"



typedef struct llvm_context{

	LLVMContextRef context;
	LLVMBuilderRef builder;
	LLVMModuleRef module;
	LLVMTypeRef int_type;

	struct function_ll_node* function_ll_head;
	struct var_ptr_ll_node* var_ptr_ll_head;

}llvm_context_t;


//function name used as key is stored in value ref
typedef struct function_ll_node{

	LLVMValueRef function;
	struct function_ll_node* next; 

}function_ll_node_t;

typedef struct var_ptr_ll_node{

	char* name;
	LLVMValueRef var_ptr;
	struct var_ptr_ll_node* next;

}var_ptr_ll_node_t;



void generate_llvm_to_file(char* filename, ast_node_t ast);

void program_node_to_llvm(program_t program_node, llvm_context_t* llvm_data);
void function_list_to_llvm(function_list_t function_list, llvm_context_t* llvm_context);
void function_node_to_llvm(function_t function_node, llvm_context_t* llvm_data);
bool append_function_ll_node(LLVMValueRef function, llvm_context_t* llvm_context);
void statement_list_to_llvm(statement_list_t statement_list, llvm_context_t* llvm_context, LLVMBasicBlockRef current_entry);
void statement_node_to_llvm(statement_t statement_node, llvm_context_t* llvm_data, LLVMBasicBlockRef current_entry);
void keyword_node_to_llvm(keyword_t keyword_node, ast_node_t second_child_node, llvm_context_t* llvm_context, LLVMBasicBlockRef current_entry);
LLVMValueRef variable_declaration_to_llvm(ast_node_t second_child_node, llvm_context_t* llvm_context);
bool append_var_ptr_ll_node(LLVMValueRef var_ptr, char* var_name, llvm_context_t* llvm_context);
LLVMValueRef binary_expression_node_to_llvm(binary_expression_t binary_expression_node, llvm_context_t* llvm_data);
LLVMValueRef primary_node_to_llvm(primary_t primary_node, llvm_context_t* llvm_data);
LLVMValueRef function_call_to_llvm(func_call_t func_call, llvm_context_t* llvm_context);
LLVMValueRef function_from_name(char* desired_name, llvm_context_t* llvm_context);





#endif
