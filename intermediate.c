#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intermediate.h"

int current_type;
size_t symbol_count = 0;
SymbolStatement symbol_table[MAX_SYMBOLS];
ASTNode *root_node = NULL;

ASTNode *create_node() {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) {
        printf("ERROR - failed to initialize node\n");
        return NULL;
    }

    return node;
}

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

void insert(const char *name) {
    if (symbol_count >= MAX_SYMBOLS) {
        printf("ERROR - Symbol table at full capacity.\n");
        return;
    }

    symbol_table[symbol_count].name = strdup(name);
    symbol_table[symbol_count].type = current_type;

    symbol_count++;
}

SymbolStatement *lookup(const char *name) {
    for (size_t i=0; i<symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return &symbol_table[i];
        }
    }

    return NULL;
}

ASTNode *create_node() {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if (!node) {
        printf("ERROR - failed to initialize node\n");
        return NULL;
    }

    return node;
}

SymbolStatement *create_ss() {
    SymbolStatement *ss = calloc(1, sizeof(SymbolStatement));
    if (!ss) {
        printf("ERROR - failed to initialize symbol statement\n");
        return NULL;
    }

    return ss;
}

ASTNode *make_statement_list(ASTNode *current, ASTNode *next) {
    ASTNode *node = create_node();

    node->left = current;
    node->right = next;
    node->name = "STATEMENTS";

    return node;
}

ASTNode *make_identifier(const char *name, SymbolStatement *ss) {
    if (ss == NULL) {
        return NULL;
    }

    ASTNode *node = create_node();
    node->name = strdup(name);

    return node;
}

ASTNode *make_constant(const char *name) {
    ASTNode *node = create_node();
    node->name = strdup(name);
    node->type = current_type;

    return node;
}

ASTNode *make_unop(const char *op, ASTNode *node_left) {
    if (node_left == NULL) {
        printf("ERROR - NULL node during unop creation\n");
        return NULL;
    }

    ASTNode *node = create_node();
    node->left = node_left;
    node->name = strdup(op);

    return node;
}

ASTNode *make_binop(const char *op, ASTNode *node_left, ASTNode *node_right) {
    if (node_left == NULL || node_right == NULL) {
        printf("ERROR - NULL node during binop creation\n");
        return NULL;
    }

    ASTNode *node = create_node();
    node->left = node_left;
    node->right = node_right;
    node->name = strdup(op);

    return node;
}
