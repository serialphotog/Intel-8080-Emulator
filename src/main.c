/*******************************************************************************
 * File: main.c
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

#include "cpu.h"
#include "platform.h"

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

static void load_roms(CPUState *state, const char *directory)
{
	static const char *names[] = { "invaders.h", "invaders.g", "invaders.f", "invaders.e" };
	char path[1024];
	for (int i = 0; i < 4; ++i) {
		snprintf(path, sizeof(path), "%s/%s", directory, names[i]);
		loadFileIntoMemoryAtOffset(state, path, (uint32_t)i * 0x800);
	}
}

int main(int argc, char **argv)
{
	CPUState *state = InitCPUState();
	Platform *platform;
	/* Toggle to RST 1 at mid-frame, then RST 2 at vertical blank. */
	int interrupt = 2;

	load_roms(state, argc > 1 ? argv[1] : "../rom");
	platform = platform_create();
	if (!platform) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	while (state->running) {
		uint64_t frame_start = SDL_GetPerformanceCounter();
		/* About 2 MHz at the 8080's typical instruction length. */
		for (int instruction = 0; instruction < 7000 && state->running; ++instruction) {
			runCPUCycle(state);
			if (instruction == 3499 || instruction == 6999) {
				if (state->int_enable) {
					interrupt = (interrupt == 1) ? 2 : 1;
					raiseInterrupt(state, interrupt);
				}
			}
		}
		state->running = (uint8_t)platform_update(platform, state);
		{
			uint64_t elapsed = SDL_GetPerformanceCounter() - frame_start;
			uint64_t frame = SDL_GetPerformanceFrequency() / 60;
			if (elapsed < frame)
				SDL_Delay((uint32_t)((frame - elapsed) * 1000 / SDL_GetPerformanceFrequency()));
		}
	}

	platform_destroy(platform);
	free(state->memory);
	free(state);
	return EXIT_SUCCESS;
}
