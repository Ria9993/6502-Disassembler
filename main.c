#include "disassembler.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    char buffer[64];
    FILE* fs = fopen("test.bin", "rb");

    static unsigned char mem[100000];
    size_t s = fread(mem, sizeof(char), 50, fs);

    for (int i = 0; i < s; i++)
    {
        printf("%02X ", mem[i]);
    }
    printf("\n\n");

    const unsigned char* next = mem;

    while (next < mem + s) {
        next = disassemble(buffer, next);
        printf("%s\n", buffer);
    }
    return 0;
}


//int main(void)
//{
//    const unsigned char mem[] = { 0x18, 0xA9, 0x05, 0x6D, 0x10, 0x20, 0x8D, 0x00, 0x10 };
//    char buffer[64];
//
//    const unsigned char* next = disassemble(buffer, mem);
//
//    assert(next == mem + 1);
//    assert(strcmp("OPCODE=18[clc i] OPERAND=.. ..", buffer) == 0);
//
//    next = disassemble(buffer, next);
//
//    assert(next == mem + 3);
//    assert(strcmp("OPCODE=A9[lda #] OPERAND=.. 05", buffer) == 0);
//
//    next = disassemble(buffer, next);
//
//    assert(next == mem + 6);
//    assert(strcmp("OPCODE=6D[adc a] OPERAND=20 10", buffer) == 0);
//
//    next = disassemble(buffer, next);
//
//    assert(next == mem + 9);
//    assert(strcmp("OPCODE=8D[sta a] OPERAND=10 00", buffer) == 0);
//
//    return 0;
//}
