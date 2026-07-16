#pragma once

#include <stdbool.h>
#include "chip8.h"
#include <SDL3/SDL.h>


bool allow_draw(Chip8 *chip8);
void display_draw(Chip8 *chip8, SDL_Renderer *renderer, SDL_Texture *texture);