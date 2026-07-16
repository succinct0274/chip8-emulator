#include "audio.h"
#include <stdio.h>

#define SAMPLING_RATE 44100

static SDL_AudioDeviceID adi;
static SDL_AudioStream *stream = NULL;

void SDLCALL audio_callback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount) {
    Chip8 *chip8 = (Chip8 *) userdata;

    int bytes_needed = additional_amount;
    int sample_count = bytes_needed / sizeof(float);
    float *buffer = SDL_malloc(bytes_needed);

    if (!buffer) {
        return;
    }

    static uint64_t total_samples = 0; 
    float target_frequency = 440.0f;
    float sample_rate = 44100.0f;
    float phase_increment = target_frequency / sample_rate;
    float samples_per_cycle = sample_rate / target_frequency;

    for (int i = 0; i < sample_count; i++) {
        if (chip8->sound_timer > 0) {
            // Get the reminder of total_samples / samples_per_cycle
            float cycle_position = SDL_fmodf((float) total_samples, samples_per_cycle);

            if (cycle_position < (samples_per_cycle / 2.0f)) {
                buffer[i] = 0.15f;
            } else {
                buffer[i] = -0.15f;
            }

            total_samples++;
        } else {
            buffer[i] = 0.0f;
            total_samples = 0;
        }
    }

    SDL_PutAudioStreamData(stream, buffer, bytes_needed);
    SDL_free(buffer);
}

bool audio_init(Chip8 *chip8) {

    adi = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

    if (adi == NULL) {
        fprintf(stderr, "Unable to open audio device: %s\n", SDL_GetError());
        return false;
    }

    SDL_AudioSpec spec = {SDL_AUDIO_F32, 1, SAMPLING_RATE};

    stream = SDL_CreateAudioStream(&spec, NULL);
    if (stream == NULL) {
        fprintf(stderr, "Unable to open audio stream: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_BindAudioStream(adi, stream)) {
        fprintf(stderr, "Unable to bind audio stream and device: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetAudioStreamGetCallback(stream, audio_callback, chip8);
    SDL_ResumeAudioDevice(adi);
    return true;
}

void audio_destroy() {
    SDL_PauseAudioDevice(adi);
	SDL_DestroyAudioStream(stream);
	SDL_CloseAudioDevice(adi);
}