ir_test: ir_test.c ../test_harness.c ../../src/ir.c ../../src/parser.c ../../src/token_stack.c ../../src/free_ast.c ../../src/lexer.c 
	gcc ir_test.c ../test_harness.c ../../src/ir.c ../../src/parser.c ../../src/token_stack.c ../../src/free_ast.c ../../src/lexer.c $$(llvm-config --cflags --ldflags --libs core) -o ir_test


ir_test_debug: ir_test.c ../test_harness.c ../../src/ir.c ../../src/parser.c ../../src/token_stack.c ../../src/free_ast.c ../../src/lexer.c 
	gcc ir_test.c ../test_harness.c ../../src/ir.c ../../src/parser.c ../../src/token_stack.c ../../src/free_ast.c ../../src/lexer.c $$(llvm-config --cflags --ldflags --libs core) -Wall -Wextra -g -o ir_test
