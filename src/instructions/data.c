/*******************************************************************************
 * File: data.c
 *
 * Purpose:
 *		The various data control operations performed by the CPU. These include
 *		instructions that move data between registers and memory.
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

#include "data.h"

// MOV between two registers
void mov_r2r(uint8_t *dest, uint8_t *src)
{
	*dest = *src;
}

// MOV from register to memory
void mov_r2m(uint8_t *memory, uint8_t *src, uint8_t *h, uint8_t *l)
{
	uint16_t offs = (*h << 8) | *l;
	memory[offs] = *src;
}

// MOV from memory to register
void mov_m2r(uint8_t *memory, uint8_t *dest, uint8_t *h, uint8_t *l) 
{
	uint16_t offs = (*h << 8) | *l;
	*dest = fetchFromMemory(memory, offs);
}

// MVI (move immediate) to register
void mvi(uint8_t *reg, uint16_t *pc, unsigned char *opcode)
{
	*reg = opcode[1];
	(*pc)++;
}

// MVI (move immediate) to memory
void mvi_m(CPUState *state, unsigned char *opcode)
{
	uint16_t offs = (state->h << 8) | state-> l;
	state->memory[offs] = opcode[1];
	state->pc++;
}

// LXI (load immediate)
void lxi(uint8_t *hi, uint8_t *lo, uint16_t *pc, unsigned char *opcode)
{
	*hi = opcode[2];
	*lo = opcode[1];
	*pc += 2;
}

// LXI (load immediate) for 16-bit register pairs
void lxi_16(uint16_t *reg, uint16_t *pc, unsigned char *opcode)
{
	*reg = (opcode[2] << 8) | opcode[1];
	*pc += 2;
}

// LDAX (load a indirect)
void ldax(uint8_t *a, uint8_t *hi, uint8_t *lo, uint8_t *memory,
	unsigned char *opcode)
{
	uint16_t offs = (*hi << 8) | *lo;
	*a = fetchFromMemory(memory, offs);
}

// PUSH 
void push(uint8_t *hi, uint8_t *lo, uint16_t *sp, uint8_t *memory)
{
	memory[*sp - 2] = *hi;
	memory[*sp - 1] = *lo;
	*sp -= 2;
}

// PUSH PSW
void push_psw(CPUState *state)
{
	state->memory[state->sp - 1] = state->a;
	uint8_t flags = (
		state->cc.z |
		state->cc.s << 1 |
		state->cc.p << 2 |
		state->cc.cy << 3 |
		state->cc.ac << 4
	);
	state->memory[state->sp - 2] = flags;
	state->sp -= 2;
}

// POP
void pop(uint8_t *hi, uint8_t *lo, uint16_t *sp, uint8_t *memory)
{
	*lo = fetchFromMemory(memory, *sp);
	*hi = fetchFromMemory(memory, *sp + 1);
	*sp += 2;
}

// POP PSW
void pop_psw(CPUState *state)
{
	state->a = fetchFromMemory(state->memory, state->sp + 1);
	uint8_t flags = fetchFromMemory(state->memory, state->sp);
	state->cc.z = ((flags & 0x01) == 0x01);
	state->cc.s = ((flags & 0x02) == 0x02);
	state->cc.p = ((flags & 0x04) == 0x04);
	state->cc.cy = ((flags & 0x08) == 0x08);
	state->cc.ac = ((flags & 0x10) == 0x10);
	state->sp += 2;
}

// XCHG (exchange)
void xchg(CPUState *state)
{
	uint8_t tmp = state->h;
	state->h = state->d;
	state->d = tmp;
	tmp = state->l;
	state->l = state->e;
	state->e = tmp;
}
