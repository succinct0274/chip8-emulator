#include <SDL2/SDL.h>
#include <stdio.h>
#include "display.h"

int main(int argc, char* argv[]) {

    if (!display_init()) {
        return -1;
    }

    bool quit = false;

    while (!quit) {
        display_draw(&quit);
    }

    display_close();
    return 0;
}