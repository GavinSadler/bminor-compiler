
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

static int arg_number = 0;

static void print_asm(const char* command, const char* operand_1, const char* operand_2, const char* operand_3)
{
    printf("\t%s", command);

    if(operand_1)
        printf("\t%s", operand_1);
    
    if(operand_3)
        printf("\t%s", operand_1);
    
    if(operand_2)
        printf("\t%s", operand_1);

    printf("\n");
}

void expr_codegen(struct expr *e)
{
    if (!e)
        return;

    // Generate code for the left and right expressions first. EXPR_ARG and
    // EXPR_CALL have special code generation cases
    if (e->kind != EXPR_ARG && e->kind != EXPR_CALL)
    {
        expr_codegen(e->left);
        expr_codegen(e->right);
    }

    // Define some useful variables for registers and their names
    int lr = -1;
    int rr = -1;
    const char *lrs = "";
    const char *rrs = "";

    if (e->left)
    {
        lr = e->left->reg;
        lrs = scratch_name(e->left->reg);
    }

    if (e->right)
    {
        lr = e->right->reg;
        lrs = scratch_name(e->right->reg);
    }

    switch (e->kind)
    {
        // ==========
        // Leaf nodes
        // ==========
    case EXPR_NAME:
        e->reg = scratch_alloc();
        print_asm("mov", symbol_codegen(e->symbol), scratch_name(e->reg), 0);
        break;
    case EXPR_CHARLITERAL:
    case EXPR_INTEGERLITERAL:
    case EXPR_BOOLEANLITERAL:
        e->reg = scratch_alloc();
        
        // Neat trick to find out how large your buffer should be. snprintf will
        // return the number of characters that would be written to a buffer if its
        // writing to null
        int buffer_size = snprintf(NULL, 0, "%d", e->literal_value);
        char *str = malloc(sizeof(char) * (buffer_size + 1));
        sprintf(str, "%d", e->literal_value);

        print_asm("mov", str, scratch_name(e->reg), 0);
        break;
    case EXPR_STRINGLITERAL:
        // TODO:
        break;

        // ==============
        // Interior nodes
        // ==============
    case EXPR_GROUP:
        e->reg = lr;
        break;
    case EXPR_ARG:
        // Args need to push their value before propogating code generation to
        // the next arg
        expr_codegen(e->left);

        char *reg;

        switch (arg_number)
        {
        case 0:
            reg = "%rdi";
            break;
        case 1:
            reg = "%rsi";
            break;
        case 2:
            reg = "%rdx";
            break;
        case 3:
            reg = "%rcx";
            break;
        case 4:
            reg = "%r8";
            break;
        case 5:
            reg = "%r9";
            break;
        default:
            printf("ERROR: Too many arguments in function call\n");
            exit(1);
        }

        arg_number++;

        print_asm("mov", scratch_name(e->left->reg), reg, 0);
        scratch_free(e->left->reg);
        expr_codegen(e->right);
        break;
    case EXPR_INITIALIZER:
        // TODO:
        break;
    case EXPR_SUBSCRIPT:
        print_asm("imul", "0x8", rrs, 0); // Right now we only care about integers, so we multiply by 8 for 64 bit integers
        print_asm("add", lrs, rrs, 0); // Then add to the base address
        e->reg = lr;
        scratch_free(rr);
        break;
    case EXPR_CALL:

        // First push caller-save registers
        print_asm("push", "%rax", 0, 0);
        print_asm("push", "%rcx", 0, 0);
        print_asm("push", "%rdx", 0, 0);
        print_asm("push", "%rsi", 0, 0);
        print_asm("push", "%rdi", 0, 0);
        print_asm("push", "%r8", 0, 0);
        print_asm("push", "%r9", 0, 0);
        print_asm("push", "%r10", 0, 0);
        print_asm("push", "%r11", 0, 0);

        // Now generate code for the args, since this will push to the stack
        arg_number = 0;
        expr_codegen(e->right);

        print_asm("call", lrs);

        // Then pop caller save registers from the stack
        print_asm("pop", "%r11", 0, 0);
        print_asm("pop", "%r10", 0, 0);
        print_asm("pop", "%r9", 0, 0);
        print_asm("pop", "%r8", 0, 0);
        print_asm("pop", "%rdi", 0, 0);
        print_asm("pop", "%rsi", 0, 0);
        print_asm("pop", "%rdx", 0, 0);
        print_asm("pop", "%rcx", 0, 0);
        print_asm("pop", "%rax", 0, 0);

        break;
    case EXPR_ASSIGNMENT:
        print_asm("mov", lrs, symbol_codegen(e->right->symbol), 0);
        e->reg = lr;
        break;

        // ==========
        // Arithmatic
        // ==========
    case EXPR_NEGATE:
        print_asm("neg", lrs, 0, 0);
        e->reg = lr;
        break;
    case EXPR_INC:
        print_asm("inc", lrs, 0, 0);
        e->reg = lr;
        break;
    case EXPR_DEC:
        print_asm("dec", lrs);
        e->reg = lr;
        break;
    case EXPR_POW:
        // TODO: Finish this portion!
        expr_codegen(e->left);
        expr_codegen(e->right);
        int result = scratch_alloc();
        printf("\tmov\t0x1\t%s\n", scratch_name(result));

        printf("\tdec\t%s\n", rrs);

        e->reg = lr;
        scratch_free(rr);
        break;
    case EXPR_MUL:
        print_asm("imul", lrs, rrs, 0);
        e->reg = lr;
        scratch_free(rr);
        break;
    case EXPR_DIV:
        // TODO:
        printf("\tmov\t0x0\t%%rdx\n");     // Set %rdx to 0
        printf("\tmov\t%s\t%%rax\n", lrs); // Set %rax to divisor
        printf("\tidiv\t%s\n", rrs);       // Perform the division

        scratch_free(lr);
        scratch_free(rr);

        e->reg = scratch_alloc();

        printf("\tmov\t%%rax\t%s\n", scratch_name(e->reg)); // Move quotient from %rax to e's register

        break;
    case EXPR_MOD:
        // TODO:
        printf("\tmov\t0x0\t%%rdx\n");     // Set %rdx to 0
        printf("\tmov\t%s\t%%rax\n", lrs); // Set %rax to divisor
        printf("\tidiv\t%s\n", rrs);       // Perform the division

        scratch_free(lr);
        scratch_free(rr);

        e->reg = scratch_alloc();

        printf("\tmov\t%%rdx\t%s\n", scratch_name(e->reg)); // Move remainder from %rax to e's register

        break;
    case EXPR_ADD:
        print_asm("add", lrs, rrs, 0);
        e->reg = lr;
        scratch_free(rr);
        break;
    case EXPR_SUB:
        print_asm("sub", lrs, rrs, 0);
        e->reg = lr;
        scratch_free(rr);
        break;

        // =================
        // Logical Operators
        // =================
    case EXPR_NOT:
        // Set zero flag by & left register against itself
        print_asm("not", lrs, 0, 0);
        e->reg = lr;
        break;
    case EXPR_LT:
        print_asm("cmp", rrs, lrs, 0);
        print_asm("setl", lrs, 0, 0);
        e->reg = lr;
        scratch_free(rr);
    case EXPR_LTE:
        print_asm("cmp", rrs, lrs, 0);
        print_asm("setle", lrs, 0, 0);
        e->reg = lr;
        scratch_free(rr);
        break;
    case EXPR_GT:
        print_asm("cmp", rrs, lrs, 0);
        print_asm("setg", lrs, 0, 0);
        e->reg = lr;
        scratch_free(rr);
        break;
    case EXPR_GTE:
        print_asm("cmp", rrs, lrs, 0);
        print_asm("setge", lrs, 0, 0);
        e->reg = lr;
        scratch_free(rr);
        break;
    case EXPR_EQUALITY:
        print_asm("cmp", rrs, lrs, 0);
        print_asm("sete", lrs, 0, 0);
        e->reg = lr;
        scratch_free(rr);
        break;
    case EXPR_NEQUALITY:
        print_asm("cmp", rrs, lrs, 0);
        print_asm("setne", lrs, 0, 0);
        e->reg = lr;
        scratch_free(rr);
        break;
    case EXPR_AND:
        print_asm("and", rrs, lrs, 0);
        print_asm("setnz", lrs, 0, 0);
        e->reg = lr;
        scratch_free(rr);
        break;
    case EXPR_OR:
        print_asm("or", rrs, lrs, 0);
        print_asm("setnz", lrs, 0, 0);
        e->reg = lr;
        scratch_free(rr);
        break;
    }
}

void stmt_codegen(struct stmt *s)
{
    if (!s)
        return;
    switch (s->kind)
    {
    case STMT_DECL:
        decl_codegen(s->decl);
        break;
    case STMT_EXPR:
        expr_codegen(s->expr);
        scratch_free(s->expr->reg);
        break;
    case STMT_IF:
        // TODO:
        break;
    case STMT_FOR:
        // TODO:
        break;
    case STMT_PRINT:
        // TODO:
        break;
    case STMT_RETURN:
    //TODO:
        expr_codegen(s->expr);
        printf("MOV %s, %%rax\n", scratch_name(s->expr->reg));
        printf("JMP .%s_epilogue\n", function_name);
        scratch_free(s->expr->reg);
        break;
    case STMT_BLOCKSTART:
        // TODO:
        break;
    case STMT_BLOCKEND:
        // TODO:
        break;
    }
    stmt_codegen(s->next);
}
