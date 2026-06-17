#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "token_stack.h"




token_stack_node_t* token_stack(FILE* cF, pos_t* lc){


	token_stack_node_t* curr = (token_stack_node_t*)malloc(sizeof(token_stack_node_t));

	curr->token = (token_t*)malloc(sizeof(token_t));
	*(curr->token) = next_token(cF, lc);
	curr->prev = NULL;

	token_stack_node_t* head = curr;

	while(curr->token->type != TOK_EOF){

		token_stack_node_t* temp_prev = curr;

		curr->next = (token_stack_node_t*)malloc(sizeof(token_stack_node_t));

		curr = curr->next;
		curr->token = (token_t*)malloc(sizeof(token_t));
		*(curr->token) = next_token(cF, lc);

		curr->prev = temp_prev;


	}



	return head;


}




token_stack_node_t* pop_token(token_stack_node_t** curr){

	token_stack_node_t* node = *curr;
	*curr = peek_token(*curr);

	return node;

}



token_stack_node_t* peek_token(token_stack_node_t* curr){

	return curr->next;


}




void free_tokens(token_stack_node_t* top){


	free(top->token->string);
	free(top->token);

	if(top->prev != NULL){
		free_tokens(top->prev);	
	}

	free(top);




}
