#include "display.h"

// CHIP-8 的原始解析度
#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32
#define WINDOW_SCALE 15 // 放大倍率（64x15 = 960 寬，32x15 = 480 高）

SDL_Window *window = NULL;
static uint32_t pixel_buffer[64 * 32];

void display_draw(Chip8 *chip8, SDL_Renderer *renderer, SDL_Texture *texture) {
    for (int i = 0; i < CHIP8_WIDTH * CHIP8_HEIGHT; i++) {
        if (chip8->gfx[i]) {
			// Foreground: Deep Sea Blue
			pixel_buffer[i] = 0xFF006994;
		} else {
			// Background: Sandy Beige
			pixel_buffer[i] = 0xFFF4E8D1;
		}
    }

    SDL_UpdateTexture(texture, NULL, pixel_buffer, 64 * sizeof(uint32_t));
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}


// bool display_init(void) {
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         printf("SDL initialization failed %s\n", SDL_GetError());
//         return true;
//     }

//     printf("SDL initialization succeed \n");

//     window = SDL_CreateWindow(
//         "CHIP-8-Emulator",
//         SDL_WINDOWPOS_CENTERED,
//         SDL_WINDOWPOS_CENTERED,             // 視窗 Y 軸居中
//         CHIP8_WIDTH * WINDOW_SCALE,         // 視窗寬度
//         CHIP8_HEIGHT * WINDOW_SCALE,        // 視窗高度
//         SDL_WINDOW_SHOWN   
//     );

//     if (window == NULL) {
//         printf("SDL Initialization failed %s\n", SDL_GetError());
//         SDL_Quit();
//         return false;
//     }

//     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
//     if (renderer == NULL) {
//         printf("SDL renderer initialization failed: %s\n", SDL_GetError());
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//         window = NULL;
//         return false;
//     }

//     return true;
// }

// void display_close(void) {
//     if (renderer) {
//         SDL_DestroyRenderer(renderer);
//         renderer = NULL;
//     }

//     if (window) {
//         SDL_DestroyWindow(window);
//         window = NULL;
//     }

//     SDL_Quit();
// }

// void display_draw(bool *quit) {
//     SDL_Event event;

//     // 處理視窗事件（點擊右上角的 X 關閉視窗）
//     while (SDL_PollEvent(&event)) {
//         if (event.type == SDL_QUIT) {
//             *quit = true;
//         }
//     }

//     // 清除畫面（填滿黑色）
//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//     SDL_RenderClear(renderer);

//     // 這裡以後可以用來畫 CHIP-8 的像素...

//     // 把畫好的畫面推送到螢幕上
//     SDL_RenderPresent(renderer);

//     // 稍微延遲，避免 CPU 跑到 100%
//     SDL_Delay(16); // 約 60 FPS
// }