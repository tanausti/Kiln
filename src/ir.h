#ifndef IR_H
#define IR_H

#include <stdio.h>

#include <llvm-c/Core.h>
#include "parser.h"



typedef struct llvm_data{

	LLVMContextRef context;
	LLVMBuilderRef builder;
	LLVMModuleRef module;
	LLVMTypeRef int_type;

}llvm_data_t;



void generate_llvm_to_file(FILE* out, ast_node_t ast);

void program_node_to_llvm(program_t program_node, llvm_data_t llvm_data);
void function_node_to_llvm(function_t function_node, llvm_data_t llvm_data);
void statement_node_to_llvm(statement_t statement_node, llvm_data_t llvm_data);
void return_statement_to_llvm(statement_t statement_node, llvm_data_t llvm_data);
LLVMValueRef binary_expression_node_to_llvm(binary_expression_t binary_expression_node, llvm_data_t llvm_data);
LLVMValueRef primary_node_to_llvm(primary_t primary_node, llvm_data_t llvm_data);



#endif
