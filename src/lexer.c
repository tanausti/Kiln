#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"

#define MAX_TOKEN_LENGTH 128




token_t next_token(FILE *cF, int* lc){

	int c = -1;

	do{

		c = advance_char(cF, lc);

	}
	while(isspace(c));



	int line = lc[0];
	int column = lc[1];


	switch(c){

		case EOF:
			{
				char* string = strdup("EOF");
				return (token_t){TOK_EOF, string, line, column};
				break;
			}
		case '+':
			{
				char* string = strdup("+");
				return (token_t){TOK_PLUS, string, line, column};
				break;
			}

		case '(':
			{
				char* string = strdup("(");
				return (token_t){TOK_LPARENTH, string, line, column};
				break;
			}

		case ')':
			{
				char* string = strdup(")");
				return (token_t){TOK_RPARENTH, string, line, column};
				break;
			}

		case '{':
			{
				char* string = strdup("{");
				return (token_t){TOK_LBRACE, string, line, column};
				break;
			}

		case '}':
			{
				char* string = strdup("}");
				return (token_t){TOK_RBRACE, string, line, column};
				break;
			}

		case ';':
			{
				char* string = strdup(";");
				return (token_t){TOK_SEMI, string, line, column};
				break;
			}

	}





	if(c >= '0' && c <= '9'){

		return create_constant_token(cF, c, lc);

	}

	if(isalpha(c) || c == '_'){

		return create_keyword_or_identifier_token(cF, c, lc);

	}

	return (token_t){TOK_UNKNOWN, "UNKNOWN", line, column};


}



int look_ahead(FILE* cF){

	int c = fgetc(cF);

	ungetc(c, cF);

	return c;

}



int advance_char(FILE *cF, int* lc){

	int* line = lc;
	int* column = lc + 1;

	int c = fgetc(cF);

	if(c == '\n' && look_ahead(cF) != EOF){

		(*line)++;
		*column = 0;


	}
	else if(c == '\t'){

		(*column) += 7;
	
	}
	else{

		(*column)++;
	}

	return c;

}



int retreat_char(FILE *cF, int* lc, int* retreat_data){

	int* line = lc;
	int* column = lc + 1;

	char c = retreat_data[0];
	char prev_column = retreat_data[1];


	int prev_char = ungetc(c, cF);

	if(c == '\n'){

		(*line)--;

	}

	*column = prev_column;

	return prev_char;

}


token_t create_constant_token(FILE *cF, char c, int* lc){

	char constant[MAX_TOKEN_LENGTH] = "";

	int start_column = lc[1];
	int prev_column = lc[1];

	do{

		char addition_str[2] = {c, '\0'};
		strcat(constant, addition_str);

		c = advance_char(cF, lc);


		prev_column = lc[1];

	}
	while(c >= '0' && c <= '9');


	int retreat_data[2] = {c, prev_column - 1};
	c = retreat_char(cF, lc, retreat_data);


	int line = lc[0];
	int column = lc[1];

	char* string = strdup(constant);

	return (token_t){TOK_INT_LITERAL, string, line, start_column};



}





token_t create_keyword_or_identifier_token(FILE *cF, char c, int* lc){

	char word[MAX_TOKEN_LENGTH] = "";

	int start_column = lc[1];
	int prev_column = lc[1];

	do{

	
		char addition_str[2] = {c, '\0'};
		strcat(word, addition_str);

		c = advance_char(cF, lc);

		prev_column = lc[1];

	}
	while(isalpha(c) || c == '_');

	int retreat_data[2] = {c, prev_column - 1};
	c = retreat_char(cF, lc, retreat_data);


	int line = lc[0];
	int column = lc[1];

	char* string = strdup(word);


	if(strcmp(string, "int") == 0){

		return (token_t){TOK_INT_TYPE, string, line, start_column};


	}
	else if(strcmp(string, "return") == 0){


		return (token_t){TOK_RETURN, string, line, start_column};


	}
	else{


		return (token_t){TOK_IDENTIFIER, string, line, start_column};

	}


}
