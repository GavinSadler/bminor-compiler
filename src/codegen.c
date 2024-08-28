
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "codegen.h"
#include "symbol.h"

// ========================
// Scratch register control
// ========================

uint8_t registers = 0;

int scratch_alloc()
{
    uint8_t r = 1;

    for (int i = 0; i <= 6; i++)
    {
        if ((registers & r) == 0)
        {
            registers &= r;
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

    for (int i = 0; i <= r; i++)
        reg <<= 1;

    if ((reg & registers) > 0)
        printf("WARNING: Attempted to free already freed register '%s'", scratch_name(r));

    registers -= reg & registers;
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

int label_counter = -1;

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

const char *symbol_codegen(struct symbol *s)
{
    s->type;
}