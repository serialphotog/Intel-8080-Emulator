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

struct Platform {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
	uint8_t coin_frames;
};

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
	if (!p || SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) return NULL;
	p->window = SDL_CreateWindow("Intel 8080 - Space Invaders", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, SDL_WINDOW_RESIZABLE);
	p->renderer = SDL_CreateRenderer(p->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!p->renderer)
		p->renderer = SDL_CreateRenderer(p->window, -1, SDL_RENDERER_SOFTWARE);
	p->texture = SDL_CreateTexture(p->renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!p->window || !p->renderer || !p->texture) { platform_destroy(p); return NULL; }
	SDL_RenderSetLogicalSize(p->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
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
	if (p->texture) SDL_DestroyTexture(p->texture);
	if (p->renderer) SDL_DestroyRenderer(p->renderer);
	if (p->window) SDL_DestroyWindow(p->window);
	SDL_Quit();
	free(p);
}
