#ifndef TOKEN_STACK_H
#define TOKEN_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"


typedef struct token_stack_node{

	token_t* token;
	struct token_stack_node* prev;
	struct token_stack_node* next;

}token_stack_node_t;



token_stack_node_t* token_stack(FILE* cF, pos_t* lc);
token_stack_node_t* pop_token(token_stack_node_t** curr);
token_stack_node_t* peek_token(token_stack_node_t* curr);


#endif
