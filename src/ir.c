#include <llvm-c/Core.h>
#include <stdio.h>
#include "parser.h"
#include "ir.h"




void generate_llvm_to_file(FILE* out, ast_node_t ast){


	LLVMContextRef context = LLVMContextCreate();
	LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
	LLVMModuleRef module = LLVMModuleCreateWithNameInContext("test", context);
	LLVMTypeRef int_type = LLVMInt32TypeInContext(context);

	llvm_data_t llvm_data = {context, builder, module, int_type};
	program_t program_node = ast.as.program;

	program_node_to_llvm(program_node, llvm_data);

	char* ir_str = LLVMPrintModuleToString(module);
	fprintf(out, "%s", ir_str);
	LLVMDisposeMessage(ir_str);


}



void program_node_to_llvm(program_t program_node, llvm_data_t llvm_data){

	function_list_t function_list = program_node.function_list;	

	for(int i = 0; i < function_list.vector_tree.size; i++){

		function_t function_node = function_list.vector_tree.children[i]->as.function;
	
		function_node_to_llvm(function_node, llvm_data);
	

	}

}



void function_node_to_llvm(function_t function_node, llvm_data_t llvm_data){


	function_prototype_t function_prototype = function_node.function_prototype;

	LLVMTypeRef function_type;

	switch(function_prototype.primitive_type){

		case PRIMITIVE_INT:
			function_type = LLVMFunctionType(llvm_data.int_type, NULL, 0, 0);
			break;
		default:
			function_type = LLVMFunctionType(llvm_data.int_type, NULL, 0, 0);
			break;

	}

	char* function_name = function_prototype.function_name;

	LLVMValueRef function = LLVMAddFunction(llvm_data.module, function_name, function_type);
	LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(llvm_data.context, function, "entry");
	LLVMPositionBuilderAtEnd(llvm_data.builder, entry);
	


	statement_list_t statement_list = function_node.statement_list;

	for(int i = 0; i < statement_list.vector_tree.size; i++){

		statement_t statement_node = statement_list.vector_tree.children[i]->as.statement;
		statement_node_to_llvm(statement_node, llvm_data);

	}



}





void statement_node_to_llvm(statement_t statement_node, llvm_data_t llvm_data){


	ast_node_t first_child = *statement_node.vector_tree.children[0];

	if(first_child.type == AST_KEYWORD){

		switch(first_child.as.keyword){

			case KEYW_RETURN:
				return_statement_to_llvm(statement_node, llvm_data);
				break;

		}

	}





}



void return_statement_to_llvm(statement_t statement_node, llvm_data_t llvm_data){


	LLVMValueRef value;

	for(int i = 1; i < statement_node.vector_tree.size; i++){

		ast_node_t ast_node = *statement_node.vector_tree.children[i];
		
		if(ast_node.type == AST_BINARY_EXPRESSION){

			value = binary_expression_node_to_llvm(ast_node.as.binary_expression, llvm_data);

		}


	}	

	LLVMBuildRet(llvm_data.builder, value);


}




LLVMValueRef binary_expression_node_to_llvm(binary_expression_t binary_expression_node, llvm_data_t llvm_data){

	LLVMValueRef value;

	LLVMValueRef left;

	if(binary_expression_node.left->type == AST_PRIMARY){

		primary_t primary_node = binary_expression_node.left->as.primary;

		left = primary_node_to_llvm(primary_node, llvm_data);

	}


	LLVMValueRef right;

	if(binary_expression_node.right->type == AST_PRIMARY){

		primary_t primary_node = binary_expression_node.right->as.primary;

		right = primary_node_to_llvm(primary_node, llvm_data);

	}
	else{

		//another b expression

	}


	if(binary_expression_node.operator == '+'){
		value = LLVMBuildAdd(llvm_data.builder, left, right, "sum");
	}

	return value;



}



LLVMValueRef primary_node_to_llvm(primary_t primary_node, llvm_data_t llvm_data){


	LLVMValueRef value;

	if(primary_node.type == PRIMARY_LITERAL){
		value = LLVMConstInt(llvm_data.int_type, primary_node.as.literal.as.integer, 0);	
	}
	else{

		//func_call

	}


	return value;

}
