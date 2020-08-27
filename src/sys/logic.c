/*******************************************************************************
 * File: logic.c
 *
 * Purpose:
 *		The various logical operations supported by the Intel 8080.
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

#include "sys/logic.h"

 // ANA (and a)
void ana(CPUState *state, uint8_t *reg)
{
	state->a = state->a & *reg;
	setFlagsFromA(state);
}

// ANI (and immediate with A)
void ani(CPUState *state, unsigned char *opcode)
{
	state->a = state->a & opcode[1];
	setFlagsFromA(state);
	state->pc++;
}

// XRA (xor with a)
void xra(CPUState *state, uint8_t *reg)
{
	state->a = state->a ^ *reg;
	setFlagsFromA(state);
}

// CPI (compare immediate with A)
void cpi(CPUState *state, unsigned char *opcode)
{
	uint8_t res = state->a - opcode[1];
	state->cc.z = (res == 0);
	state->cc.s = ((res & 0x80) == 0x80);
	state->cc.p = calculateParity(res, 8);
	state->cc.cy = (state->a < opcode[1]);
	state->pc++;
}

// RRC (Rotate A right)
void rrc(CPUState *state)
{
	uint8_t previousA = state->a;
	state->a = ((previousA & 0x01) << 7) | (previousA >> 1);
	state->cc.cy = ((previousA & 0x01) == 0x01);
}
