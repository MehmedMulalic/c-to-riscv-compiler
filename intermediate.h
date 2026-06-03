#pragma once

#define MAX_SYMBOLS 2 << 8  // 512
#define MAX_EXPR_DEPTH 40
#define TEMP_REG_ALLOC (MAX_EXPR_DEPTH * 4) // 40 words
#define RISC_FILENAME "program.s"

typedef enum {
    NODE_NULL,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_MOD,
    NODE_BL,
    NODE_BR,
    NODE_GE,
    NODE_LE,
    NODE_LEQ,
    NODE_GEQ,
    NODE_EQ,
    NODE_ISEQ,
    NODE_NEQ,
    NODE_BAND,
    NODE_XOR,
    NODE_BOR,
    NODE_AND,
    NODE_OR
} NodeData;

typedef enum {
    NODE_STATEMENTS,
    NODE_OPERAND,
    NODE_IDENTIFIER,
    NODE_CONSTANT,
    NODE_IF,
    NODE_SWITCH,
    NODE_ELSE,
    NODE_WHILE,
    NODE_DO,
    NODE_FOR,
    NODE_WHILE_COND
} NodeKind;

typedef struct ASTNode {
    NodeKind kind;
    NodeData op;
    struct ASTNode *left;
    struct ASTNode *right;
    char *name;
    int type;
    int offset;
} ASTNode;

typedef struct SymbolStatement {
    char *name;
    int type;
    int offset;
} SymbolStatement;

int new_label();
void insert(const char *name);
SymbolStatement *lookup(const char *name);

ASTNode *create_node();
ASTNode *make_statement_list(ASTNode *current, ASTNode *next);
ASTNode *make_identifier(const char *name, SymbolStatement *ss);
ASTNode *make_constant(const char *name);
ASTNode *make_unop(const char *op, ASTNode *node_left);
ASTNode *make_binop(NodeData nodedata, ASTNode *node_left, ASTNode *node_right);
ASTNode *make_conditional(NodeKind nodekind, ASTNode *node_left, ASTNode *node_right);
ASTNode *make_while_cond(ASTNode *expr);

void generate_code(ASTNode *root);
void generate_symbol_code(FILE *f);
void generate_node_code(FILE *f, ASTNode *node);

extern int current_type;
extern int label_count;
extern int expr_depth;
extern long current_offset;
extern size_t symbol_count;
extern SymbolStatement symbol_table[MAX_SYMBOLS];
extern ASTNode *root_node;
