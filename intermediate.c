#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#include "intermediate.h"
#include "parser.tab.h"

#define IS_FLOAT(t) ((t) == FLOAT)

int current_type = 0;
int label_count = 0;
int float_const_count = 0;
int expr_depth = 0;
long current_offset = 0;
size_t symbol_count = 0;
SymbolStatement symbol_table[MAX_SYMBOLS];
ASTNode *root_node = NULL;
FloatConst float_consts[MAX_FLOAT_CONSTS];

static const char *intern_float_const(const char *value) {
    for (int i = 0; i < float_const_count; i++) {
        if (strcmp(float_consts[i].value, value) == 0)
            return float_consts[i].label;
    }
    if (float_const_count >= MAX_FLOAT_CONSTS) {
        fprintf(stderr, "ERROR - float constant table full\n");
        return ".FC_ERR";
    }

    char buf[32];
    snprintf(buf, sizeof(buf), ".FC%d", float_const_count);
    float_consts[float_const_count].label = strdup(buf);
    float_consts[float_const_count].value = strdup(value);
    float_const_count++;
    
    return float_consts[float_const_count - 1].label;
}

int new_label() {
    return label_count++;
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
    node->kind = NODE_STATEMENTS;

    return node;
}

/* Only one argument is allowed */
ASTNode *make_argument_expression_list(ASTNode *function, ASTNode *args) {
    ASTNode *node = create_node();

    node->left = function;
    node->right = args;
    node->kind = NODE_FUNCTION;

    return node;
}

ASTNode *make_identifier(const char *name, SymbolStatement *ss) {
    if (ss == NULL) {
        return NULL;
    }

    ASTNode *node = create_node();
    node->name = strdup(name);
    node->kind = NODE_IDENTIFIER;
    node->offset = ss->offset;
    node->type = ss->type;

    return node;
}

ASTNode *make_constant(const char *name) {
    ASTNode *node = create_node();
    node->name = strdup(name);
    node->kind = NODE_CONSTANT;
    
    if (strchr(name, '.') || strchr(name, 'e') || strchr(name, 'E')) {
        node->type = FLOAT;
        node->float_label = strdup(intern_float_const(name));
    } else {
        node->type = INT;
    }

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
    node->kind = NODE_OPERAND;
    node->type = node_left->type;

    return node;
}

ASTNode *make_binop(NodeData nodedata, ASTNode *node_left, ASTNode *node_right) {
    if (node_left == NULL || node_right == NULL) {
        printf("ERROR - NULL node during binop creation\n");
        return NULL;
    }

    ASTNode *node = create_node();
    node->left = node_left;
    node->right = node_right;
    node->kind = NODE_OPERAND;
    node->op = nodedata;
    node->type = (IS_FLOAT(node_left->type) || IS_FLOAT(node_right->type))
                ? FLOAT : node_left->type;

    return node;
}

ASTNode *make_conditional(NodeKind nodekind, ASTNode *node_left, ASTNode *node_right) {
    if (node_left == NULL || node_right == NULL) {
        printf("ERROR - NULL node during binop creation\n");
        return NULL;
    }

    ASTNode *node = create_node();
    node->left = node_left;
    node->right = node_right;
    node->kind = nodekind;

    return node;
}

ASTNode *make_while_cond(ASTNode *expr) {
    if (expr == NULL) {
        printf("ERROR - NULL node during do-whle node creation\n");
        return NULL;
    }

    ASTNode *node = create_node();
    node->left = expr;
    node->kind = NODE_WHILE_COND;

    return node;
}

void generate_code(ASTNode *root) {
    if (root == NULL) return;
    FILE *f = fopen(RISC_FILENAME, "w");

    if (float_const_count > 0) {
        fprintf(f, ".data\n");
        for (int i = 0; i < float_const_count; i++) {
            fprintf(f, "%s: .float %s\n", float_consts[i].label, float_consts[i].value);
        }
        fprintf(f, "\n");
    }

    fprintf(f, ".text\n");
    fprintf(f, ".globl main\n\n");

    fprintf(f, "main:\n");
    generate_symbol_code(f);
    generate_node_code(f, root);

    fprintf(f, "\n    li a0, 0\n");
    fprintf(f, "\n    li a7, 93\n");
    fprintf(f, "    ecall\n");
    fclose(f);
}

void generate_symbol_code(FILE *f) {
    // 16-bit alignment
    int times = (symbol_count + 3) / 4;
    int bytes = times * 16 + TEMP_REG_ALLOC;
    fprintf(f, "    addi sp, sp, -%d\n", bytes);
    fprintf(f, "    mv fp, sp\n\n");
}

