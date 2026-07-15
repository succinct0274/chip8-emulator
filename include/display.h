#pragma once

#include <stdbool.h>
#include "chip8.h"
#include <SDL3/SDL.h>

// bool display_init(void);
// void display_close(void);
// void display_draw(bool *quit);

void display_draw(Chip8 *chip8, SDL_Renderer *renderer, SDL_Texture *texture);