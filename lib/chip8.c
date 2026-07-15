#include "chip8.h"
#include "common.h"
#include "instruction.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const unsigned char chip8_fontset[CHIP8_FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init(Chip8 *chip8) {
    // Start address of ROM
    chip8->pc = 0x200;
    chip8->opcode = 0;
    chip8->I = 0;
    chip8->sp = 0;

    memset(chip8->gfx, 0, sizeof(chip8->gfx));
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->V, 0, sizeof(chip8->V));
    memset(chip8->memory, 0, sizeof(chip8->memory));
    memset(chip8->key, 0, sizeof(chip8->key));
    memset(chip8->key_prev, 0, sizeof(chip8->key_prev));

    for (int i = 0; i < CHIP8_FONTSET_SIZE; i++) {
        chip8->memory[i] = chip8_fontset[i];
    }

    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    chip8->draw_flag = 1;
}

void chip8_load_rom(Chip8 *chip8, char *filename) {

    char full_path[256];

    int result = snprintf(full_path, sizeof(full_path), "%s", filename);
    if (result < 0 || result >= (int)sizeof(full_path)) {
        fprintf(stderr, "Error: filename is too long\n");
        return;
    }

    printf("Attempting to open %s", full_path);
    FILE *file = fopen(full_path, "rb");
    if (file == NULL) {
        perror("Error opening file\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long rom_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("ROM File Size: %ld Bytes\n", rom_size);
    if (rom_size > CHIP8_MEMORY_SIZE - CHIP8_MEMORY_OFFSET) {
        fprintf(stderr, "Error: ROM file size (%ld bytes) is too long\n", rom_size);
        fclose(file);
        return;
    }

    // Read the rom into chip8 memory
    size_t bytes_read = fread(&chip8->memory[CHIP8_MEMORY_OFFSET], 1, rom_size, file);
    if (bytes_read != (size_t)rom_size) {
        fprintf(stderr, "Error while reading ROM file\n");
        fclose(file);
        return;
    }

    fclose(file);
    printf("Successfully loaded ROM: %s (%ld bytes)\n", filename, rom_size);
}

void chip8_emulate_cycle(Chip8 *chip8) {

    // Fetch opcode (two bytes as one instruction set)
    chip8->opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];

    instruction_execute(chip8, chip8->opcode);
}

void chip8_update_timer(Chip8 *chip8) {
    if (chip8->delay_timer > 0) {
        chip8->delay_timer--;
    }

    if (chip8->sound_timer > 0) {
        chip8->sound_timer--;
    }
}