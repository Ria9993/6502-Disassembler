#ifndef INSTRUCTION_6502_H
#define INSTRUCTION_6502_H

/** Global Address Mode **/
typedef enum {
    AM_A = 0,
    AM_IMM,
    AM_ABS,
    AM_ZP,
    AM_IMP,
    AM_REL,
    AM__ABS_, //< (a)
    AM_A_X,
    AM_A_Y,
    AM_ZP_X,
    AM_ZP_Y,
    AM__ZP_X_, //< (zp,x)
    AM__ZP__Y //< (zp),y
} am_global_t;
const char AM_STR[][7] = { "A", "#", "a", "zp", "i", "r", "(a)", "a,x", "a,y", "zp,x", "zp,y", "(zp,x)", "(zp),y" };
const unsigned char AM_OPERAND_BYTE[] = { 0, 1, 2, 1, 0, 1, 2, 2, 2, 1, 1, 1, 1 };

/** Grouped Instruction **/
/* Group 1 (cc == 01) */
typedef enum {
    OP_ORA = 0b000,
    OP_AND,
    OP_EOR,
    OP_ADC,
    OP_STA,
    OP_LDA,
    OP_CMD,
    OP_SBC
} g1_opcode_t;
const char G1_OP_STR[][4] = { "ora", "and", "eor", "adc", "sta", "lda", "cmp", "sbc" };

typedef enum {
    G1_AM__ZP_X_ = 0b000, //< (zp, X)
    G1_AM_ZP,
    G1_AM_IMM,
    G1_AM_ABS,
    G1_AM__ZP_Y, //< (zp), Y
    G1_AM_ZP_X,
    G1_AM_ABS_Y,
    G1_AM_ABS_X
} g1_am_t;
const char G1_AM_STR[][7] = { "(zp,x)", "zp", "#", "a", "(zp),y", "zp,x", "a,y", "a,x" };
const unsigned char G1_AM_OPERAND_BYTE[] = { 1, 1, 1, 2, 1, 1, 2, 2 };

/* Group 2 (cc == 10) */
typedef enum {
    OP_ASL = 0b000,
    OP_ROL,
    OP_LSR,
    OP_ROR,
    OP_STX,
    OP_LDX,
    OP_DEC,
    OP_INC
} g2_opcode_t;
const char G2_OP_STR[][4] = { "asl", "rol", "lsr", "ror", "stx", "ldx", "dec", "inc" };

typedef enum {
    G2_AM_IMM = 0b000,
    G2_AM_ZP,
    G2_AM_A,
    G2_AM_ABS,
    G2_AM_NONE1,
    G2_AM_ZP_X,
    G2_AM_NONE2,
    G2_AM_A_X
} g2_am_t;
const char G2_AM_STR[][5] = { "#", "zp", "A", "a", "", "zp,x", "", "a,x" };
const unsigned char G2_AM_OPERAND_BYTE[] = { 1, 1, 0, 2, 0, 1, 0, 2 };

/* Group 3 (cc == 00) */
typedef enum {
    OP_BIT = 0b001,
    OP_JMP,
    OP_JMP_INDIRECT, //< JMP (a)
    OP_STY,
    OP_LDY,
    OP_CPY,
    OP_CPX
} g3_opcode_t;
const char G3_OP_STR[][4] = { "", "bit", "jmp", "jmp", "sty", "ldy", "cpy", "cpx" };

typedef enum {
    G3_AM_IMM = 0b000,
    G3_AM_ZP,
    G3_AM_NONE1,
    G3_AM_ABS,
    G3_AM_NONE2,
    G3_AM_ZP_X,
    G3_AM_NONE3,
    G3_AM_ABS_X
} g3_am_t;
const char G3_AM_STR[][5] = { "#", "zp", "", "a", "", "zp,x", "", "a,x" };
const unsigned char G3_AM_OPERAND_BYTE[] = { 1, 1, 0, 2, 0, 1, 0, 2 };

/** Non-grouped Instructions **/
typedef struct {
    char op_str[4];
    unsigned char binary;
} opcode_t;

/* Conditional Branch Instruction */
/* Format : [xxy10000]
    xx : Determine flag
        00 : negative
        01 : overflow
        10 : carry
        11 : zero
    y : Operand to compare */
const opcode_t COND_BRANCH[] = {
    { "bpl", 0x10 },
    { "bmi", 0x30 },
    { "bvc", 0x50 },
    { "bvs", 0x70 },
    { "bcc", 0x90 },
    { "bcs", 0xB0 },
    { "bne", 0xD0 },
    { "beq", 0xF0 }
};

/* Interrupt and Subroutine Instruction */
const opcode_t INT_SUB[] = {
    { "brk", 0x00 },
    { "rti", 0x40 },
    { "rts", 0x60 }
    // { "jsr", 3, 0x20 }, //< abs mode
};
const opcode_t JSR = { "jsr", 0x20 };

/* Single Instruction*/
const opcode_t SINGLE[] = {
    { "php", 0x08 },
    { "plp", 0x28 },
    { "pha", 0x48 },
    { "pla", 0x68 },
    { "dey", 0x88 },
    { "tay", 0xA8 },
    { "iny", 0xC8 },
    { "inx", 0xE8 },
    { "clc", 0x18 },
    { "sec", 0x38 },
    { "cli", 0x58 },
    { "sei", 0x78 },
    { "tya", 0x98 },
    { "clv", 0xB8 },
    { "cld", 0xD8 },
    { "sed", 0xF8 },
    { "txa", 0x8A },
    { "txs", 0x9A },
    { "tax", 0xAA },
    { "tsx", 0xBA },
    { "dex", 0xCA },
    { "nop", 0xEA }
};

#endif /* INSTRUCTION_6502_H */
