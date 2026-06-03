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




token_t create_constant_token(FILE *cF, char c, int* lc){

	char constant[MAX_TOKEN_LENGTH] = "";

	int constant_start_column = lc[1];


	//read until end of constant string, append to constant
	while(look_ahead(cF) >= '0' && look_ahead(cF) <= '9'){
		
		char addition_str[2] = {c, '\0'};
		strcat(constant, addition_str);

		c = advance_char(cF, lc);

	}


	char addition_str[2] = {c, '\0'};
	strcat(constant, addition_str);

	int line = lc[0];
	int column = lc[1];

	char* string = strdup(constant);

	return (token_t){TOK_INT_LITERAL, string, line, constant_start_column};



}





token_t create_keyword_or_identifier_token(FILE *cF, char c, int* lc){

	char word[MAX_TOKEN_LENGTH] = "";

	int word_start_column = lc[1];


	//read until end of word, append to word
	while(isalpha(look_ahead(cF)) || look_ahead(cF) == '_'){


		char addition_str[2] = {c, '\0'};
		strcat(word, addition_str);

		c = advance_char(cF, lc);


	}


	char addition_str[2] = {c, '\0'};
	strcat(word, addition_str);


	int line = lc[0];
	int column = lc[1];

	char* string = strdup(word);


	if(strcmp(string, "int") == 0){
	
		return (token_t){TOK_INT_TYPE, string, line, word_start_column};

	}
	else if(strcmp(string, "return") == 0){


		return (token_t){TOK_RETURN, string, line, word_start_column};


	}
	else{


		return (token_t){TOK_IDENTIFIER, string, line, word_start_column};

	}


}




int look_ahead(FILE* cF){

	int c = fgetc(cF);

	ungetc(c, cF);

	return c;

}
