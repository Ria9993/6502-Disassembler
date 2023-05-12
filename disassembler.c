#include "6502Instruction.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void make_outstring(char* const out_buffer64, const unsigned char* const mem, const char* const op_str, const char* const am_str, const unsigned char operand_byte);

/* @Return
        Pointer of next instruction
*/
const unsigned char* disassemble(char* out_buffer64, const unsigned char* mem)
{
    /* 6502 instruction format
        [aaabbbcc]
        aaa = opcode
        bbb = addressing mode
        cc = group-code. Determine opcode group
    */
    const unsigned char inst = *mem;
    const unsigned char opcode = (inst >> 5);
    const unsigned char addr_mode = (inst >> 2) & 0b111;
    const unsigned char group_code = inst & 0b11;

    const unsigned char* next_inst = mem;

    // Conditional branch instruction
    for (int i = 0; i < 8; i++) {
        if (inst == COND_BRANCH[i].binary) {
            make_outstring(out_buffer64, mem, COND_BRANCH[i].op_str, "r", 1);
            return mem + 2;
        }
    }

    // Interrupt and Subroutine Instruction
    for (int i = 0; i < 3; i++) {
        if (inst == INT_SUB[i].binary) {
            make_outstring(out_buffer64, mem, INT_SUB[i].op_str, "i", 0);
            return mem + 1;
        }
    }
    if (inst == JSR.binary) { // JSR << abs mode
        make_outstring(out_buffer64, mem, JSR.op_str, "a", 2);
        return mem + 3;
    }

    // Single Instruction
    for (int i = 0; i < 22; i++) {
        if (inst == SINGLE[i].binary) {
            make_outstring(out_buffer64, mem, SINGLE[i].op_str, "i", 0);
            return mem + 1;
        }
    }

    // Grouped Instruction
    switch (group_code) {
    // Group 1
    case 0b01:
        make_outstring(out_buffer64, mem, G1_OP_STR[opcode], G1_AM_STR[addr_mode], G1_AM_OPERAND_BYTE[addr_mode]);
        next_inst += 1 + G1_AM_OPERAND_BYTE[addr_mode];
        break;
    // Group 2
    case 0b10:
        // Handle [STX zp,x], [LDX zp,x], [LDX a,x]
        // [STX zp,x] [LDX zp,x] is should be changed to [zp,y]
        if ((opcode == OP_STX || opcode == OP_LDX) && addr_mode == G2_AM_ZP_X) {
            make_outstring(out_buffer64, mem, G2_OP_STR[opcode], AM_STR[AM_ZP_Y], G2_AM_OPERAND_BYTE[addr_mode]);
            next_inst += 1 + G2_AM_OPERAND_BYTE[addr_mode];
            break;
        }
        // [LDX a,x] is should be changed to [LDX a,y]
        if (opcode == OP_LDX && addr_mode == G2_AM_A_X) {
            make_outstring(out_buffer64, mem, G2_OP_STR[opcode], AM_STR[AM_A_Y], G2_AM_OPERAND_BYTE[addr_mode]);
            next_inst += 1 + G2_AM_OPERAND_BYTE[addr_mode];
            break;
        }
        make_outstring(out_buffer64, mem, G2_OP_STR[opcode], G2_AM_STR[addr_mode], G2_AM_OPERAND_BYTE[addr_mode]);
        next_inst += 1 + G2_AM_OPERAND_BYTE[addr_mode];
        break;
    // Group 3
    case 0b00:
        // Handle [JMP (a)]
        if (opcode == OP_JMP_INDIRECT) {
            make_outstring(out_buffer64, mem, G3_OP_STR[opcode], "(a)", 2);
            next_inst += 3;
            break;
        }
        make_outstring(out_buffer64, mem, G3_OP_STR[opcode], G3_AM_STR[addr_mode], G3_AM_OPERAND_BYTE[addr_mode]);
        next_inst += 1 + G3_AM_OPERAND_BYTE[addr_mode];
        break;
    // Group 4 (not used in 6502. 65C02 only)
    case 0b11:
        break;
    default:
        assert(0);
    }

    return next_inst;
}

/* @Format
    OPCODE=<옵코드>[<니모닉> <주소지정 모드 표기>] OPERAND=<상위 바이트> <하위 바이트>

    -Exampele-
    A9 FF		"OPCODE=A9[lda #] OPERAND=.. FF"
    AD 10 20	"OPCODE=AD[lda a] OPERAND=20 10"
    0A			"OPCODE=0A[asl A] OPERAND=.. .." */
static void make_outstring(char* const out_buffer64, const unsigned char* const mem, const char* const op_str, const char* const am_str, const unsigned char operand_byte)
{
    sprintf(out_buffer64, "OPCODE=%02X[%s %s] OPERAND=", *mem, op_str, am_str);
    char* str_next = out_buffer64 + strlen(out_buffer64);

    if (operand_byte >= 2) {
        sprintf(str_next, "%02X ", *(mem + 2));
    } else {
        sprintf(str_next, ".. ");
    }
    if (operand_byte >= 1) {
        sprintf(str_next + 3, "%02X", *(mem + 1));
    } else {
        sprintf(str_next + 3, "..");
    }
}
