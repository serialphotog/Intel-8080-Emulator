/*******************************************************************************
 * File: special.c
 *
 * Purpose:
 *		The various special operations supported by the CPU.
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

#include "special.h"

 // OUT
void out(CPUState *state)
{
	uint8_t port = state->memory[state->pc];
	if (port < sizeof(state->output_ports))
		state->output_ports[port] = state->a;
	if (port == 2)
		state->shift_offset = state->a & 7;
	else if (port == 4)
		state->shift_register = (uint16_t)((state->a << 8) | (state->shift_register >> 8));
	state->pc++;
}

void in(CPUState *state)
{
	uint8_t port = state->memory[state->pc++];
	if (port <= 3)
		state->a = state->input_ports[port];
	else
		state->a = 0;

	if (port == 3)
		state->a = (uint8_t)(state->shift_register >> (8 - state->shift_offset));
}

// EI (enable interrupts)
void ei(CPUState *state)
{
	state->int_enable = 1;
}
