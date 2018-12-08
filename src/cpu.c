/*******************************************************************************
 * File: cpu.c
 *
 * Purpose:
 *		The data structures and operations used by the CPU.
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

#include "cpu.h"

#include "decoder.h"
#include "disasm.h"

#include <stdio.h>
#include <stdlib.h>

// Initializes the CPU's state
CPUState* InitCPUState() 
{
	CPUState *state = calloc(1, sizeof(CPUState));
	state->memory = malloc(0x10000); // Allocate 16K of memory
	return state;
}

// Run the fetch execute cycle
void runCPU(CPUState *state)
{
	int complete = 0;

	while (complete == 0)
	{
		printf("\t");
		disassembleInstruction(state->memory, state->pc);

		complete = decode(state);

		// Print debug info
		printDebug(state);
	}
}

// Prints debug to console
void printDebug(CPUState *state)
{
	printf("\t");
	printf("%c", state->cc.z ? 'z' : '.');
	printf("%c", state->cc.s ? 's' : '.');
	printf("%c", state->cc.p ? 'p' : '.');
	printf("%c", state->cc.cy ? 'c' : '.');
	printf("%c", state->cc.ac ? 'a' : '.');
	printf(" A $%02x B $%02x C %02x D $%02x E $%02x H $%02x L $%02x SP %04x\n",
		state->a, state->b, state->c, state->d, state->e, state->h, state->l, 
		state->sp);
}

// Swaps register values
void swapRegisters(uint8_t *reg1, uint8_t *reg2)
{
	uint8_t tmp = *reg1;
	*reg1 = *reg2;
	*reg2 = tmp;
}

// Builds a memory offset
uint16_t buildMemoryOffset(uint8_t hi, uint8_t lo)
{
	uint16_t offs = (hi << 8) | lo;
	return offs;
}

// Retrieves a value from memory
uint8_t fetchFromMemory(uint8_t *memory, uint16_t offs)
{
	return memory[offs];
}

// Sets a memory offset to a value
void setMemoryOffset(uint8_t *memory, uint16_t offs, uint8_t value)
{
	memory[offs] = value;
}

// Encodes the CPU flags as a bitstream
uint8_t encodeFlags(CPUState *state)
{
	uint8_t flags = (
		state->cc.z |
		state->cc.s << 1 |
		state->cc.p << 2 |
		state->cc.cy << 3 |
		state->cc.ac << 4
	);
	return flags;
}

// Decodes the flags from a bitstream
void decodeFlags(CPUState *state, uint8_t flags)
{
	state->cc.z = ((flags & 0x01) == 0x01);
	state->cc.s = ((flags & 0x02) == 0x02);
	state->cc.p = ((flags & 0x04) == 0x04);
	state->cc.cy = ((flags & 0x08) == 0x08);
	state->cc.ac = ((flags & 0x10) == 0x10);
}

// Calculates the parity of a number
// Parity is one if the number of one bits is even.
int calculateParity(int num, int size)
{
	int i;
	int parity = 0;

	num = (num & ((1 << size) - 1));
	for (i = 0; i < size; i++)
	{
		if (num & 0x1)
			parity++;
		num = num >> 1;
	}

	return ((parity & 0x1) == 0);
}

// Reads a binary file into memory
void loadFileIntoMemoryAtOffset(CPUState *state, char *file, uint32_t offset)
{
	FILE *f = fopen(file, "rb");
	if (f == NULL) 
	{
		printf("[ERROR]: Couldn't open %s\n", file);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	uint8_t *buffer = &state->memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);
}
