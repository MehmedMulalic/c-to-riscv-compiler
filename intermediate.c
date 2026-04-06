#include <stdio.h>
#include <stdlib.h>
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

    fprintf(f, "  node%d [label=\"%s\"];\n", id, node->name);

    if (node->left != NULL) {
        int left_id = *counter;
        print_dot_node(f, node->left, counter);
        fprintf(f, "  node%d -> node%d;\n", id, left_id);
    }
    if (node->right != NULL) {
        int right_id = *counter;
        print_dot_node(f, node->right, counter);
        fprintf(f, "  node%d -> node%d;\n", id, right_id);
    }
}

void insert(char *name) {
    if (symbol_count >= MAX_SYMBOLS) {
        printf("ERROR - Symbol table at full capacity.\n");
        return;
    }

    symbol_table[symbol_count].name = strdup(name);
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

ASTNode *make_statement_list(ASTNode *current, ASTNode *next) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        printf("ERROR - failed to initialize node\n");
        return NULL;
    }

    node->left = current;
    node->right = next;
    node->name = strdup("STLIST");

    return node;
}

ASTNode *make_identifier(char *name, SymbolStatement *ss) {
    if (ss == NULL) {
        printf("ERROR - identifier not declared\n");
        return NULL;
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        printf("ERROR - failed to initialize node\n");
        return NULL;
    }

    node->name = strdup(name);
    return node;
}

ASTNode *make_constant(char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        printf("ERROR - failed to initialize node\n");
        return NULL;
    }

    node->name = strdup(name);
    return node;
}

ASTNode *make_binop(char *op, ASTNode *node1, ASTNode *node2) {
    if (node1 == NULL || node2 == NULL) {
        printf("ERROR - NULL symbols during binop creation\n");
        return NULL;
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        printf("ERROR - failed to initialize node\n");
        return NULL;
    }
    ASTNode *node_left = malloc(sizeof(ASTNode));
    if (!node) {
        printf("ERROR - failed to initialize node\n");
        return NULL;
    }
    ASTNode *node_right = malloc(sizeof(ASTNode));
    if (!node) {
        printf("ERROR - failed to initialize node\n");
        return NULL;
    }

    node->left = node_left;
    node->right = node_right;
    node->name = strdup(op);

    return node;
}
