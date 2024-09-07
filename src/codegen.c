
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "codegen.h"
#include "symbol.h"

// ========================
// Scratch register control
// ========================

static uint8_t registers = 0;

int scratch_alloc()
{
    uint8_t r = 1;

    for (int i = 0; i <= 6; i++)
    {
        if ((registers & r) == 0)
        {
            registers |= r;
            return i;
        }

        r <<= 1;
    }

    printf("ERROR: Register allocation requested but all registers are in use.\n");
    exit(1);
}

void scratch_free(int r)
{
    if (r < 0 || r > 6)
    {
        printf("WARNING: Attempted to free an out-of-range register\n");
        return;
    }

    uint8_t reg = 1;

    reg <<= r;

    if ((reg & registers) == 0)
        printf("WARNING: Attempted to free already freed register '%s'\n", scratch_name(r));

    registers -= (reg & registers);
}

const char *scratch_name(int r)
{
    switch (r)
    {
    case 0:
        return "%rbx";
    case 1:
        return "%r10";
    case 2:
        return "%r11";
    case 3:
        return "%r12";
    case 4:
        return "%r13";
    case 5:
        return "%r14";
    case 6:
        return "%r15";
    default:
        printf("ERROR: Name request for invalid register, %d, requested\n", r);
        exit(1);
        break;
    }
}

// ===================
// Labelling functions
// ===================

static int label_counter = 0;

int label_create()
{
    return label_counter++;
}

const char *label_name(int label)
{
    // Neat trick to find out how large your buffer should be. snprintf will
    // return the number of characters that would be written to a buffer if its
    // writing to null
    int buffer_size = snprintf(NULL, 0, ".L%d", label);
    char *str = malloc(sizeof(char) * (buffer_size + 1));
    sprintf(str, ".L%d", label);
    return str;
}

// ===============
// Code generation
// ===============

// Used to offset local variables from parameter argument variables on the stack
// This gets reset to 0 once
static int stack_position = 0;

const char *symbol_codegen(struct symbol *s)
{
    if (s->kind == SYMBOL_GLOBAL)
        return s->name;

    // Push the stack position down
    stack_position += -8;

    // Print out the label needed
    int buffer_size = snprintf(NULL, 0, "%d(%%rbp)", stack_position);
    char *str = malloc(sizeof(char) * (buffer_size + 1));
    sprintf(str, "%d(%%rbp)", stack_position);

    return str;
}

void expr_codegen(struct expr *e)
{
    if (!e)
        return;

    switch (e->kind)
    {
        // ==========
        // Leaf nodes
        // ==========
    case EXPR_NAME:
        e->reg = scratch_alloc();
        printf("mov %s, %s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
        break;
    case EXPR_CHARLITERAL:
    case EXPR_INTEGERLITERAL:
    case EXPR_BOOLEANLITERAL:
        e->reg = scratch_alloc();
        printf("mov %d, %s\n", e->literal_value, scratch_name(e->reg));
        break;
    case EXPR_STRINGLITERAL:

        break;

        // ==============
        // Interior nodes
        // ==============
    case EXPR_GROUP:

        break;
    case EXPR_ARG:

        break;
    case EXPR_INITIALIZER:

        break;
    case EXPR_SUBSCRIPT:

        break;
    case EXPR_CALL:

        break;
    case EXPR_INC:

        break;
    case EXPR_DEC:

        break;
    case EXPR_NEGATE:

        break;
    case EXPR_NOT:

        break;
    case EXPR_POW:
        expr_codegen(e->left);
        expr_codegen(e->right);
        // TODO: Add a loop unrolling optimization here
        int result = scratch_alloc();
        printf("\tmov\t0x1\t%s\n", scratch_name(result));

        printf("\tdec\t%s\n", scratch_name(e->right->reg));

        break;
    case EXPR_MUL:
        expr_codegen(e->left);
        expr_codegen(e->right);
        printf("\timul\t%s\t%s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
        e->reg = e->right->reg;
        scratch_free(e->left->reg);
        break;
    case EXPR_DIV:
        expr_codegen(e->left);
        expr_codegen(e->right);

        printf("\tmov\t0x0\t%%rdx\n");                               // Set %rdx to 0
        printf("\tmov\t%s\t%%rax\n", scratch_name(e->left->reg)); // Set %rax to divisor
        printf("\tidiv\t%s\n", scratch_name(e->right->reg));      // Perform the division

        scratch_free(e->left->reg);
        scratch_free(e->right->reg);

        e->reg = scratch_alloc();

        printf("\tmov\t%%rax\t%s\n", scratch_name(e->reg)); // Move quotient from %rax to e's register

        break;
    case EXPR_MOD:
        expr_codegen(e->left);
        expr_codegen(e->right);

        printf("\tmov\t0x0\t%%rdx\n");                               // Set %rdx to 0
        printf("\tmov\t%s\t%%rax\n", scratch_name(e->left->reg)); // Set %rax to divisor
        printf("\tidiv\t%s\n", scratch_name(e->right->reg));      // Perform the division

        scratch_free(e->left->reg);
        scratch_free(e->right->reg);

        e->reg = scratch_alloc();

        printf("\tmov\t%%rdx\t%s\n", scratch_name(e->reg)); // Move remainder from %rax to e's register

        break;
    case EXPR_ADD:
        expr_codegen(e->left);
        expr_codegen(e->right);
        printf("add %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
        e->reg = e->right->reg;
        scratch_free(e->left->reg);
        break;
    case EXPR_SUB:
        expr_codegen(e->left);
        expr_codegen(e->right);
        printf("sub %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
        e->reg = e->right->reg;
        scratch_free(e->left->reg);
        break;
    case EXPR_LT:

        break;
    case EXPR_LTE:

        break;
    case EXPR_GT:

        break;
    case EXPR_GTE:

        break;
    case EXPR_EQUALITY:

        break;
    case EXPR_NEQUALITY:

        break;
    case EXPR_AND:

        break;
    case EXPR_OR:

        break;
    case EXPR_ASSIGNMENT:
        expr_codegen(e->left);
        printf("mov %s, %s\n", scratch_name(e->left->reg), symbol_codegen(e->right->symbol));
        e->reg = e->left->reg;
        break;
    }
}
