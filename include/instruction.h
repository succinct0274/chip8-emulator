#pragma once

#include "chip8.h"
#include <stdint.h>

#define NIBBLE_TO_INDEX(category) ((uint8_t) ((category) >> 12))

typedef void (*InstructionHandler)(Chip8* chip8, uint16_t opcode);

void instruction_execute(Chip8 *chip8, uint16_t opcode);

void handle_0000(Chip8 *chip8, uint16_t opcode);
void handle_jump(Chip8 *chip8, uint16_t opcode);
void handle_call(Chip8 *chip8, uint16_t opcode); // Handles 2NNN
void handle_se_vx_nn(Chip8 *chip8, uint16_t opcode); // Handles 3XNN
void handle_sne_vx_nn(Chip8 *chip8, uint16_t opcode); // Handles 4XNN
void handle_se_vx_vy(Chip8 *chip8, uint16_t opcode); // Handles 5XY0
void handle_ld_vx_nn(Chip8 *chip8, uint16_t opcode); // Handles 6XNN
void handle_add_vx_nn(Chip8 *chip8, uint16_t opcode); // Handles 7XNN
void handle_8000(Chip8 *chip8, uint16_t opcode); // Sub-switch for arithmetic group 8XY0 - 8XYE
// void handle_sne_vx_vy(SChip8 *Chip8, uint16_t Opcode); // Handles 9XY0
// void handle_ld_i_nnn(SChip8 *Chip8, uint16_t Opcode); // Handles ANNN
// void handle_jp_v0_nnn(SChip8 *Chip8, uint16_t Opcode); // Handles BNNN
// void handle_rnd_vx_nn(SChip8 *Chip8, uint16_t Opcode); // Handles CXNN
// void handle_drw_vx_vy_n(SChip8 *Chip8, uint16_t Opcode); // Handles DXYN
// void handle_E000(SChip8 *Chip8, uint16_t Opcode); // Sub-switch for key inputs EX9E and EXA1
// void handle_F000(SChip8 *Chip8, uint16_t Opcode); // Sub-switch for FX07, FX55, FX65, etc.

extern const InstructionHandler instruction_table[];


