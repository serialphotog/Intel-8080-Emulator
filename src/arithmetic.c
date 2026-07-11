/*******************************************************************************
 * File: arithmetic.c
 *
 * Purpose:
 *		The various arithmetic operations supported by the Intel 8080 CPU.
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

#include "arithmetic.h"

 // ADI (add immediate)
void adi(CPUState *state, unsigned char *opcode)
{
	add(state, opcode[1], 0);
	state->pc++;
}

void add(CPUState *state, uint8_t value, uint8_t carry)
{
	uint8_t before = state->a;
	uint16_t result = (uint16_t)before + value + carry;
	state->a = (uint8_t)result;
	state->cc.z = (state->a == 0);
	state->cc.s = ((state->a & 0x80) != 0);
	state->cc.p = calculateParity(state->a, 8);
	state->cc.cy = (result > 0xff);
	state->cc.ac = (((before & 0x0f) + (value & 0x0f) + carry) > 0x0f);
}

// DAA (decimal adjust accumulator)
void daa(CPUState *state)
{
	uint8_t before = state->a;
	uint8_t correction = 0;
	uint8_t carry = state->cc.cy;
	uint16_t result;

	if ((before & 0x0f) > 9 || state->cc.ac)
		correction |= 0x06;
	if (before > 0x99 || carry)
		correction |= 0x60;

	result = (uint16_t)before + correction;
	state->a = (uint8_t)result;
	state->cc.z = (state->a == 0);
	state->cc.s = ((state->a & 0x80) != 0);
	state->cc.p = calculateParity(state->a, 8);
	state->cc.ac = (((before & 0x0f) + (correction & 0x0f)) > 0x0f);
	state->cc.cy = (carry || result > 0xff);
}

// DAD (direct add)
void dad(uint8_t *h, uint8_t *l, uint8_t *hi, uint8_t *lo, CPUState *state)
{
	uint32_t hl = build2ByteValue(*h, *l);
	uint32_t hilo = build2ByteValue(*hi, *lo);
	uint32_t res = hl + hilo;
	*h = (res & 0xff00) >> 8;
	*l = res & 0xff;
	state->cc.cy = ((res & 0xffff0000) != 0);
}

// DAD H (direct add with H)
void dad_h(uint8_t *h, uint8_t *l, CPUState *state)
{
	uint32_t hl = build2ByteValue(*h, *l);
	uint32_t res = hl + hl;
	*h = (res & 0xff00) >> 8;
	*l = res & 0xff;
	state->cc.cy = ((res & 0xffff0000) != 0);
}

// INX
void inx(uint8_t *reg1, uint8_t *reg2)
{
	(*reg2)++;
	if (*reg2 == 0)
		(*reg1)++;
}

void dcx(uint8_t *high, uint8_t *low)
{
	uint16_t value = build2ByteValue(*high, *low);
	value = (uint16_t)(value - 1);
	*high = (uint8_t)(value >> 8);
	*low = (uint8_t)value;
}

// INR (increment register or memory byte)
void inr(CPUState *state, uint8_t *value)
{
	uint8_t before = *value;
	uint8_t result = (uint8_t)(before + 1);
	state->cc.z = (result == 0);
	state->cc.s = ((result & 0x80) != 0);
	state->cc.p = calculateParity(result, 8);
	state->cc.ac = ((before & 0x0f) == 0x0f);
	*value = result;
}

// DCR (decrement register)
void dcr(CPUState *state, uint8_t *reg, unsigned char *opcode)
{
	uint8_t before = *reg;
	uint8_t res = (uint8_t)(before - 1);
	state->cc.z = (res == 0);
	state->cc.s = ((res & 0x80) == 0x80);
	state->cc.p = calculateParity(res, 8);
	state->cc.ac = ((before & 0x0f) == 0);
	*reg = res;
}

void sub(CPUState *state, uint8_t value, uint8_t borrow)
{
	uint8_t before = state->a;
	uint16_t operand = (uint16_t)value + borrow;
	uint8_t result = (uint8_t)(before - operand);
	state->a = result;
	state->cc.z = (result == 0);
	state->cc.s = ((result & 0x80) != 0);
	state->cc.p = calculateParity(result, 8);
	state->cc.cy = ((uint16_t)before < operand);
	state->cc.ac = ((before & 0x0f) < ((value & 0x0f) + borrow));
}
