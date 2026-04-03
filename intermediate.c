#include <stdio.h>
#include <string.h>
#include "intermediate.h"

int current_type;
size_t symbol_count = 0;
SymbolStatement symbol_table[MAX_SYMBOLS];
ASTNode *root_node = NULL;

void print_dot(char *filename, ASTNode *root) {
    FILE *f = fopen(filename, "w");
    fprintf(f, "digraph AST {\n");
    fprintf(f, "  edge [arrowhead=none];\n");

    int counter = 0;
    print_dot_node(f, root, &counter);
    
    fprintf(f, "}\n");
    fclose(f);
}

void print_dot_node(FILE *f, ASTNode *node, int *counter) {
    if (node == NULL) return;

    int id = (*counter)++;

    fprintf(f, "  node%d [label=\"%s\"];\n", id, get_label(node));

    if (node->right != NULL) {
        int right_id = *counter;
        print_dot_node(f, node->right, right_id);
        fprintf(f, "  node%d -> node%d;\n", id, right_id);
    }
    if (node->left != NULL) {
        int left_id = *counter;
        print_dot_node(f, node->left, left_id);
        fprintf(f, "  node%d -> node%d;\n", id, left_id);
    }
}

char *get_label(ASTNode *node) {
    switch (node->node_type) {
        case NODE_BINOP:        
        case NODE_UNOP:
        case NODE_CONSTANT:     return node->name;
        case NODE_IDENTIFIER:   return node->name;
        case NODE_RETURN:
        case NODE_IF:
    }
}

void insert(char *name) {
    printf("DEBUG insert - %s\n", name);
    if (symbol_count >= MAX_SYMBOLS) {
        printf("ERROR - Symbol table at full capacity.\n");
        return;
    }

    strcpy(symbol_table[symbol_count].name, name);
    symbol_table[symbol_count].type = current_type;

    symbol_count++;
}

SymbolStatement *lookup(char *name) {
    for (size_t i=0; i<symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return &symbol_table[i];
        }
    }

    return NULL;
}

ASTNode *make_constant(char *name) {
    ASTNode node = {};
    node.node_type = NODE_CONSTANT;
    snprintf(node.name, sizeof(node.name), name);
    printf("DEBUG constant - %s\n", name);

    return &node;
}

ASTNode *make_identifier(char *name) {
    ASTNode node = {};
    node.node_type = NODE_IDENTIFIER;
    snprintf(node.name, sizeof(node.name), name);
    printf("DEBUG identifier - %s\n", name);

    return &node;
}

ASTNode *make_binop(int op_type, char *var1, char *var2) {
    ASTNode node = {};

    return &node;
}