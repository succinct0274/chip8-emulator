#include "display.h"

// CHIP-8 的原始解析度
#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

SDL_Window *window = NULL;
static uint32_t pixel_buffer[64 * 32];

void display_draw(Chip8 *chip8, SDL_Renderer *renderer, SDL_Texture *texture) {
    for (int i = 0; i < CHIP8_WIDTH * CHIP8_HEIGHT; i++) {
        if (chip8->gfx[i]) {
			pixel_buffer[i] = 0xFFFFCC01;
		} else {
			pixel_buffer[i] = 0xFF996700;
		}
    }

    SDL_UpdateTexture(texture, NULL, pixel_buffer, 64 * sizeof(uint32_t));
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
    chip8->draw_flag = false;
}
