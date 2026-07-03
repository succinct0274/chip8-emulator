#include <SDL2/SDL.h>
#include <stdio.h>
#include "display.h"
#include "chip8.h"

int main(int argc, char* argv[]) {

    if (!display_init()) {
        return -1;
    }

    Chip8 chip8;
    char *rom_filename;
    if (argc >= 2) {
        chip8_init(&chip8);
        rom_filename = argv[1];
        printf("Loading ROM: %s\n", rom_filename);

        chip8_load_rom(&chip8, rom_filename);
    }
    
    bool quit = false;
    while (!quit) {
        display_draw(&quit);
    }

    display_close();
    return 0;
}