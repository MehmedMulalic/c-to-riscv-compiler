#pragma once

#define MAX_SYMBOLS 2 << 8  // 512

typedef struct ASTNode {
    struct ASTNode *left;
    struct ASTNode *right;
    char *name;
    int type;
} ASTNode;

typedef struct SymbolStatement {
    char *name;
    int type;
} SymbolStatement;

void print_dot(char *filename, ASTNode *root);
void print_dot_node(FILE *f, ASTNode *node, int *counter);

void insert(const char *name);
SymbolStatement *lookup(const char *name);

ASTNode *create_node();
SymbolStatement *create_ss();

ASTNode *make_statement_list(ASTNode *current, ASTNode *next);
ASTNode *make_identifier(const char *name, SymbolStatement *ss);
ASTNode *make_constant(const char *name);
ASTNode *make_unop(const char *op, ASTNode *node_left);
ASTNode *make_binop(const char *op, ASTNode *node_left, ASTNode *node_right);

extern int current_type;
extern size_t symbol_count;
extern SymbolStatement symbol_table[MAX_SYMBOLS];
extern ASTNode *root_node;
