#ifndef IR_H
#define IR_H



#include <stdio.h>
#include "parser.h"


void print_llvm_to_file(FILE* out, ast_node_t ast);


#endif
