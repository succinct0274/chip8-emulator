#include "instruction.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

const InstructionHandler instruction_table[] = {[0x0] = handle_0000,
                                                [0x1] = handle_jump,
                                                [0x2] = handle_call,
                                                [0x3] = handle_se_vx_nn,
                                                [0x4] = handle_sne_vx_nn,
                                                [0x5] = handle_se_vx_vy,
                                                [0x6] = handle_ld_vx_nn,
                                                [0x7] = handle_add_vx_nn,
                                                [0x8] = handle_8000,
                                                [0x9] = handle_sne_vx_vy,
                                                [0xA] = handle_ld_i_nnn,
                                                [0xB] = handle_jp_v0_nnn,
                                                [0xC] = handle_rnd_vx_nn,
                                                [0xD] = handle_drw_vx_vy_n};

typedef enum {
    OP_CLS = 0x00E0, // Clear the display.
    OP_RET = 0x00EE  // Return from a subroutine
} Opcode_0_Subtype;

typedef enum {
    OP_LD = 0x0,
    OP_OR = 0x1,
    OP_AND = 0x2,
    OP_XOR = 0x3,
    OP_ADD = 0x4,
    OP_SUB = 0x5,
    OP_SHR = 0X6,
    OP_SUBN = 0x7,
    OP_SHL = 0XE
} Opcode_8_Subtype;

void instruction_execute(Chip8 *chip8, uint16_t opcode) {
    uint8_t idx = NIBBLE_TO_INDEX(opcode & 0xF000);
    printf("Retrieve instruction at index %d\n", idx);
    instruction_table[idx](chip8, opcode);
}

// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1
void handle_0000(Chip8 *chip8, uint16_t opcode) {

    fprintf(stderr, "opcode: %d\n", opcode);
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

void handle_se_vx_nn(Chip8 *chip8, uint16_t opcode) {
    // Register index
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = opcode & 0x00FF;
    if (chip8->V[x] == nn) {
        chip8->pc += 4;
    } else {
        chip8->pc += 2;
    }
}

void handle_sne_vx_nn(Chip8 *chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = opcode & 0x00FF;
    if (chip8->V[x] != nn) {
        chip8->pc += 4;
    } else {
        chip8->pc += 2;
    }
}

void handle_se_vx_vy(Chip8 *chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (chip8->V[x] == chip8->V[y]) {
        chip8->pc += 4;
    } else {
        chip8->pc += 2;
    }
}

void handle_ld_vx_nn(Chip8 *chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = (opcode & 0x00FF);
    chip8->V[x] = kk;
    chip8->pc += 2;
}

void handle_add_vx_nn(Chip8 *chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = (opcode & 0x00FF);
    chip8->V[x] += kk;
    chip8->pc += 2;
}

void handle_8000(Chip8 *chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t type = opcode & 0x000F;

    switch (type) {
    case OP_LD:
        chip8->V[x] = chip8->V[y];
        break;
    case OP_OR:
        chip8->V[x] |= chip8->V[y];
        break;
    case OP_AND:
        chip8->V[x] &= chip8->V[y];
        break;
    case OP_XOR:
        chip8->V[x] ^= chip8->V[y];
        break;
    case OP_ADD:
        // Carry if Vx + Vy > 255
        chip8->V[15] = (chip8->V[x] > (0xFF - chip8->V[y])) ? 1 : 0;
        chip8->V[x] += chip8->V[y];
        break;
    case OP_SUB:
        // Borrow flag: VF = 1 if Vx >= Vy
        chip8->V[15] = (chip8->V[x] >= chip8->V[y]) ? 1 : 0;
        chip8->V[x] -= chip8->V[y];
        break;
    case OP_SHR:
        chip8->V[15] = (chip8->V[x] & 0x01);
        chip8->V[x] >>= 1;
        break;
    case OP_SUBN:
        // Borrow flag: VF = 1 if Vy >= Vx
        chip8->V[15] = (chip8->V[y] >= chip8->V[x]) ? 1 : 0;
        chip8->V[x] = chip8->V[y] - chip8->V[x];
        break;
    case OP_SHL:
        chip8->V[15] = (chip8->V[x] >> 7) & 0x01;
        chip8->V[x] <<= 1;
        break;
    default:
        fprintf(stderr, "Unknown opcode 0x%X\n", opcode);
        break;
    }

    chip8->pc += 2;
}

void handle_sne_vx_vy(Chip8 *chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if (chip8->V[x] != chip8->V[y]) {
        chip8->pc += 4;
    } else {
        chip8->pc += 2;
    }
}

void handle_ld_i_nnn(Chip8 *chip8, uint16_t opcode) {
    uint16_t nnn = opcode & 0x0FFF;
    chip8->I = nnn;
    chip8->pc += 2;
}

void handle_jp_v0_nnn(Chip8 *chip8, uint16_t opcode) {
    uint16_t nnn = opcode & 0x0FFF;
    chip8->pc = nnn + chip8->V[0];
}

void handle_rnd_vx_nn(Chip8 *chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint16_t rbyte = (rand() % 256);
    uint16_t kk = opcode & 0x00FF;
    chip8->V[x] = rbyte & kk;
    chip8->pc += 2;
}

// Render the pixel by drawing the rfx 2-d array
void handle_drw_vx_vy_n(Chip8 *chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = (opcode & 0x000F);

    chip8->V[15] = 0;
    for (int row = 0; row < n; row++) {

        if (chip8->I + row >= CHIP8_MEMORY_SIZE) {
            fprintf(stderr, "Memory exceeded range");
            return;
        }
        
        uint8_t sprite_byte = chip8->memory[chip8->I + row];
        
        // 8 bits as each instruction is 2-bytes long
        for (int col = 0; col < 8; col++) {
            // For XOR, if it is 0 bit, no modification needed
            if((sprite_byte & (0x80 >> col)) != 0) {
                int x_pos = (chip8->V[x] + col) % CHIP8_GFX_WIDTH;
                int y_pos = (chip8->V[y] + row) % CHIP8_GFX_HEIGHT;

                // Clipping pixel to retain actual hardware accuracy
                if (x_pos >= CHIP8_GFX_WIDTH || y_pos >= CHIP8_GFX_HEIGHT) {
                    continue;
                }

                if (chip8->gfx[x_pos + y_pos * CHIP8_GFX_WIDTH] == 1) {
                    chip8->V[15] = 1;
                }

                chip8->gfx[x_pos + y_pos * CHIP8_GFX_WIDTH] ^= 1;
            }
        }
    }

    chip8->draw_flag = true;
    chip8->pc += 2;
}