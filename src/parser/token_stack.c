#include <stdio.h>
#include <stdlib.h>
#include "../lexer/lexer.h"
#include "token_stack.h"


token_stack_node_t* token_stack(FILE* cF, pos_t* lc){

	token_stack_node_t* curr = (token_stack_node_t*)malloc(sizeof(token_stack_node_t));

	curr->token = (token_t*)malloc(sizeof(token_t));
	*(curr->token) = next_token(cF, lc);

	token_stack_node_t* head = curr;

	while(curr->token->type != TOK_EOF){

		curr->next = (token_stack_node_t*)malloc(sizeof(token_stack_node_t));

		curr = curr->next;
		curr->token = (token_t*)malloc(sizeof(token_t));		
		*(curr->token) = next_token(cF, lc);

	}

	curr->next = NULL;

	return head;

}


void pop_token_node(token_stack_node_t** curr){

	if(curr != NULL && *curr != NULL){

		token_stack_node_t* prev = *curr;
		(*curr) = peek_token_node(curr);
		free_token_stack_node(prev);

	}
	else{

		fprintf(stderr, "Internal error: Attempt to pop nonexistent token node!\n");

	}
}


void free_token_stack_node(token_stack_node_t* curr){

	free(curr->token->string);
	free(curr->token);
	free(curr);

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
