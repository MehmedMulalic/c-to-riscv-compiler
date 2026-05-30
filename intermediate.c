#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include "intermediate.h"
#include "parser.tab.h"

int current_type;
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
 */
int parse_int32(const char *s, int32_t *out) {
    errno = 0;
    char *end;

    long val = strtol(s, &end, 10);
    if (end == s) {
        printf("Error parsing value to int32: No digits found\n");
        return 1;
    }
    if (errno == ERANGE || val < INT32_MIN || val > INT32_MAX) {
        printf("Error parsing value to int32: Value outside of bounds\n");
        return 2;
    }
    if (*end != '\0') {
        printf("Error parsing value to int32: The parsed value contains characters\n");
        return 3;
    }

    *out = (int32_t)val;
    return 0;
}

/* IN CASE THE INTERPRETER DOES NOT ASSUME INTEGERS ONLY
=== delete otherwise ===

int get_data_type_bytes(int type) {
    switch(type) {
        case CHAR: return 1;
        case INT: return 4;
        case FLOAT: return 4;
        case DOUBLE: return 8;
        default: return -1;
    }
}

void print_ss_value(SymbolStatement *ss, double value) {
    if (ss->type == INT) {
        printf("    li t0, %d\n", (int)value);
        return;
    }
    if (ss->type == FLOAT) {
        printf("    %f\n", (float)value);
        return;
    }
    if (ss->type == DOUBLE) {
        printf("    %f\n", value);
        return;
    }

    printf("Error assigning value to DynamicVariable: %f\n", value);
    return;
}


type_fn 
static DataTypeEntry datatypes[] = {
    {1, convert_char},
    {INT, to_int},
    {NULL, NULL}
};

int convert_int(double v) {
    return int(v);
}
*/

void check_ss(SymbolStatement *ss1, SymbolStatement *ss2) {
    if ((ss1 == NULL) || (ss2 == NULL)) {
        printf("Null symbol statement\n");
        exit(EXIT_FAILURE);
    }
}

void handle_err(int err) {
    if (err != 0) {
        printf("Error parsing value: %d\n", err);
        exit(EXIT_FAILURE);
    }
}

void handle_add(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {
    check_ss(ss_left, ss_right);

    int32_t c_left, c_right;
    int err = 0;

    // If ss is a constant
    if (isdigit((unsigned char)ss_left->name[0])) {
        err = parse_int32(ss_left->name, &c_left);
        handle_err(err);
    }
    if (isdigit((unsigned char)ss_right->name[0])) {
        err = parse_int32(ss_right->name, &c_right);
        handle_err(err);
    }

    // 5 + 3
    fprintf(f, "   li t0, %d\n", c_left);
    fprintf(f, "   li t1, %d\n", c_right);
    fprintf(f, "   add t2, t0, t1\n");

    // a + 5 i obrnuto


    // a + b
    fprintf(f, "   lw t0, %d(fp)\n", ss_left->offset);
    fprintf(f, "   lw t1, %d(fp)\n", ss_right->offset);
    fprintf(f, "   add t2, t0, t1\n");
}
void handle_sub(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_mul(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_div(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_mod(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_bl(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_br(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_ge(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_le(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_leq(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_geq(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_eq(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {
    int32_t c_right;
    int err = 0;
    if (isdigit((unsigned char)ss_right->name[0])) {
        err = parse_int32(ss_right->name, &c_right);
        handle_err(err);
    }

    // a = 0
    fprintf(f, "   li t0, %d\n", c_right);
    fprintf(f, "   sw t0, %d(fp)\n", ss_left->offset);

    // a = b
    fprintf(f, "   lw t0, %d(fp)\n", ss_right->offset);
    fprintf(f, "   sw t0, %d(fp)\n", ss_left->offset);
}
void handle_iseq(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_neq(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_band(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_xor(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_bor(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_and(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_or(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_if(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_else(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_switch(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_while(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_do(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}
void handle_for(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right) {}

static TokenEntry tokens[] = {
    {"+", handle_add},
    {"-", handle_sub},
    {"*", handle_mul},
    {"/", handle_div},
    {"\%", handle_mod},
    {"<<", handle_bl},
    {">>", handle_br},
    {">", handle_ge},
    {"<", handle_le},
    {"<=", handle_leq},
    {">=", handle_geq},
    {"=", handle_eq},
    {"==", handle_iseq},
    {"!=", handle_neq},
    {"&", handle_band},
    {"^", handle_xor},
    {"|", handle_bor},
    {"&&", handle_and},
    {"||", handle_or},
    {"IF", handle_if},
    {"ELSE", handle_else},
    {"SWITCH", handle_switch},
    {"WHILE", handle_while},
    {"DO", handle_do},
    {"FOR", handle_for},
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

int isLeaf(ASTNode *node) {
    if ((node->left == NULL) && (node->right == NULL))
        return 1;
    
    return 0;
}

int isVariable(ASTNode *node) {
    SymbolStatement *symbol = lookup(node->name);
    if (symbol != NULL)
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

    return NULL;
}

void generate_code(ASTNode *root) {
    if (root == NULL) return;
    FILE *f = fopen(RISC_FILENAME, "w");

    fprintf(f, ".text\n");
    fprintf(f, ".globl main\n\n");

    fprintf(f, "main:\n");
    generate_node_code(f, root);

    fprintf(f, "\n    li a7, 93\n");
    fprintf(f, "    ecall\n");
    fclose(f);
}

SymbolStatement *generate_node_code(FILE *f, ASTNode *node) {
    if (node == NULL) return NULL;
    
    // Leaf node
    if (isLeaf(node)) {
        // Identifier
        if (lookup(node->name) != NULL) {
            return lookup(node->name);
        }
        
        // Constant
        SymbolStatement *ss = create_ss();
        ss->name = strdup(node->name);
        ss->type = node->type;

        return ss;
    }
    
    // Parent node
    token_fn current_operation = NULL;
    SymbolStatement *ss_left = NULL;
    SymbolStatement *ss_right = NULL;

    if (token_handler(node) != NULL) {
        current_operation = token_handler(node);
    }
    if (node->left != NULL) {
        ss_left = generate_node_code(f, node->left);
    }
    if (node->right != NULL) {
        ss_right = generate_node_code(f, node->right);
    }
    if (ss_left != NULL && ss_right != NULL) {
        current_operation(f, ss_left, ss_right);
    }

    SymbolStatement *ss = create_ss();
    // ss->name

    //* ovdje sam po izgledu stao htio sam napraviti ss haj ga sad znaj lol

    return ss;
}
