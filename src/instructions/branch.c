/*******************************************************************************
 * File: branch.c
 *
 * Purpose:
 *		The various branch instructions supported by the Intel 8080. These 
 *		include calls such as jumps and calls.
 *
 * Copyright 2018 Adam Thompson <adam@serialphotog.com>
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

#include "branch.h"

#include "data.h"

// JMP (unconditional jump)
void jmp(CPUState *state, unsigned char *opcode)
{
	state->pc = (opcode[2] << 8) | opcode[1];
}

// JNZ (jump not zero)
void jnz(CPUState *state, unsigned char *opcode)
{
	if (state->cc.z == 0)
		jmp(state, opcode);
	else
		state->pc += 2;
}

// CALL (unconditional call)
void call(CPUState *state, unsigned char *opcode)
{
	// PUSH our return location on to the stack
	uint16_t ret = state->pc + 2;
	uint8_t hi = (ret & 0xff);
	uint8_t lo = (ret >> 8) & 0xff;
	push(&hi, &lo, &state->sp, state->memory);

	// Jump to the desired location
	jmp(state, opcode);
}

// RET (return)
void ret(CPUState *state)
{
	state->pc = fetchFromMemory(state->memory, state->sp) | 
		(fetchFromMemory(state->memory, state->sp + 1) << 8);
	state->sp += 2;
}
