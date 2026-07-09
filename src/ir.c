#include <llvm-c/Core.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "ir.h"
#include "free_ast.h"


void generate_llvm_to_file(char* filename, ast_node_t ast){

	LLVMContextRef context = LLVMContextCreate();
	LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
	LLVMModuleRef module = LLVMModuleCreateWithNameInContext(filename, context);
	LLVMTypeRef int_type = LLVMInt32TypeInContext(context);

	llvm_context_t llvm_context = {context, builder, module, int_type, .current_entry = 0, .function_ll_head = NULL, .var_ptr_ll_head = NULL};


	program_t program = ast.as.program;
	program_to_llvm(program, &llvm_context);
	free_ast(ast);


	char* ir_str = LLVMPrintModuleToString(module);

	free_llvm_context_member_structs(llvm_context);
	LLVMDisposeBuilder(builder);
	LLVMDisposeModule(module);
	LLVMContextDispose(context);

	FILE* fp = fopen(filename, "w");
	fprintf(fp, "%s", ir_str);
	LLVMDisposeMessage(ir_str);
	fclose(fp);

}

void free_llvm_context_member_structs(llvm_context_t llvm_context){


	if(llvm_context.function_ll_head != NULL){
		function_ll_node_t* function_ll_head = llvm_context.function_ll_head;
		function_ll_node_t* function_ll_curr = function_ll_head;

		while(function_ll_curr != NULL){

			function_ll_node_t* function_ll_prev = function_ll_curr;
			function_ll_curr = function_ll_curr->next;

			free(function_ll_prev);

		}

	}


	if(llvm_context.var_ptr_ll_head != NULL){
		var_ptr_ll_node_t* var_ptr_ll_head = llvm_context.var_ptr_ll_head;
		var_ptr_ll_node_t* var_ptr_ll_curr = var_ptr_ll_head;

		while(var_ptr_ll_curr != NULL){

			var_ptr_ll_node_t* var_ptr_ll_prev = var_ptr_ll_curr;
			var_ptr_ll_curr = var_ptr_ll_curr->next;

			free(var_ptr_ll_prev->name);
			free(var_ptr_ll_prev);

		}
	}


}


void program_to_llvm(program_t program, llvm_context_t* llvm_context){

	function_list_t function_list = program.function_list;
	function_list_to_llvm(function_list, llvm_context);
	
}


void function_list_to_llvm(function_list_t function_list, llvm_context_t* llvm_context){
	
	for(int i = 0; i < function_list.vector_tree.size; i++){

		function_t function = function_list.vector_tree.children[i]->as.function;
		function_to_llvm(function, llvm_context);
	
	}

}


void function_to_llvm(function_t function, llvm_context_t* llvm_context){

	function_prototype_t function_prototype = function.function_prototype;
	LLVMTypeRef llvm_function_type;

	switch(function_prototype.primitive_type){

		case PRIMITIVE_INT:
			llvm_function_type = LLVMFunctionType(llvm_context->int_type, NULL, 0, 0);
			break;
		default:
			llvm_function_type = LLVMFunctionType(llvm_context->int_type, NULL, 0, 0);
			break;

	}

	char* function_name = function_prototype.function_name;


	LLVMValueRef llvm_function = LLVMAddFunction(llvm_context->module, function_name, llvm_function_type);
	append_function_ll_node(llvm_function, llvm_context);


	LLVMBasicBlockRef llvm_function_entry = LLVMAppendBasicBlockInContext(llvm_context->context, llvm_function, "entry");
	LLVMPositionBuilderAtEnd(llvm_context->builder, llvm_function_entry);
	llvm_context->current_entry = (current_entry_t){.has_return = false, .entry = llvm_function_entry};


	statement_list_t statement_list = function.statement_list;
	statement_list_to_llvm(statement_list, llvm_context);


}


void append_function_ll_node(LLVMValueRef llvm_function, llvm_context_t* llvm_context){

	function_ll_node_t* head = llvm_context->function_ll_head;

	if(head == NULL){

		function_ll_node_t head_value = {llvm_function, NULL};
		llvm_context->function_ll_head = malloc(sizeof(head_value));
		*(llvm_context->function_ll_head) = head_value;

	}
	else if(head->next == NULL){

		function_ll_node_t next_value = {llvm_function, NULL};
		head->next = malloc(sizeof(next_value));
		*(head->next) = next_value;

	}
	else{

		fprintf(stderr, "Internal error: Failed to append function to linked list!\n");
		exit(EXIT_FAILURE);

	}


}


void statement_list_to_llvm(statement_list_t statement_list, llvm_context_t* llvm_context){

	for(int i = 0; i < statement_list.vector_tree.size; i++){

		ast_node_t child_node = *(statement_list.vector_tree.children[i]); 

		if(child_node.type == AST_IF_STATEMENT){

			if_statement_t if_statement = child_node.as.if_statement;
			if_statement_to_llvm(if_statement, llvm_context);

		}
		else if(child_node.type == AST_STATEMENT){

			statement_t statement = child_node.as.statement;
			statement_to_llvm(statement, llvm_context);

		}

	}

}


