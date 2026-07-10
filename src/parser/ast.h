#ifndef AST_H
#define AST_H


/*
 

program
    -> function_list

function_list
    -> function*

function
    -> function_prototype statement_list

function_prototype
    -> "int" IDENTIFIER "(" ")"

statement_list
    -> "{" (statement | if statement)* "}"

statement
    -> keyword expression ";"
     | expression ";"

if_statement
    -> "if" "(" expression ")" statement_list

keyword
    -> "return" expression
     | "int" expression

expression
    -> equality

equality
    -> comparison

comparison
    -> term

term
    -> factor ( ("+") factor )*

factor
    -> unary

unary
    -> primary

primary
    -> INTEGER
     | IDENTIFIER
     | IDENTIFIER "(" ")"


*/

typedef struct ast_node ast_node_t;


typedef struct vector_tree {

    ast_node_t** children;

    int size;
    int capacity;

} vector_tree_t;


typedef enum primitive_type {

    PRIMITIVE_INT

} primitive_type_t;


typedef struct literal {

    primitive_type_t primitive_type;

    union {

        int integer;

    } as;

} literal_t;


typedef struct func_call {

    primitive_type_t primitive_type;
    char* callee;

} func_call_t;


typedef struct variable {

    primitive_type_t primitive_type;
    char* name;

} variable_t;


typedef enum primary_type {

    PRIMARY_LITERAL,
    PRIMARY_FUNC_CALL,
    PRIMARY_VARIABLE

} primary_type_t;

typedef struct primary {

    primary_type_t type;

    union {

        literal_t literal;
        func_call_t func_call;
        variable_t variable;

    } as;

} primary_t;


typedef struct function_list {

    vector_tree_t vector_tree;

} function_list_t;

typedef struct function_prototype {

    primitive_type_t primitive_type;
    char* function_name;

} function_prototype_t;

typedef struct statement_list {

    vector_tree_t vector_tree;

} statement_list_t;

typedef struct if_statement {

    ast_node_t* condition_node;
    statement_list_t statement_list;

} if_statement_t;

typedef struct statement {

    vector_tree_t vector_tree;

} statement_t;

typedef struct function {

    function_prototype_t function_prototype;
    statement_list_t statement_list;

} function_t;


typedef enum keyword {

    KEYW_RETURN,
    KEYW_INT,
    KEYW_IF

} keyword_t;


typedef struct binary_expression {

    ast_node_t* left;
    char operator;
    ast_node_t* right;

    primitive_type_t primitive_type;

} binary_expression_t;


typedef struct program {

    function_list_t function_list;

} program_t;


typedef enum ast_node_type {

    AST_ERROR,

    AST_PROGRAM,
    AST_FUNCTION_LIST,
    AST_FUNCTION,
    AST_IF_STATEMENT,
    AST_STATEMENT,

    AST_BINARY_EXPRESSION,

    AST_KEYWORD,
    AST_PRIMARY

} ast_node_type_t;

typedef struct ast_node {

    ast_node_type_t type;

    union {

        program_t program;

        function_t function;

        statement_list_t statement_list;

        if_statement_t if_statement;

        statement_t statement;

        keyword_t keyword;

        binary_expression_t binary_expression;

        primary_t primary;

    } as;

} ast_node_t;


ast_node_t init_error_node();
ast_node_t init_program_node();
ast_node_t init_keyword_node();
ast_node_t init_primary_node();
ast_node_t init_function_node();
ast_node_t init_if_statement_node();
ast_node_t init_statement_node();
ast_node_t init_binary_expression_node();
vector_tree_t init_vector_tree();

void vec_tree_add_right_child(vector_tree_t* parent_tree, ast_node_t child);

#endif
