#define SDL_MAIN_USE_CALLBACKS 1 // Tell SDL to use the callback architecture
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "chip8.h"
#include "display.h"
#include <stdio.h>


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Chip8 chip8;
    bool running;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }

    AppState *ctx = SDL_calloc(1, sizeof(AppState));
    if (!ctx) {
        return SDL_APP_FAILURE;
    }

    *appstate = ctx;

    ctx->window = SDL_CreateWindow("SDL3 CHIP-8 Window", 640, 480, 0);
    if (!ctx->window) {
        return SDL_APP_FAILURE;
    }

    ctx->renderer = SDL_CreateRenderer(ctx->window, NULL);
    if (!ctx->renderer) {
        return SDL_APP_FAILURE;
    }

    // Initialize chip8
    chip8_init(&ctx->chip8);
    char *rom_filename;
    if (argc >= 2) {
        chip8_init(&ctx->chip8);
        rom_filename = argv[1];
        printf("Loading ROM: %s\n", rom_filename);

        chip8_load_rom(&ctx->chip8, rom_filename);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *ctx = (AppState *)appstate;
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; // Clean exit
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *ctx = (AppState *)appstate;

    // Render clear screen (Red background)
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
    SDL_RenderClear(ctx->renderer);
    SDL_RenderPresent(ctx->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (appstate) {
        AppState *ctx = (AppState *)appstate;
        SDL_DestroyRenderer(ctx->renderer);
        SDL_DestroyWindow(ctx->window);
        SDL_free(ctx);
    }
    SDL_Quit();
}

// int main(int argc, char* argv[]) {

//     if (!display_init()) {
//         return -1;
//     }

//     Chip8 chip8;
//     char *rom_filename;
//     if (argc >= 2) {
//         chip8_init(&chip8);
//         rom_filename = argv[1];
//         printf("Loading ROM: %s\n", rom_filename);

//         chip8_load_rom(&chip8, rom_filename);
//     }

//     bool quit = false;
//     while (!quit) {
//         display_draw(&quit);
//     }

//     display_close();
//     return 0;
// }