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

typedef struct ExprResult {
    int is_var;
    int is_const;
    int32_t int_val;
    int offset;
    char reg[4];
} ExprResult;

typedef ExprResult (*token_fn)(FILE *f, ExprResult left, ExprResult right);
typedef struct {
    const char *token;
    token_fn fn;
} TokenEntry;

int parse_int32(const char *s, int32_t *out);

void handle_err(int err);
ExprResult handle_add(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_sub(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_mul(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_div(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_mod(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_bl(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_br(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_ge(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_le(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_leq(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_geq(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_eq(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_iseq(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_neq(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_band(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_xor(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_bor(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_and(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_or(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_if(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_else(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_switch(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_while(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_do(FILE *f, ExprResult left, ExprResult right);
ExprResult handle_for(FILE *f, ExprResult left, ExprResult right);

void print_dot(const char *filename, ASTNode *root);
void print_dot_node(FILE *f, ASTNode *node, int *counter);

void insert(const char *name);
SymbolStatement *lookup(const char *name);

ASTNode *create_node();
ASTNode *make_statement_list(ASTNode *current, ASTNode *next);
ASTNode *make_identifier(const char *name, SymbolStatement *ss);
ASTNode *make_constant(const char *name);
ASTNode *make_unop(const char *op, ASTNode *node_left);
ASTNode *make_binop(const char *op, ASTNode *node_left, ASTNode *node_right);

int isLeaf(ASTNode *node);
token_fn token_handler(ASTNode *node);

void generate_code(ASTNode *root);
void generate_symbol_code(FILE *f);
ExprResult generate_node_code(FILE *f, ASTNode *node);

extern int current_type;
extern long current_offset;
extern size_t symbol_count;
extern SymbolStatement symbol_table[MAX_SYMBOLS];
extern ASTNode *root_node;
