#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed %s\n", SDL_GetError());
        return 1;
    }

    printf("SDL initialization succeed \n");
    SDL_Quit();

    return 0;
}