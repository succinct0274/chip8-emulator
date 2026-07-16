#define SDL_MAIN_USE_CALLBACKS 1 // Tell SDL to use the callback architecture
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "chip8.h"
#include "display.h"
#include "input.h"
#include <stdio.h>


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    Chip8 chip8;

    uint64_t last_time;
    double timer_accumulator;
    double cpu_accumulator;    // Tracks partial time for CPU cycles
    bool vblank_ready;         // Flag signaling a new 60Hz frame has arrived
    bool display_wait_quirk;  
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

    ctx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_ARGB8888,
															SDL_TEXTUREACCESS_STREAMING, CHIP8_GFX_WIDTH,
															CHIP8_GFX_HEIGHT);
	if (ctx->texture == NULL) {
		fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_SetTextureScaleMode(ctx->texture, SDL_SCALEMODE_NEAREST)) {
		fprintf(stderr, "Nearest neighbour texture filtering failed to enable.");
	}

    // Initialize chip8
    char *rom_filename;
    if (argc >= 2) {
        chip8_init(&ctx->chip8);
        rom_filename = argv[1];
        printf("Loading ROM: %s\n", rom_filename);

        chip8_load_rom(&ctx->chip8, rom_filename);

        printf("Loaded ROM: %s\n", rom_filename);

    }

    ctx->last_time = SDL_GetTicks();
    ctx->timer_accumulator = 0.0;

    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx->renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *ctx = (AppState *)appstate;
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; // Clean exit
    }

    // handle keyboard input
    return handle_input(&ctx->chip8, event);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *ctx = (AppState *)appstate;

    uint64_t current_time = SDL_GetTicks();
    double delta_time = (double)(current_time - ctx->last_time); 
    ctx->last_time = current_time;

    // Accumulate time for both systems
    ctx->timer_accumulator += delta_time;
    ctx->cpu_accumulator += delta_time;

    // 1. Process 60Hz System Timers
    const double target_60hz_ms = 1000.0 / 60.0; // ~16.67ms
    while (ctx->timer_accumulator >= target_60hz_ms) {
        chip8_update_timer(&ctx->chip8);
        ctx->vblank_ready = true; 
        ctx->timer_accumulator -= target_60hz_ms;
    }

    // 2. Process CPU Cycles Dynamically (Assuming a target of ~500Hz)
    // 500Hz means 1 cycle every 2.0 milliseconds
    const double target_cpu_ms = 1000.0 / 500.0; 
    
    memcpy(ctx->chip8.key_prev, ctx->chip8.key, sizeof(ctx->chip8.key));

    while (ctx->cpu_accumulator >= target_cpu_ms) {
  
        bool is_draw_opcode = allow_draw(&ctx->chip8);
        // If Display Wait is enabled, we are hitting a draw opcode, AND vblank hasn't happened yet...
        if (ctx->display_wait_quirk && is_draw_opcode && !ctx->vblank_ready) {
            // STOP executing instructions for this iteration. 
            // Leave the cpu_accumulator alone so it retries next time!
            break; 
        }

        // Run the cycle normally
        chip8_emulate_cycle(&ctx->chip8);

        // If we just successfully executed a Draw Opcode, consume our VBLANK token
        if (is_draw_opcode) {
            ctx->vblank_ready = false; 
        }

        ctx->cpu_accumulator -= target_cpu_ms;
    }

    // 3. Render Pass
    if (ctx->chip8.draw_flag) {
        display_draw(&ctx->chip8, ctx->renderer, ctx->texture);
    }

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