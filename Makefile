compiler: src/compiler/compiler.c src/lexer/lexer.c src/parser/parser.c src/parser/free_ast.c src/parser/validate_ast.c src/parser/token_stack.c src/ir/ir.c
	gcc src/compiler/compiler.c src/lexer/lexer.c src/parser/parser.c src/parser/free_ast.c src/parser/validate_ast.c src/parser/token_stack.c src/ir/ir.c $$(llvm-config --cflags --ldflags --libs core) -o compiler
