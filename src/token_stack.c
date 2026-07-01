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

	curr->next = NULL;



	return head;


}




void pop_token_node(token_stack_node_t** curr){

	if(curr != NULL && *curr != NULL){

		token_stack_node_t prev = **curr;
		(*curr) = peek_token_node(curr);

	}
	else{

		fprintf(stderr, "Internal error: Attempt to pop nonexistent token node!\n");

	}
}



token_stack_node_t* peek_token_node(token_stack_node_t** curr){

	if(curr != NULL && *curr != NULL){
		return (**curr).next;
	}
	else{

		fprintf(stderr, "Internal error: Attempt to peek null token node!\n");	
		return NULL;
	}


}
