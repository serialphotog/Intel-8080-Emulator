/*******************************************************************************
 * File: arithmetic.c
 *
 * Purpose: 
 *		The various arithmetic operations supported by the Intel 8080 CPU.
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

#include "arithmetic.h"

// ADI (add immediate)
void adi(CPUState *state, unsigned char *opcode)
{
	// Note, we cast the value to 16-bits for higher precision. This is sow we
	// can capture the carry out value.
	uint16_t res = (uint16_t) state->a + (uint16_t) opcode[1];
	state->a = (uint8_t) res;
	state->cc.z = ((res & 0xff) == 0);
	state->cc.s = ((res & 0x80) == 0x80);
	state->cc.p = calculateParity((res & 0xff), 8);
	state->cc.cy = (res > 0xff);
	state->pc++;
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

// DCR (decrement register)
void dcr(CPUState *state, uint8_t *reg, unsigned char *opcode)
{
	uint8_t res = *reg - 1;
	state->cc.z = (res == 0);
	state->cc.s = ((res & 0x80) == 0x80);
	state->cc.p = calculateParity(res, 8);
	*reg = res;
}
