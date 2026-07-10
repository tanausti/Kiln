compiler: compiler.c ../lexer/lexer.c ../parser/parser.c ../parser/free_ast.c ../parser/validate_ast.c ../parser/token_stack.c ../ir/ir.c
	gcc compiler.c ../lexer/lexer.c ../parser/parser.c ../parser/free_ast.c ../parser/validate_ast.c ../parser/token_stack.c ../ir/ir.c $$(llvm-config --cflags --ldflags --libs core) -o compiler

compiler_debug: compiler.c ../lexer/lexer.c ../parser/parser.c ../parser/free_ast.c ../parser/validate_ast.c ../parser/token_stack.c ../ir/ir.c
	gcc compiler.c ../lexer/lexer.c ../parser/parser.c ../parser/free_ast.c ../parser/validate_ast.c ../parser/token_stack.c ../ir/ir.c $$(llvm-config --cflags --ldflags --libs core) -Wall -Wextra -g -o compiler
