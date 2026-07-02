#include "chip8.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_POINTER(ptr) (memset((ptr), 0, (sizeof(*(ptr)))))

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

    INIT_POINTER(chip8->gfx);
    INIT_POINTER(chip8->stack);
    INIT_POINTER(chip8->V);
    INIT_POINTER(chip8->memory);
    INIT_POINTER(chip8->key);
    INIT_POINTER(chip8->key_prev);

    for (int i = 0; i < CHIP8_FONTSET_SIZE; i++) {
        chip8->memory[i] = chip8_fontset[i];
    }

    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    chip8->draw_flag = 1;
}

void chip8_load_rom(Chip8 *chip8, char *filename) {
	
	char full_path[256];
	
	int result = snprintf(full_path, sizeof(full_path), "CHIP-8 Emulator | ROM: %s", filename);
	if (result < 0 || result >= (int) sizeof(full_path)) {
		fprintf(stderr, "Error: filename is too long\n");
		return;
	}

	FILE *file = fopen(full_path, "rb");
	if (file == NULL) {
		fprintf(stderr, "Error opening file\n");
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
	if (bytes_read != (size_t) rom_size) {
		fprintf(stderr, "Error while reading ROM file\n");
		fclose(file);
		return;
	}

	fclose(file);
	printf("Successfully loaded ROM: %s (%ld bytes)\n", filename, rom_size);
}