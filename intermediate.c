#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include "intermediate.h"
#include "parser.tab.h"

int current_type = 0;
long current_offset = 0;
size_t symbol_count = 0;
SymbolStatement symbol_table[MAX_SYMBOLS];
ASTNode *root_node = NULL;

/**
 * @brief Parses a string into a 32-bit integer
 * 
 * This function parses a string into an integer in addition to error checks
 * to determine whether the parsed integer contains is within range or
 * whether the string can be converted to a number in the first place.
 * 
 * @param s String to parse
 * @return int32_t Parsed 32-bit integer
 * @return -1 if parsed value it out of bounds
 * @return -2 if parsed value is a string instead of a number
 * @return -3 if parsed value contains characters
 */
int parse_int32(const char *s, int32_t *out) {
    errno = 0;
    char *end;

    long val = strtol(s, &end, 10);
    if (end == s) {
        printf("Error parsing value to int32: No digits found\n");
        return -1;
    }
    if (errno == ERANGE || val < INT32_MIN || val > INT32_MAX) {
        printf("Error parsing value to int32: Value outside of bounds\n");
        return -2;
    }
    if (*end != '\0') {
        printf("Error parsing value to int32: The parsed value contains characters\n");
        return -3;
    }

    *out = (int32_t)val;
    return 0;
}

void handle_err(int err) {
    if (err != 0) {
        printf("Error parsing value: %d\n", err);
        exit(EXIT_FAILURE);
    }
}

ExprResult handle_add(FILE *f, ExprResult left, ExprResult right) {
    ExprResult r;

    fprintf(f, "    add t0, t1, t0\n");
    strcpy(r.reg, "t0");

    return r;
}

ExprResult handle_sub(FILE *f, ExprResult left, ExprResult right) {
    ExprResult r;

    fprintf(f, "    sub t0, t1, t0\n");
    strcpy(r.reg, "t0");

    return r;
}

ExprResult handle_mul(FILE *f, ExprResult left, ExprResult right) {
    ExprResult r;

    fprintf(f, "    mul t0, t1, t0\n");
    strcpy(r.reg, "t0");

    return r;
}

ExprResult handle_div(FILE *f, ExprResult left, ExprResult right) {
    ExprResult r;

    fprintf(f, "    div t0, t1, t0\n");
    strcpy(r.reg, "t0");

    return r;
}

ExprResult handle_mod(FILE *f, ExprResult left, ExprResult right) {
    ExprResult r;

    fprintf(f, "    rem t0, t1, t0\n");
    strcpy(r.reg, "t0");

    return r;
}

ExprResult handle_eq(FILE *f, ExprResult left, ExprResult right) {
    ExprResult r;

    fprintf(f, "    sw t0, %d(fp)\n", left.offset);

    return r;
}

static TokenEntry tokens[] = {
    {"+", handle_add},
    {"-", handle_sub},
    {"*", handle_mul},
    {"/", handle_div},
    {"\%", handle_mod},
    // {"<<", handle_bl},
    // {">>", handle_br},
    // {">", handle_ge},
    // {"<", handle_le},
    // {"<=", handle_leq},
    // {">=", handle_geq},
    {"=", handle_eq},
    // {"==", handle_iseq},
    // {"!=", handle_neq},
    // {"&", handle_band},
    // {"^", handle_xor},
    // {"|", handle_bor},
    // {"&&", handle_and},
    // {"||", handle_or},
    // {"IF", handle_if},
    // {"ELSE", handle_else},
    // {"SWITCH", handle_switch},
    // {"WHILE", handle_while},
    // {"DO", handle_do},
    // {"FOR", handle_for},
    {NULL, NULL}
};

void print_dot(const char *filename, ASTNode *root) {
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
    current_offset -= 4;

    symbol_table[symbol_count].name = strdup(name);
    symbol_table[symbol_count].type = current_type;
    symbol_table[symbol_count].offset = current_offset;

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

int isLeaf(ASTNode *node) {
    if ((node->left == NULL) && (node->right == NULL))
        return 1;
    
    return 0;
}

/**
 * @brief Returns the function assigned to the desired token.
 * 
 * This function checks the tokens array and returns the matching
 * token's function, otherwise returns NULL.
 * 
 * @param node The node in question, representing an operation (+, IF, etc.)
 * @return token_fn Function handler
 */
token_fn token_handler(ASTNode *node) {
    for (int i = 0; tokens[i].token != NULL; i++) {
        if (strcmp(node->name, tokens[i].token) == 0) {
            return tokens[i].fn;
        }
    }

    printf("Expression not found: %s\n", node->name);
    return NULL;
}

void generate_code(ASTNode *root) {
    if (root == NULL) return;
    FILE *f = fopen(RISC_FILENAME, "w");

    fprintf(f, ".text\n");
    fprintf(f, ".globl main\n\n");

    fprintf(f, "main:\n");
    generate_symbol_code(f);
    generate_node_code(f, root);

    fprintf(f, "\n    li a7, 93\n");
    fprintf(f, "    ecall\n");
    fclose(f);
}

void generate_symbol_code(FILE *f) {
    // 16-bit alignment
    int times = (symbol_count + 3) / 4;
    int bytes = times * 16;
    fprintf(f, "    addi sp, sp, -%d\n", bytes);
}

ExprResult generate_node_code(FILE *f, ASTNode *node) {
    if (node == NULL) {
        printf("Node is NULL\n");
        ExprResult r = {0};
        return r;
    }
    
    // Leaf node
    if (isLeaf(node)) {
        SymbolStatement *sym = lookup(node->name);
        ExprResult r;

        // Identifier
        if (sym != NULL) {
            fprintf(f, "    lw t0, %d(fp)\n", sym->offset);

            r.is_var = 1;
            r.offset = sym->offset;
            strcpy(r.reg, "t0");

            return r;
        }
        
        // Constant
        fprintf(f, "    li t0, %s\n", node->name);

        r.is_const = 1;
        handle_err(parse_int32(node->name, &r.int_val));
        strcpy(r.reg, "t0");

        return r;
    }
    
    // Parent node
    ExprResult left = generate_node_code(f, node->left);
    fprintf(f, "    mv t1, t0\n");
    
    ExprResult right = generate_node_code(f, node->right);
    token_fn fn = token_handler(node);

    if (fn) {
        return fn(f, left, right);
    }

    ExprResult r;
    printf("No function to execute\n");
    return r;
}
