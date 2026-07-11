/*******************************************************************************
 * File: logic.c
 *
 * Purpose:
 *		The various logical operations supported by the Intel 8080.
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

#include "logic.h"

 // ANA (and a)
void ana(CPUState *state, uint8_t *reg)
{
	state->a = state->a & *reg;
	setFlagsFromA(state);
	state->cc.ac = 1;
}

// ANI (and immediate with A)
void ani(CPUState *state, unsigned char *opcode)
{
	state->a = state->a & opcode[1];
	setFlagsFromA(state);
	state->cc.ac = 1;
	state->pc++;
}

// XRA (xor with a)
void xra(CPUState *state, uint8_t *reg)
{
	state->a = state->a ^ *reg;
	setFlagsFromA(state);
}

// ORA (or with accumulator)
void ora(CPUState *state, uint8_t value)
{
	state->a |= value;
	setFlagsFromA(state);
}

// CPI (compare immediate with A)
void cpi(CPUState *state, unsigned char *opcode)
{
	cmp(state, opcode[1]);
	state->pc++;
}

void cmp(CPUState *state, uint8_t value)
{
	uint8_t res = (uint8_t)(state->a - value);
	state->cc.z = (res == 0);
	state->cc.s = ((res & 0x80) == 0x80);
	state->cc.p = calculateParity(res, 8);
	state->cc.cy = (state->a < value);
	state->cc.ac = ((state->a & 0x0f) < (value & 0x0f));
}

// RRC (Rotate A right)
void rrc(CPUState *state)
{
	uint8_t previousA = state->a;
	state->a = ((previousA & 0x01) << 7) | (previousA >> 1);
	state->cc.cy = ((previousA & 0x01) == 0x01);
}

void rlc(CPUState *state)
{
	uint8_t bit = (uint8_t)(state->a >> 7);
	state->a = (uint8_t)((state->a << 1) | bit);
	state->cc.cy = bit;
}

void ral(CPUState *state)
{
	uint8_t old_carry = state->cc.cy;
	state->cc.cy = (uint8_t)(state->a >> 7);
	state->a = (uint8_t)((state->a << 1) | old_carry);
}

void rar(CPUState *state)
{
	uint8_t old_carry = state->cc.cy;
	state->cc.cy = state->a & 1;
	state->a = (uint8_t)((state->a >> 1) | (old_carry << 7));
}
