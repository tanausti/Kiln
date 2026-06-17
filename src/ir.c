#include <llvm-c/Core.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "ir.h"





void generate_llvm_to_file(char* filename, ast_node_t ast){

	
	FILE* fp = fopen(filename, "w");


	LLVMContextRef context = LLVMContextCreate();
	LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
	LLVMModuleRef module = LLVMModuleCreateWithNameInContext(filename, context);
	LLVMTypeRef int_type = LLVMInt32TypeInContext(context);

	llvm_context_t llvm_context = {context, builder, module, int_type, NULL};


	program_t program_node = ast.as.program;
	program_node_to_llvm(program_node, &llvm_context);


	char* ir_str = LLVMPrintModuleToString(module);
	fprintf(fp, "%s", ir_str);
	LLVMDisposeMessage(ir_str);


	fclose(fp);

}



void program_node_to_llvm(program_t program_node, llvm_context_t* llvm_context){

	function_list_t function_list = program_node.function_list;
	function_list_to_llvm(function_list, llvm_context);
	

}


void function_list_to_llvm(function_list_t function_list, llvm_context_t* llvm_context){

	
	for(int i = 0; i < function_list.vector_tree.size; i++){

		function_t function_node = function_list.vector_tree.children[i]->as.function;
	
		function_node_to_llvm(function_node, llvm_context);
	
	}

}



void function_node_to_llvm(function_t function_node, llvm_context_t* llvm_context){


	function_prototype_t function_prototype = function_node.function_prototype;

	LLVMTypeRef function_type;

	switch(function_prototype.primitive_type){

		case PRIMITIVE_INT:
			function_type = LLVMFunctionType(llvm_context->int_type, NULL, 0, 0);
			break;
		default:
			function_type = LLVMFunctionType(llvm_context->int_type, NULL, 0, 0);
			break;

	}

	char* function_name = function_prototype.function_name;

	LLVMValueRef function = LLVMAddFunction(llvm_context->module, function_name, function_type);

	append_function_ll_node(function, llvm_context);


	LLVMBasicBlockRef current_entry = LLVMAppendBasicBlockInContext(llvm_context->context, function, "entry");
	LLVMPositionBuilderAtEnd(llvm_context->builder, current_entry);
	


	statement_list_t statement_list = function_node.statement_list;
	statement_list_to_llvm(statement_list, llvm_context, current_entry);


}



//add free ll func
bool append_function_ll_node(LLVMValueRef function, llvm_context_t* llvm_context){

	function_ll_node_t* head = llvm_context->function_ll_head;

	if(head == NULL){

		function_ll_node_t head_value = {function, NULL};
		llvm_context->function_ll_head = malloc(sizeof(head_value));
		*(llvm_context->function_ll_head) = head_value;

	}
	else if(head->next == NULL){

		function_ll_node_t next_value = {function, NULL};
		head->next = malloc(sizeof(next_value));
		*(head->next) = next_value;

	}
	else{
		return false;
	}

	return true;


}


void statement_list_to_llvm(statement_list_t statement_list, llvm_context_t* llvm_context, LLVMBasicBlockRef current_entry){


	for(int i = 0; i < statement_list.vector_tree.size; i++){

		statement_t statement_node = statement_list.vector_tree.children[i]->as.statement;
		statement_node_to_llvm(statement_node, llvm_context, current_entry);

	}


}




//this sucks fix it
void statement_node_to_llvm(statement_t statement_node, llvm_context_t* llvm_context, LLVMBasicBlockRef current_entry){



	bool return_expr = false;
	LLVMValueRef value;

	for(int i = 0; i < statement_node.vector_tree.size; i++){

		ast_node_t ast_node = *statement_node.vector_tree.children[i];
	

		if(ast_node.type == AST_KEYWORD){


			switch(ast_node.as.keyword){

				case KEYW_RETURN:
					return_expr = true;
					break;
				case KEYW_INT:
					assign_expr = true;
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

		
		LLVMPositionBuilderAtEnd(llvm_context->builder, current_entry);
		LLVMBuildRet(llvm_context->builder, value);


	}
	/*
	if(assign_expr){

		LLVMValueRef var = LLVMBuildAlloca(builder, llvm_context->int_type, statement_node.vector_tree.children[1]->left->as.primary.as.variable.name);

		LLVMBuildStore(llvm_context->builder, value, var);

		LLVMValueRef val = LLVMBuildLoad2(llvm_context->builder, llvm_context->int_type, var, statement_node.vector_tree.children[1]->left->as.primary.as.variable.name); 


	}*/	



}





LLVMValueRef primary_node_to_llvm(primary_t primary_node, llvm_context_t* llvm_context){


	LLVMValueRef value;

	if(primary_node.type == PRIMARY_LITERAL){

		value = LLVMConstInt(llvm_context->int_type, primary_node.as.literal.as.integer, 0);	

	}
	else{

		value = function_call_to_llvm(primary_node.as.func_call, llvm_context);

	}



	return value;

}


LLVMValueRef function_call_to_llvm(func_call_t func_call, llvm_context_t* llvm_context){


	char* callee_name = func_call.callee;

	printf("The name is %s\n", callee_name);
	primitive_type_t primitive_type = func_call.primitive_type;
	
	LLVMValueRef callee_ref = function_from_name(callee_name, llvm_context);

	return callee_ref;
	

}


LLVMValueRef function_from_name(char* desired_name, llvm_context_t* llvm_context){

	function_ll_node_t* curr = llvm_context->function_ll_head;


	while(curr->next != NULL){

		char* curr_name = LLVMGetValueName(curr->function);

		if(strcmp(desired_name, curr_name) == 0){

			LLVMValueRef function = curr->function;

			//pass in the type dont search for it
			LLVMValueRef result = LLVMBuildCall2(llvm_context->builder, LLVMGlobalGetValueType(function), function, NULL, 0, "calltmp");

			return result;

		}

		curr = curr->next;

	}

	printf("No function matches desired name: %s\n", desired_name);


}




LLVMValueRef binary_expression_node_to_llvm(binary_expression_t binary_expression_node, llvm_context_t* llvm_context){

	LLVMValueRef value;

	LLVMValueRef left;

	if(binary_expression_node.left->type == AST_PRIMARY){

		primary_t primary_node = binary_expression_node.left->as.primary;

		left = primary_node_to_llvm(primary_node, llvm_context);

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
		value = LLVMBuildAdd(llvm_context->builder, left, right, "sum");
	}

	return value;


}
