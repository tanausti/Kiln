#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>


#define MAX_TOKEN_LENGTH 128

typedef enum token_type{


	TOK_EOF,

	TOK_INT_TYPE,
	TOK_RETURN,
	TOK_IF,

	TOK_INT_LITERAL,

	TOK_PLUS,
	TOK_EQ,

	TOK_IDENTIFIER,

	TOK_LPARENTH,
	TOK_RPARENTH,
	TOK_LBRACE,
	TOK_RBRACE,
	TOK_SEMI,

	TOK_ERROR

}token_type_t;


typedef struct token{


	token_type_t type;
	char* string;
	int line;
	int column;

}token_t;


typedef struct pos{

	int line;
	int column;


}pos_t;



token_t next_token(FILE* cF, pos_t* lc);
int advance_char(FILE *cF, pos_t* lc);
int look_ahead(FILE *cF);
token_t create_error_token(FILE* cF, pos_t* lc, int start_column, char* prev_str);
token_t create_constant_token(FILE* cF, char c, pos_t* lc);
token_t create_keyword_or_identifier_token(FILE* cF, char c, pos_t* lc);



#endif
