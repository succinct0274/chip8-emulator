#include "instruction.h"
#include "common.h"
#include <stdio.h>

const InstructionHandler instruction_table[] = {[0x0] = handle_0000, [0x1] = handle_jump, [0x2] = handle_call};

typedef enum {
    OP_CLS = 0x00E0, // Clear the display.
    OP_RET = 0x00EE  // Return from a subroutine
} Opcode;

// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1
void handle_0000(Chip8 *chip8, uint16_t opcode) {
    switch (opcode) {
    case OP_CLS:
        RESET_POINTER(chip8->gfx);
        chip8->draw_flag = true;
        break;
    case OP_RET:
        chip8->sp -= 1;
        chip8->pc = chip8->stack[chip8->sp];
        break;
    default: // 0nnn
        printf("Unknown opcode [0x0000] SYS addr: 0x%X\n", opcode);
        break;
    }

    chip8->pc += 2;
}

// Jump to nnn directly
void handle_jump(Chip8 *chip8, uint16_t opcode) {
    chip8->pc = opcode & 0x0FFF;
}

// Move to nnn, but save current position to stack first
void handle_call(Chip8 *chip8, uint16_t opcode) {
    chip8->stack[chip8->sp] = chip8->pc;
    chip8->sp += 1;
    chip8->pc = opcode & 0x0FFF;
}