void if_statement_to_llvm(if_statement_t if_statement, llvm_context_t* llvm_context){
		
	ast_node_t* condition_node = if_statement.condition_node;
	LLVMValueRef llvm_condition_value_i1 = condition_node_to_llvm(*condition_node, llvm_context);


	LLVMValueRef llvm_parent_block = LLVMGetBasicBlockParent(llvm_context->current_entry.entry);
	LLVMBasicBlockRef llvm_then_block = LLVMAppendBasicBlock(llvm_parent_block, "then");
	LLVMBasicBlockRef llvm_end_block = LLVMAppendBasicBlock(llvm_parent_block, "end");

	//conditional branch, always build
	LLVMBuildCondBr(llvm_context->builder, llvm_condition_value_i1, llvm_then_block, llvm_end_block);

	//emit into then block
	statement_list_t then_statement_list = if_statement.statement_list;
	LLVMPositionBuilderAtEnd(llvm_context->builder, llvm_then_block);
	statement_list_to_llvm(then_statement_list, llvm_context);
	LLVMPositionBuilderAtEnd(llvm_context->builder, llvm_then_block);

	//branch to end block only if function not terminated
	if(!llvm_context->current_entry.has_return){
		
		LLVMBuildBr(llvm_context->builder, llvm_end_block);
	}

	//emit into end block
	LLVMPositionBuilderAtEnd(llvm_context->builder, llvm_end_block);

	llvm_context->current_entry.entry = llvm_end_block;
	llvm_context->current_entry.has_return = false;

}


LLVMValueRef condition_node_to_llvm(ast_node_t condition_node, llvm_context_t* llvm_context){

	LLVMValueRef llvm_condition_value_i32;

	if(condition_node.type == AST_BINARY_EXPRESSION){

		llvm_condition_value_i32 = binary_expression_to_llvm(condition_node.as.binary_expression, llvm_context);	

	}
	else if(condition_node.type == AST_PRIMARY){

		llvm_condition_value_i32 = primary_to_llvm(condition_node.as.primary, llvm_context);

	}

	LLVMValueRef llvm_condition_value_i1 = LLVMBuildICmp(llvm_context->builder, LLVMIntNE, llvm_condition_value_i32, LLVMConstInt(LLVMInt32Type(), 0, 0), "boolean");

	return llvm_condition_value_i1;
}


void statement_to_llvm(statement_t statement, llvm_context_t* llvm_context){

	ast_node_t first_child_node = *statement.vector_tree.children[0];

	if(first_child_node.type == AST_KEYWORD){

		ast_node_t second_child_node = *statement.vector_tree.children[1];
		keyword_to_llvm(first_child_node.as.keyword, second_child_node, llvm_context);

	}
	else if(first_child_node.type == AST_BINARY_EXPRESSION){

		LLVMValueRef binary_expression = binary_expression_to_llvm(first_child_node.as.binary_expression, llvm_context);

	}
	else if(first_child_node.type == AST_PRIMARY){

		LLVMValueRef primary = primary_to_llvm(first_child_node.as.primary, llvm_context);

	}	

}


//keyword_to_llvm also emits the second child node of statement to llvm
void keyword_to_llvm(keyword_t keyword, ast_node_t second_child_node, llvm_context_t* llvm_context){

	if(keyword == KEYW_INT){

		variable_declaration_to_llvm(second_child_node, llvm_context);

		if(second_child_node.type == AST_BINARY_EXPRESSION){

			binary_expression_t binary_expression = second_child_node.as.binary_expression;
			binary_expression_to_llvm(binary_expression, llvm_context);

		}

	}
	if(keyword == KEYW_RETURN){

		llvm_context->current_entry.has_return = true;

		LLVMValueRef llvm_return_val;

		if(second_child_node.type == AST_BINARY_EXPRESSION){

			llvm_return_val = binary_expression_to_llvm(second_child_node.as.binary_expression, llvm_context);

		}
		else if(second_child_node.type == AST_PRIMARY){

			llvm_return_val = primary_to_llvm(second_child_node.as.primary, llvm_context);

		}
	
		LLVMBuildRet(llvm_context->builder, llvm_return_val);

	}

}


LLVMValueRef variable_declaration_to_llvm(ast_node_t second_child_node, llvm_context_t* llvm_context){

	variable_t variable;

	if(second_child_node.type == AST_PRIMARY){

		primary_t primary = second_child_node.as.primary;
		variable = primary.as.variable;

	}
	if(second_child_node.type == AST_BINARY_EXPRESSION){

		binary_expression_t binary_expression = second_child_node.as.binary_expression;
		primary_t primary = binary_expression.left->as.primary;
		variable = primary.as.variable;

	}

	primitive_type_t variable_type = variable.primitive_type;
	char* var_name = variable.name;


	LLVMValueRef llvm_var_ptr = NULL;

	if(variable.primitive_type == PRIMITIVE_INT){

		llvm_var_ptr = LLVMBuildAlloca(llvm_context->builder, llvm_context->int_type, var_name);

	}

	append_var_ptr_ll_node(llvm_var_ptr, var_name, llvm_context);
	return llvm_var_ptr;

}


