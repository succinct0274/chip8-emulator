#pragma once

#include <stdbool.h>
#include "chip8.h"
#include <SDL2/SDL.h>

bool display_init(void);
void display_close(void);
void display_draw(bool *quit);