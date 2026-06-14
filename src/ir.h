#ifndef IR_H
#define IR_H

#include <stdio.h>

#include <llvm-c/Core.h>
#include "parser.h"



typedef struct llvm_context{

	LLVMContextRef context;
	LLVMBuilderRef builder;
	LLVMModuleRef module;
	LLVMTypeRef int_type;

}llvm_context_t;



void generate_llvm_to_file(char* filename, ast_node_t ast);

void program_node_to_llvm(program_t program_node, llvm_context_t llvm_data);
void function_list_to_llvm(function_list_t function_list, llvm_context_t llvm_context);
void function_node_to_llvm(function_t function_node, llvm_context_t llvm_data);
void statement_list_to_llvm(statement_list_t statement_list, llvm_context_t llvm_context, LLVMBasicBlockRef current_entry);
void statement_node_to_llvm(statement_t statement_node, llvm_context_t llvm_data, LLVMBasicBlockRef current_entry);
LLVMValueRef binary_expression_node_to_llvm(binary_expression_t binary_expression_node, llvm_context_t llvm_data);
LLVMValueRef primary_node_to_llvm(primary_t primary_node, llvm_context_t llvm_data);


#endif
