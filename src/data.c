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
	setMemoryOffset(memory, buildMemoryOffset(*h, *l), *src);
}

// MOV from memory to register
void mov_m2r(uint8_t *memory, uint8_t *dest, uint8_t *h, uint8_t *l)
{
	*dest = fetchFromMemory(memory, buildMemoryOffset(*h, *l));
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
	setMemoryOffset(state->memory, buildMemoryOffset(state->h, state->l),
		opcode[1]);
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
	*reg = build2ByteValue(opcode[2], opcode[1]);
	*pc += 2;
}

// LDA 
void lda(CPUState *state, unsigned char *opcode)
{
	state->a = fetchFromMemory(state->memory,
		buildMemoryOffset(opcode[2], opcode[1]));
	state->pc += 2;
}

// LDAX (load a indirect)
void ldax(uint8_t *a, uint8_t *hi, uint8_t *lo, uint8_t *memory,
	unsigned char *opcode)
{
	*a = fetchFromMemory(memory, buildMemoryOffset(*hi, *lo));
}

// STA (store a direct)
void sta(CPUState *state, unsigned char *opcode)
{
	setMemoryOffset(state->memory, buildMemoryOffset(opcode[2], opcode[1]),
		state->a);
	state->pc += 2;
}

// PUSH 
void push(uint8_t *hi, uint8_t *lo, uint16_t *sp, uint8_t *memory)
{
	setMemoryOffset(memory, *sp - 2, *hi);
	setMemoryOffset(memory, *sp - 1, *lo);
	*sp -= 2;
}

// PUSH PSW
void push_psw(CPUState *state)
{
	setMemoryOffset(state->memory, state->sp - 1, state->a);
	setMemoryOffset(state->memory, state->sp - 2, encodeFlags(state));
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
	decodeFlags(state, fetchFromMemory(state->memory, state->sp));
	state->sp += 2;
}

// XCHG (exchange)
void xchg(CPUState *state)
{
	swapRegisters(&state->h, &state->d);
	swapRegisters(&state->l, &state->e);
}
