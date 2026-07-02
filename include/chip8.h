#pragma once

#include <stdbool.h>

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_FONTSET_SIZE 80
#define CHIP8_MEMORY_OFFSET 0x200 // 512


// https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
typedef struct {
    unsigned short opcode;
    unsigned char memory[CHIP8_MEMORY_SIZE];

    // V0 - VE, last byte for carry forward
    unsigned char V[16];

    /*
    0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    0x200-0xFFF - Program ROM and work RAM
    */
    unsigned short I;   // Index register
    unsigned short pc;  

    unsigned char gfx[64 * 32];
    
    unsigned char delay_timer;
    unsigned char sound_timer;
    bool draw_flag;

    unsigned short stack[16];
    unsigned short sp;  // stack pointer

    unsigned char key[16];
    unsigned char key_prev[16];

} Chip8;

extern const unsigned char chip8_fontset[80];

void chip8_init(Chip8 *chip8);
void chip8_load_rom(Chip8 *chip8, char *filename);
// void emulateCycle(Chip8 *chip8);