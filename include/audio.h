#include "chip8.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

void audio_callback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount);
bool audio_init(Chip8 *chip8);
void audio_destroy();