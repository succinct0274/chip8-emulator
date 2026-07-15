#include "input.h"
#include <SDL3/SDL.h>

static const SDL_Scancode KEYMAP[CHIP8_BUTTON_COUNT] = {SDL_SCANCODE_X,
                                    SDL_SCANCODE_1,
                                    SDL_SCANCODE_2,
                                    SDL_SCANCODE_3,
                                    SDL_SCANCODE_Q,
                                    SDL_SCANCODE_W,
                                    SDL_SCANCODE_E,
                                    SDL_SCANCODE_A,
                                    SDL_SCANCODE_S,
                                    SDL_SCANCODE_D,
                                    SDL_SCANCODE_Z,
                                    SDL_SCANCODE_C,
                                    SDL_SCANCODE_4,
                                    SDL_SCANCODE_R,
                                    SDL_SCANCODE_F,
                                    SDL_SCANCODE_V};

SDL_AppResult handle_input(Chip8 *chip8, SDL_Event *event) {
    if (!(event->type == SDL_EVENT_KEY_DOWN) && !(event->type == SDL_EVENT_KEY_UP)) {
        return SDL_APP_CONTINUE;
    }

    if (event->type == SDL_EVENT_KEY_DOWN && event->key.scancode == SDL_SCANCODE_ESCAPE) {
        return SDL_APP_SUCCESS;
    }

    uint8_t state = event->type == SDL_EVENT_KEY_DOWN ? 1 : 0;
    for (int i = 0; i < CHIP8_BUTTON_COUNT; i++) {
        if (event->key.scancode == KEYMAP[i]) {
            chip8->key[i] = state;
            break;
        }
    }

    return SDL_APP_CONTINUE;
}