/*******************************************************************************
 * File: platform.c
 *
 * Purpose:
 *		Provides the platform layer for the emulator.
 *
 * Copyright 2018, 2026 Adam Thompson <adam@hackeradam.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ******************************************************************************/

#include "platform.h"

#include <SDL.h>
#include <stdlib.h>

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256
#define VIDEO_RAM 0x2400
#define AUDIO_RATE 48000

typedef struct Voice {
	float phase;
	float frequency;
	float volume;
	int remaining;
	int noise;
} Voice;

struct Platform {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
	uint8_t coin_frames;
	SDL_AudioDeviceID audio_device;
	Voice voices[5];
	uint32_t noise_state;
	uint8_t last_sound3;
	uint8_t last_sound5;
	uint8_t ufo_active;
};

static void audio_callback(void *userdata, Uint8 *stream, int length)
{
	Platform *p = userdata;
	float *samples = (float *)stream;
	int count = length / (int)sizeof(float);
	for (int i = 0; i < count; ++i) {
		float mixed = 0.0f;
		if (p->ufo_active) {
			p->voices[0].phase += 110.0f / AUDIO_RATE;
			if (p->voices[0].phase >= 1.0f) p->voices[0].phase -= 1.0f;
			mixed += (p->voices[0].phase < 0.45f ? 0.10f : -0.10f);
		}
		for (int v = 1; v < 5; ++v) {
			Voice *voice = &p->voices[v];
			if (voice->remaining <= 0) continue;
			float value;
			if (voice->noise) {
				p->noise_state = p->noise_state * 1664525u + 1013904223u;
				value = (p->noise_state & 0x80000000u) ? voice->volume : -voice->volume;
			} else {
				voice->phase += voice->frequency / AUDIO_RATE;
				if (voice->phase >= 1.0f) voice->phase -= 1.0f;
				value = voice->phase < 0.5f ? voice->volume : -voice->volume;
			}
			mixed += value * ((float)voice->remaining / (float)AUDIO_RATE + 0.15f);
			voice->remaining--;
		}
		if (mixed > 0.8f) mixed = 0.8f;
		if (mixed < -0.8f) mixed = -0.8f;
		samples[i] = mixed;
	}
}

static void trigger_voice(Voice *voice, float frequency, float volume, int milliseconds, int noise)
{
	voice->phase = 0.0f;
	voice->frequency = frequency;
	voice->volume = volume;
	voice->remaining = AUDIO_RATE * milliseconds / 1000;
	voice->noise = noise;
}

static void update_sound(Platform *p, CPUState *state)
{
	uint8_t sound3 = state->output_ports[3];
	uint8_t sound5 = state->output_ports[5];
	uint8_t rise3 = sound3 & (uint8_t)~p->last_sound3;
	uint8_t rise5 = sound5 & (uint8_t)~p->last_sound5;
	if (!p->audio_device) return;
	SDL_LockAudioDevice(p->audio_device);
	p->ufo_active = sound3 & 0x01;
	if (rise3 & 0x02) trigger_voice(&p->voices[1], 700.0f, 0.20f, 180, 1); /* shot */
	if (rise3 & 0x04) trigger_voice(&p->voices[2], 90.0f, 0.24f, 550, 1);  /* explosion */
	if (rise3 & 0x08) trigger_voice(&p->voices[3], 420.0f, 0.16f, 100, 0); /* invader hit */
	if (rise3 & 0x10) trigger_voice(&p->voices[3], 880.0f, 0.14f, 250, 0); /* bonus */
	for (int bit = 0; bit < 4; ++bit)
		if (rise5 & (1u << bit))
			trigger_voice(&p->voices[4], 85.0f + bit * 22.0f, 0.20f, 70, 0);
	if (rise5 & 0x10) trigger_voice(&p->voices[2], 160.0f, 0.24f, 400, 1); /* UFO hit */
	p->last_sound3 = sound3;
	p->last_sound5 = sound5;
	SDL_UnlockAudioDevice(p->audio_device);
}

static void set_control(Platform *platform, CPUState *state, SDL_Keycode key, int down)
{
	uint8_t port = 1, mask = 0;
	switch (key) {
	case SDLK_c:
	case SDLK_5:
		if (down) {
			state->input_ports[1] |= 0x01;
			platform->coin_frames = 6;
		}
		return;
	case SDLK_2: mask = 0x02; break;
	case SDLK_1: mask = 0x04; break;
	case SDLK_SPACE: mask = 0x10; break;
	case SDLK_LEFT: mask = 0x20; break;
	case SDLK_RIGHT: mask = 0x40; break;
	case SDLK_UP: mask = 0x10; port = 2; break;
	case SDLK_a: mask = 0x20; port = 2; break;
	case SDLK_d: mask = 0x40; port = 2; break;
	default: return;
	}
	if (down) state->input_ports[port] |= mask;
	else state->input_ports[port] &= (uint8_t)~mask;
}

Platform *platform_create(void)
{
	Platform *p = calloc(1, sizeof(*p));
	if (!p || SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != 0) return NULL;
	p->window = SDL_CreateWindow("Intel 8080 - Space Invaders", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, SDL_WINDOW_RESIZABLE);
	p->renderer = SDL_CreateRenderer(p->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!p->renderer)
		p->renderer = SDL_CreateRenderer(p->window, -1, SDL_RENDERER_SOFTWARE);
	p->texture = SDL_CreateTexture(p->renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!p->window || !p->renderer || !p->texture) { platform_destroy(p); return NULL; }
	SDL_RenderSetLogicalSize(p->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	{
		SDL_AudioSpec desired;
		SDL_zero(desired);
		desired.freq = AUDIO_RATE;
		desired.format = AUDIO_F32SYS;
		desired.channels = 1;
		desired.samples = 1024;
		desired.callback = audio_callback;
		desired.userdata = p;
		p->noise_state = 0x8080u;
		p->audio_device = SDL_OpenAudioDevice(NULL, 0, &desired, NULL, 0);
		if (p->audio_device) SDL_PauseAudioDevice(p->audio_device, 0);
	}
	return p;
}

int platform_update(Platform *p, CPUState *state)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) return 0;
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) return 0;
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
			set_control(p, state, event.key.keysym.sym, event.type == SDL_KEYDOWN);
	}
	if (p->coin_frames > 0) {
		p->coin_frames--;
		if (p->coin_frames == 0)
			state->input_ports[1] &= (uint8_t)~0x01;
	}
	update_sound(p, state);
	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
		for (int x = 0; x < SCREEN_WIDTH; ++x) {
			int source_bit = x * SCREEN_HEIGHT + (SCREEN_HEIGHT - 1 - y);
			uint8_t byte = state->memory[VIDEO_RAM + source_bit / 8];
			p->pixels[y * SCREEN_WIDTH + x] = (byte & (1u << (source_bit & 7))) ? 0xffffffff : 0xff000000;
		}
	}
	SDL_UpdateTexture(p->texture, NULL, p->pixels, SCREEN_WIDTH * (int)sizeof(uint32_t));
	SDL_RenderClear(p->renderer);
	SDL_RenderCopy(p->renderer, p->texture, NULL, NULL);
	SDL_RenderPresent(p->renderer);
	return 1;
}

void platform_destroy(Platform *p)
{
	if (!p) return;
	if (p->audio_device) SDL_CloseAudioDevice(p->audio_device);
	if (p->texture) SDL_DestroyTexture(p->texture);
	if (p->renderer) SDL_DestroyRenderer(p->renderer);
	if (p->window) SDL_DestroyWindow(p->window);
	SDL_Quit();
	free(p);
}
