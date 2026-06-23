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

	llvm_context_t llvm_context = {context, builder, module, int_type, NULL, NULL};


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



void statement_node_to_llvm(statement_t statement_node, llvm_context_t* llvm_context, LLVMBasicBlockRef current_entry){


	ast_node_t first_child_node = *statement_node.vector_tree.children[0];
	ast_node_t second_child_node = *statement_node.vector_tree.children[1];

	if(first_child_node.type == AST_KEYWORD){

		keyword_node_to_llvm(first_child_node.as.keyword, second_child_node, llvm_context, current_entry);

	}
	else if(first_child_node.type == AST_BINARY_EXPRESSION){

		LLVMValueRef binary_expression = binary_expression_node_to_llvm(first_child_node.as.binary_expression, llvm_context);

	}
	else if(first_child_node.type == AST_PRIMARY){

		LLVMValueRef primary = primary_node_to_llvm(first_child_node.as.primary, llvm_context);

	}	

	
	



}

//the second node is passed in as just an ast node as it could be a primary, variable or binary expression
void keyword_node_to_llvm(keyword_t keyword_node, ast_node_t second_child_node, llvm_context_t* llvm_context, LLVMBasicBlockRef current_entry){


	if(keyword_node == KEYW_INT){

		variable_declaration_to_llvm(second_child_node, llvm_context);

		if(second_child_node.type == AST_BINARY_EXPRESSION){
			
			LLVMValueRef binary_expression = binary_expression_node_to_llvm(second_child_node.as.binary_expression, llvm_context);
		}

	}
	if(keyword_node == KEYW_RETURN){

		LLVMValueRef return_val;
		if(second_child_node.type == AST_BINARY_EXPRESSION){
			return_val = binary_expression_node_to_llvm(second_child_node.as.binary_expression, llvm_context);
		}
		else{
			return_val = primary_node_to_llvm(second_child_node.as.primary, llvm_context);
		}

		LLVMBuildRet(llvm_context->builder, return_val);

	}


}



LLVMValueRef variable_declaration_to_llvm(ast_node_t second_child_node, llvm_context_t* llvm_context){

	variable_t variable;

	if(second_child_node.type == AST_PRIMARY){

		primary_t primary_node = second_child_node.as.primary;
		variable = primary_node.as.variable;

	}
	else if(second_child_node.type == AST_BINARY_EXPRESSION){

		binary_expression_t binary_expression_node = second_child_node.as.binary_expression;
		primary_t primary_node = binary_expression_node.left->as.primary;
		variable = primary_node.as.variable;

	}

	primitive_type_t variable_type = variable.primitive_type;
	char* var_name = variable.name;

	LLVMValueRef var_ptr = NULL;
	if(variable.primitive_type == PRIMITIVE_INT){
		var_ptr = LLVMBuildAlloca(llvm_context->builder, llvm_context->int_type, var_name);
	}

	append_var_ptr_ll_node(var_ptr, var_name, llvm_context);

	return var_ptr;


}



//add free ll func
bool append_var_ptr_ll_node(LLVMValueRef var_ptr, char* var_name, llvm_context_t* llvm_context){

	var_ptr_ll_node_t* head = llvm_context->var_ptr_ll_head;

	if(head == NULL){

		var_ptr_ll_node_t head_value = {var_name, var_ptr, NULL};
		llvm_context->var_ptr_ll_head = malloc(sizeof(head_value));
		*(llvm_context->var_ptr_ll_head) = head_value;

	}
	else if(head->next == NULL){

		var_ptr_ll_node_t next_value = {var_name, var_ptr, NULL};
		head->next = malloc(sizeof(next_value));
		*(head->next) = next_value;

	}
	else{
		return false;
	}

	return true;


}





LLVMValueRef var_ptr_from_name(char* desired_name, llvm_context_t* llvm_context){

	var_ptr_ll_node_t* curr = llvm_context->var_ptr_ll_head;


	while(curr != NULL){

		char* curr_name = curr->name;

		if(strcmp(desired_name, curr_name) == 0){

			LLVMValueRef var_ptr = curr->var_ptr;

			return var_ptr;

		}

		curr = curr->next;

	}

	printf("No var pointer matches desired name: %s\n", desired_name);
	
	return NULL;

}





LLVMValueRef primary_node_to_llvm(primary_t primary_node, llvm_context_t* llvm_context){


	LLVMValueRef value;

	if(primary_node.type == PRIMARY_LITERAL){

		value = LLVMConstInt(llvm_context->int_type, primary_node.as.literal.as.integer, 0);	

	}
	else if(primary_node.type == PRIMARY_VARIABLE){

		value = var_ptr_from_name(primary_node.as.variable.name, llvm_context); 

		if(value != NULL){

			LLVMBuildLoad2(llvm_context->builder, llvm_context->int_type, value, "load_var");

		}

	}
	else{

		value = function_call_to_llvm(primary_node.as.func_call, llvm_context);

	}



	return value;

}


LLVMValueRef function_call_to_llvm(func_call_t func_call, llvm_context_t* llvm_context){


	char* callee_name = func_call.callee;

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
	
	return NULL;

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
	else if(binary_expression_node.operator == '='){

		char* name = binary_expression_node.left->as.primary.as.variable.name;
		LLVMValueRef var_ptr = var_ptr_from_name(name, llvm_context);
		if(var_ptr == NULL){
			puts("NULL");
		}
		else{
			puts("NOT NULL");
		}

		LLVMBuildStore(llvm_context->builder, right, var_ptr);

	}

	return value;


}
