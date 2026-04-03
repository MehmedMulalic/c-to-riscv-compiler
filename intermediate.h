#pragma once

#define MAX_SYMBOLS 2 << 8  // 512

typedef enum {
    NODE_BINOP,
    NODE_UNOP,
    NODE_CONSTANT,
    NODE_IDENTIFIER,
    NODE_RETURN,
    NODE_IF
} NodeType;

typedef struct ASTNode {
    NodeType node_type;
    int op;
    struct ASTNode *left;
    struct ASTNode *right;
    char name[64];
} ASTNode;

typedef struct SymbolStatement {
    char name[128];
    int type;
} SymbolStatement;

void print_dot(char *filename, ASTNode *root);
void print_dot_node(FILE *f, ASTNode *node, int *counter);
char *get_label(ASTNode *node);

void insert(char *name);
SymbolStatement *lookup(char *name);

ASTNode *make_constant(char *name);
ASTNode *make_identifier(char *name);
ASTNode *make_binop(int op_type, char *var1, char *var2);
