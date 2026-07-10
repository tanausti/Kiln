ir_test: ir_test.c ../test_harness.c ../../src/ir/ir.c ../../src/parser/parser.c ../../src/parser/ast.h ../../src/parser/token_stack.c ../../src/parser/free_ast.c ../../src/lexer/lexer.c 
	gcc  ir_test.c ../test_harness.c ../../src/ir/ir.c ../../src/parser/parser.c ../../src/parser/ast.h ../../src/parser/token_stack.c ../../src/parser/free_ast.c ../../src/lexer/lexer.c $$(llvm-config --cflags --ldflags --libs core) -o ir_test


ir_test_debug: ir_test.c ../test_harness.c ../../src/ir/ir.c ../../src/parser/parser.c ../../src/parser/ast.h ../../src/parser/token_stack.c ../../src/parser/free_ast.c ../../src/lexer/lexer.c 
	gcc  ir_test.c ../test_harness.c ../../src/ir/ir.c ../../src/parser/parser.c ../../src/parser/ast.h ../../src/parser/token_stack.c ../../src/parser/free_ast.c ../../src/lexer/lexer.c $$(llvm-config --cflags --ldflags --libs core) -Wall -Wextra -g -o ir_test
