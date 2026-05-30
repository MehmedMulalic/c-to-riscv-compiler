#pragma once

#define MAX_SYMBOLS 2 << 8  // 512
#define RISC_FILENAME "program.s"

typedef struct ASTNode {
    struct ASTNode *left;
    struct ASTNode *right;
    char *name;
    int type;
} ASTNode;

typedef struct SymbolStatement {
    char *name;
    int type;
    int offset;
} SymbolStatement;

typedef void (*token_fn)(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
typedef struct {
    const char *token;
    token_fn fn;
} TokenEntry;

int parse_int32(const char *s, int32_t *out);
void check_ss(SymbolStatement *ss1, SymbolStatement *ss2);

void handle_err(int err);
void handle_add(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_sub(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_mul(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_div(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_mod(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_bl(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_br(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_ge(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_le(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_leq(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_geq(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_eq(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_iseq(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_neq(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_band(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_xor(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_bor(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_and(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_or(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_if(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_else(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_switch(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_while(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_do(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);
void handle_for(FILE *f, SymbolStatement *ss_left, SymbolStatement *ss_right);

void print_dot(const char *filename, ASTNode *root);
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

int isLeaf(ASTNode *node);
int isVariable(ASTNode *node);
token_fn token_handler(ASTNode *node);

void generate_code(ASTNode *root);
SymbolStatement *generate_node_code(FILE *f, ASTNode *node);

extern int current_type;
extern long current_offset;
extern size_t symbol_count;
extern SymbolStatement symbol_table[MAX_SYMBOLS];
extern ASTNode *root_node;