void promote_if_needed(FILE *f, int left_type, int right_type) {
    if (!IS_FLOAT(left_type) && IS_FLOAT(right_type)) {
        fprintf(f, "    fcvt.s.w ft1, t1\n");
    }
    if (IS_FLOAT(left_type) && !IS_FLOAT(right_type)) {
        fprintf(f, "    fcvt.s.w ft0, t0\n");
    }
}

void generate_node_code(FILE *f, ASTNode *node) {
    if (!node) return;
    if (node->kind == NODE_STATEMENTS) {
        generate_node_code(f, node->left);
        generate_node_code(f, node->right);
        return;
    }

    /****************/
    /*  Leaf nodes  */
    /****************/

    if (node->kind == NODE_CONSTANT) {
        if (IS_FLOAT(node->type)) {
            const char *label = intern_float_const(node->name);
            fprintf(f, "    lui t0, %%hi(%s)\n", label);
            fprintf(f, "    flw ft0, %%lo(%s)(t0)\n", label);
        } else {
            fprintf(f, "    li t0, %s\n", node->name);
        }
        return;
    }

    if (node->kind == NODE_IDENTIFIER) {
        if (IS_FLOAT(node->type)) {
            fprintf(f, "    flw ft0, %d(fp)\n", node->offset);
        } else {
            fprintf(f, "    lw t0, %d(fp)\n", node->offset);
        }
        return;
    }
    
    /******************/
    /*  Parent nodes  */
    /******************/

    /* Supports only floats and integers; assumes printf with only one argument */
    if (node->kind == NODE_FUNCTION) {
        generate_node_code(f, node->right);

        if (!IS_FLOAT(node->right->type)) {
            fprintf(f, "    li a7, 1\n");
            fprintf(f, "    mv a0, t0\n");
        } else {
            fprintf(f, "    li a7, 2\n");
            fprintf(f, "    fmv.s fa0, ft0\n");
        }

        fprintf(f, "    ecall\n");
        fprintf(f, "    li a0, 10\n");
        fprintf(f, "    li a7, 11\n");
        fprintf(f, "    ecall\n");
        return;
    }

    // Conditions
    if (node->kind == NODE_IF) {
        if (node->right && node->right->kind == NODE_ELSE) {
            int else_label = new_label();
            int end_label = new_label();

            generate_node_code(f, node->left);
            fprintf(f, "    beqz t0, .L%d\n", else_label);

            generate_node_code(f, node->right->left);
            fprintf(f, "    j .L%d\n", end_label);

            fprintf(f, ".L%d:\n", else_label);
            generate_node_code(f, node->right->right);

            fprintf(f, ".L%d:\n", end_label);
        } else {
            int end_label = new_label();

            generate_node_code(f, node->left);
            fprintf(f, "    beqz t0, .L%d\n", end_label);
            
            generate_node_code(f, node->right);
            
            fprintf(f, ".L%d:\n", end_label);
        }
        return;
    }
        
    if (node->kind == NODE_WHILE) {
        int start_label = new_label();
        int end_label = new_label();

        fprintf(f, ".L%d:\n", start_label);
        
        generate_node_code(f, node->left);
        fprintf(f, "    beqz t0, .L%d\n", end_label);
        
        generate_node_code(f, node->right);
        fprintf(f, "    j .L%d\n", start_label);
        
        fprintf(f, ".L%d:\n", end_label);
        return;
    }

    if (node->kind == NODE_WHILE_COND) {
        generate_node_code(f, node->left);
        return;
    }
    
    if (node->kind == NODE_DO) {
        int start_label = new_label();
        int end_label = new_label();

        fprintf(f, ".L%d:\n", start_label);

        generate_node_code(f, node->left);
        generate_node_code(f, node->right);

        fprintf(f, "    bnez t0, .L%d\n", start_label);
        fprintf(f, ".L%d:\n", end_label);
        
        return;
    }

    if (node->kind == NODE_FOR) {
        ASTNode *parts = node->left;
        ASTNode *body = node->right;
        ASTNode *init, *cond, *incr;

        if (parts->left && parts->left->kind == NODE_STATEMENTS) {
            init = parts->left->left;
            cond = parts->left->right;
            incr = parts->right;
        } else {
            init = parts->left;
            cond = parts->right;
            incr = NULL;
        }

        int start_label = new_label();
        int end_label = new_label();

        generate_node_code(f, init);

        fprintf(f, ".L%d:\n", start_label);
        generate_node_code(f, cond);
        fprintf(f, "    beqz t0, .L%d\n", end_label);

        generate_node_code(f, body);
        if (incr) generate_node_code(f, incr);
        fprintf(f, "    j .L%d\n", start_label);

        fprintf(f, ".L%d:\n", end_label);
        return;
    }

    if (node->op == NODE_EQ) {
        generate_node_code(f, node->right);
        if (IS_FLOAT(node->type)) {
            if (!IS_FLOAT(node->right->type)) {
                fprintf(f, "    fcvt.s.w ft0, t0\n");
            }
            fprintf(f, "    fsw ft0, %d(fp)\n", node->left->offset);
        } else {
            fprintf(f, "    sw t0, %d(fp)\n", node->left->offset);
        }
        return;
    }

    // Expressions
    /*
    * Temporary values stored by nested arithmetics use
    * fp-relative memory addresses. These addresses
    * use the expr_depth variable which tracks how deeply
    * nested a sub-expression is.
    */
   if (expr_depth >= MAX_EXPR_DEPTH) {
    fprintf(stderr, "ERROR - expression stack overflow (depth %d)\n", expr_depth);
    return;
    }
    
    int is_float_expr = IS_FLOAT(node->type);
    int mem = (expr_depth + 1) * 4;

    generate_node_code(f, node->left);
    if (IS_FLOAT(node->left->type)) {
        fprintf(f, "    fsw ft0, %d(fp)\n", mem);
    } else {
        fprintf(f, "    sw t0, %d(fp)\n", mem);
    }
    expr_depth++;

    generate_node_code(f, node->right);
    if (IS_FLOAT(node->left->type)) {
        fprintf(f, "    flw ft1, %d(fp)\n", mem);
    } else {
        fprintf(f, "    lw t1, %d(fp)\n", mem);
    }
    expr_depth--;

    promote_if_needed(f, node->left->type, node->right->type);

    if (is_float_expr) {
        switch (node->op) {
        case NODE_ADD:  fprintf(f, "    fadd.s ft0, ft1, ft0\n"); break;
        case NODE_SUB:  fprintf(f, "    fsub.s ft0, ft1, ft0\n"); break;
        case NODE_MUL:  fprintf(f, "    fmul.s ft0, ft1, ft0\n"); break;
        case NODE_DIV:  fprintf(f, "    fdiv.s ft0, ft1, ft0\n"); break;
        case NODE_LE:   fprintf(f, "    flt.s t0, ft1, ft0\n"); break;
        case NODE_GE:   fprintf(f, "    flt.s t0, ft0, ft1\n"); break;
        case NODE_LEQ:  fprintf(f, "    fle.s t0, ft1, ft0\n"); break;
        case NODE_GEQ:  fprintf(f, "    fle.s t0, ft0, ft1\n"); break;
        case NODE_ISEQ: fprintf(f, "    feq.s t0, ft1, ft0\n"); break;
        case NODE_NEQ:
            fprintf(f, "    feq.s t0, ft1, ft0\n");
            fprintf(f, "    xori t0, t0, 1\n");
            break;
        default: break;
        }
    } else {
        switch (node->op) {
        case NODE_ADD:  fprintf(f, "    add t0, t1, t0\n"); break;
        case NODE_SUB:  fprintf(f, "    sub t0, t1, t0\n"); break;
        case NODE_MUL:  fprintf(f, "    mul t0, t1, t0\n"); break;
        case NODE_DIV:  fprintf(f, "    div t0, t1, t0\n"); break;
        case NODE_MOD:  fprintf(f, "    rem t0, t1, t0\n"); break;
        case NODE_LE:   fprintf(f, "    slt t0, t1, t0\n"); break;
        case NODE_GE:   fprintf(f, "    slt t0, t0, t1\n"); break;
        case NODE_ISEQ:
            fprintf(f, "    sub t0, t1, t0\n");
            fprintf(f, "    seqz t0, t0\n");
            break;
        case NODE_NEQ:
            fprintf(f, "    sub t0, t1, t0\n");
            fprintf(f, "    snez t0, t0\n");
            break;
        case NODE_BAND: fprintf(f, "    and t0, t1, t0\n"); break;
        case NODE_BOR:  fprintf(f, "    or t0, t1, t0\n"); break;
        case NODE_XOR:  fprintf(f, "    xor t0, t1, t0\n"); break;
        case NODE_BL:   fprintf(f, "    sll t0, t1, t0\n"); break;
        case NODE_BR:   fprintf(f, "    srl t0, t1, t0\n"); break;
        case NODE_AND:
            fprintf(f, "    snez t0, t1, t1\n");
            fprintf(f, "    snez t0, t0, t0\n");
            fprintf(f, "    and t0, t1, t0\n");
            break;
        case NODE_OR:
            fprintf(f, "    snez t0, t1, t1\n");
            fprintf(f, "    snez t0, t0, t0\n");
            fprintf(f, "    or t0, t1, t0\n");
            break;
        case NODE_LEQ:
            fprintf(f, "    slt t0, t0, t1\n");
            fprintf(f, "    xori t0, t0, 1\n");
            break;
        case NODE_GEQ:
            fprintf(f, "    slt t0, t1, t0\n");
            fprintf(f, "    xori t0, t0, 1\n");
            break;
        default: break;
        }
    }
}
