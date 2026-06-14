#include <llvm-c/Core.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "ir.h"



//each function prototype will map to a LLVMValueRef (function)


void generate_llvm_to_file(char* filename, ast_node_t ast){

	
	FILE* fp = fopen(filename, "w");


	LLVMContextRef context = LLVMContextCreate();
	LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
	LLVMModuleRef module = LLVMModuleCreateWithNameInContext(filename, context);
	LLVMTypeRef int_type = LLVMInt32TypeInContext(context);

	llvm_context_t llvm_context = {context, builder, module, int_type};


	program_t program_node = ast.as.program;
	program_node_to_llvm(program_node, llvm_context);


	char* ir_str = LLVMPrintModuleToString(module);
	fprintf(fp, "%s", ir_str);
	LLVMDisposeMessage(ir_str);


	fclose(fp);

}



void program_node_to_llvm(program_t program_node, llvm_context_t llvm_context){

	function_list_t function_list = program_node.function_list;
	function_list_to_llvm(function_list, llvm_context);
	

}


void function_list_to_llvm(function_list_t function_list, llvm_context_t llvm_context){

	
	for(int i = 0; i < function_list.vector_tree.size; i++){

		function_t function_node = function_list.vector_tree.children[i]->as.function;
	
		function_node_to_llvm(function_node, llvm_context);
	
	}

}



void function_node_to_llvm(function_t function_node, llvm_context_t llvm_context){


	function_prototype_t function_prototype = function_node.function_prototype;

	LLVMTypeRef function_type;

	switch(function_prototype.primitive_type){

		case PRIMITIVE_INT:
			function_type = LLVMFunctionType(llvm_context.int_type, NULL, 0, 0);
			break;
		default:
			function_type = LLVMFunctionType(llvm_context.int_type, NULL, 0, 0);
			break;

	}

	char* function_name = function_prototype.function_name;

	LLVMValueRef function = LLVMAddFunction(llvm_context.module, function_name, function_type);


	LLVMBasicBlockRef current_entry = LLVMAppendBasicBlockInContext(llvm_context.context, function, "entry");
	LLVMPositionBuilderAtEnd(llvm_context.builder, current_entry);
	


	statement_list_t statement_list = function_node.statement_list;
	statement_list_to_llvm(statement_list, llvm_context, current_entry);


}


void statement_list_to_llvm(statement_list_t statement_list, llvm_context_t llvm_context, LLVMBasicBlockRef current_entry){


	for(int i = 0; i < statement_list.vector_tree.size; i++){

		statement_t statement_node = statement_list.vector_tree.children[i]->as.statement;
		statement_node_to_llvm(statement_node, llvm_context, current_entry);

	}


}





void statement_node_to_llvm(statement_t statement_node, llvm_context_t llvm_context, LLVMBasicBlockRef current_entry){



	bool return_expr = false;
	LLVMValueRef value;

	for(int i = 0; i < statement_node.vector_tree.size; i++){

		ast_node_t ast_node = *statement_node.vector_tree.children[i];
	

		if(ast_node.type == AST_KEYWORD){


			switch(ast_node.as.keyword){

				case KEYW_RETURN:
					return_expr = true;
					break;

			}



		}
		else if(ast_node.type == AST_BINARY_EXPRESSION){

			value = binary_expression_node_to_llvm(ast_node.as.binary_expression, llvm_context);

		}
		else if(ast_node.type == AST_PRIMARY){

			value = primary_node_to_llvm(ast_node.as.primary, llvm_context);


		}


	}

	if(return_expr){

		
		LLVMPositionBuilderAtEnd(llvm_context.builder, current_entry);
		LLVMBuildRet(llvm_context.builder, value);


	}	



}





LLVMValueRef primary_node_to_llvm(primary_t primary_node, llvm_context_t llvm_context){


	LLVMValueRef value;

	if(primary_node.type == PRIMARY_LITERAL){

		value = LLVMConstInt(LLVMInt32TypeInContext(llvm_context.context), primary_node.as.literal.as.integer, 0);	

	}
	else{

		//func_call

	}



	return value;

}




LLVMValueRef binary_expression_node_to_llvm(binary_expression_t binary_expression_node, llvm_context_t llvm_context){

	LLVMValueRef value;

	LLVMValueRef left;

	if(binary_expression_node.left->type == AST_PRIMARY){

		primary_t primary_node = binary_expression_node.left->as.primary;

		left = primary_node_to_llvm(primary_node, llvm_context);

		printf("left is: %d\n", primary_node.as.literal.as.integer);

	}




	LLVMValueRef right;

	if(binary_expression_node.right->type == AST_PRIMARY){

		primary_t primary_node = binary_expression_node.right->as.primary;

		right = primary_node_to_llvm(primary_node, llvm_context);

	}
	else{

		right = binary_expression_node_to_llvm(binary_expression_node.right->as.binary_expression, llvm_context);

	}


	if(binary_expression_node.operator == '+'){
		value = LLVMBuildAdd(llvm_context.builder, left, right, "sum");
	}

	return value;


}