void append_var_ptr_ll_node(LLVMValueRef llvm_var_ptr, char* var_name, llvm_context_t* llvm_context){

	var_ptr_ll_node_t* head = llvm_context->var_ptr_ll_head;

	if(head == NULL){

		var_ptr_ll_node_t head_value = {var_name, llvm_var_ptr, NULL};
		llvm_context->var_ptr_ll_head = malloc(sizeof(head_value));
		*(llvm_context->var_ptr_ll_head) = head_value;

	}
	else if(head->next == NULL){

		var_ptr_ll_node_t next_value = {var_name, llvm_var_ptr, NULL};
		head->next = malloc(sizeof(next_value));
		*(head->next) = next_value;

	}
	else{

		fprintf(stderr, "Internal error: Failed to append var pointer to linked list!\n");
		exit(EXIT_FAILURE);

	}

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

	fprintf(stderr, "Internal error: No var pointer matches desired name: %s\n", desired_name);
	exit(EXIT_FAILURE);

}


LLVMValueRef primary_to_llvm(primary_t primary, llvm_context_t* llvm_context){

	LLVMValueRef value;

	if(primary.type == PRIMARY_LITERAL){

		value = LLVMConstInt(llvm_context->int_type, primary.as.literal.as.integer, 0);	

	}
	else if(primary.type == PRIMARY_VARIABLE){

		LLVMValueRef ptr = var_ptr_from_name(primary.as.variable.name, llvm_context); 

		if(ptr != NULL){

			value = LLVMBuildLoad2(llvm_context->builder, llvm_context->int_type, ptr, "load_var");

		}

	}
	else{

		value = function_call_to_llvm(primary.as.func_call, llvm_context);

	}

	return value;

}


LLVMValueRef function_call_to_llvm(func_call_t func_call, llvm_context_t* llvm_context){

	LLVMTypeRef llvm_callee_type;
	primitive_type_t callee_type = func_call.primitive_type;

	switch(callee_type){
	
		case PRIMITIVE_INT:
			llvm_callee_type = llvm_context->int_type;
			break;
		default:
			llvm_callee_type = llvm_context->int_type;
			break;

	}

	char* callee_name = func_call.callee;


	LLVMValueRef llvm_callee_ref = function_from_name(callee_name, llvm_context);
	LLVMValueRef llvm_result = LLVMBuildCall2(llvm_context->builder, LLVMGlobalGetValueType(llvm_callee_ref), llvm_callee_ref, NULL, 0, "calltmp");

	return llvm_result;	

}


LLVMValueRef function_from_name(char* desired_name, llvm_context_t* llvm_context){

	function_ll_node_t* curr = llvm_context->function_ll_head;

	while(curr->next != NULL){

		char* curr_name = LLVMGetValueName(curr->function);

		if(strcmp(desired_name, curr_name) == 0){

			LLVMValueRef llvm_function = curr->function;
			
			return llvm_function;

		}

		curr = curr->next;

	}

	fprintf(stderr, "Internal error: No function matches desired name: %s\n", desired_name);
	exit(EXIT_FAILURE);
	
}


LLVMValueRef binary_expression_to_llvm(binary_expression_t binary_expression, llvm_context_t* llvm_context){

	LLVMValueRef llvm_result_value;


	LLVMValueRef llvm_left_value;

	if(binary_expression.left->type == AST_PRIMARY){

		primary_t primary = binary_expression.left->as.primary;

		llvm_left_value = primary_to_llvm(primary, llvm_context);

	}


	LLVMValueRef llvm_right_value;

	if(binary_expression.right->type == AST_PRIMARY){

		primary_t primary = binary_expression.right->as.primary;

		llvm_right_value = primary_to_llvm(primary, llvm_context);

	}
	else{

		llvm_right_value = binary_expression_to_llvm(binary_expression.right->as.binary_expression, llvm_context);

	}


	if(binary_expression.operator == '+'){

		llvm_result_value = LLVMBuildAdd(llvm_context->builder, llvm_left_value, llvm_right_value, "sum");

	}
	else if(binary_expression.operator == '='){

		llvm_result_value = llvm_right_value;
		char* name = binary_expression.left->as.primary.as.variable.name;
		LLVMValueRef llvm_var_ptr = var_ptr_from_name(name, llvm_context);

		LLVMBuildStore(llvm_context->builder, llvm_result_value, llvm_var_ptr);

	}


	return llvm_result_value;

}
