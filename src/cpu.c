/*******************************************************************************
 * File: cpu.c
 *
 * Purpose:
 *		The data structures and operations used by the CPU.
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

#define _CRT_SECURE_NO_WARNINGS // Stop visual studio from complaining about insecure functions

#include "cpu.h"

#include "decoder.h"
#include "disasm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // Initializes the CPU's state
CPUState* InitCPUState()
{
	CPUState *state = calloc(1, sizeof(CPUState));
	/* Real RAM powers up undefined, but exposing host heap contents as video
	 * produces nondeterministic garbage and can turn stray execution into
	 * arbitrary opcodes. Start the emulated address space deterministically. */
	state->memory = calloc(0x10000, sizeof(uint8_t));
	state->input_ports[0] = 0x0e;
	state->input_ports[1] = 0x08;
	state->running = 1;
	return state;
}

// Run the fetch execute cycle
int runCPUCycle(CPUState *state)
{
	if (state->halted)
		return 0;
	int complete = decode(state);

	return complete;
}

// Raises an interrupt
void raiseInterrupt(CPUState *state, int interruptCode)
{
	state->halted = 0;
	// Push PC to the stack
	state->memory[state->sp - 1] = ((state->pc & 0xff00) >> 8);
	state->memory[state->sp - 2] = (state->pc & 0xff);
	state->sp -= 2;

	// Set PC to the interrupt handler
	state->pc = 8 * interruptCode;
	state->int_enable = 0;
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

// Sets the CPU flags based on the value of the A register
void setFlagsFromA(CPUState *state)
{
	state->cc.cy = state->cc.ac = 0;
	state->cc.z = (state->a == 0);
	state->cc.s = ((state->a & 0x80) == 0x80);
	state->cc.p = calculateParity(state->a, 8);
}

// Swaps register values
void swapRegisters(uint8_t *reg1, uint8_t *reg2)
{
	uint8_t tmp = *reg1;
	*reg1 = *reg2;
	*reg2 = tmp;
}

// Builds a 2-byte value
uint16_t build2ByteValue(uint8_t hi, uint8_t lo)
{
	uint16_t ret = (hi << 8) | lo;
	return ret;
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
	uint8_t flags = (uint8_t)(
		(state->cc.s << 7) |
		(state->cc.z << 6) |
		(state->cc.ac << 4) |
		(state->cc.p << 2) |
		0x02 |
		state->cc.cy);
	return flags;
}

// Decodes the flags from a bitstream
void decodeFlags(CPUState *state, uint8_t flags)
{
	state->cc.s = ((flags & 0x80) != 0);
	state->cc.z = ((flags & 0x40) != 0);
	state->cc.ac = ((flags & 0x10) != 0);
	state->cc.p = ((flags & 0x04) == 0x04);
	state->cc.cy = ((flags & 0x01) != 0);
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

	uint8_t *file_data = calloc((size_t)fsize + 1, 1);
	if (!file_data || fread(file_data, 1, (size_t)fsize, f) != (size_t)fsize)
	{
		printf("[ERROR]: Couldn't read %s\n", file);
		free(file_data);
		fclose(f);
		exit(1);
	}
	fclose(f);

	/* Some distributed ROM sets wrap their bytes in a JavaScript-style
	 * {"bytes": [0x00, ...]} object. Accept that representation as well as
	 * conventional raw binary dumps. */
	if (fsize > 0 && file_data[0] == '{' && strstr((char *)file_data, "\"bytes\"") != NULL)
	{
		char *cursor = (char *)file_data;
		uint32_t address = offset;
		while (cursor != NULL)
		{
			char *comment = strstr(cursor, "//");
			char *token = strstr(cursor, "0x");
			if (!token)
				break;
			if (comment && comment < token)
			{
				cursor = strchr(comment, '\n');
				continue;
			}
			cursor = token;
			char *end;
			unsigned long value = strtoul(cursor + 2, &end, 16);
			if (end == cursor + 2)
			{
				printf("[ERROR]: Invalid byte array in %s\n", file);
				free(file_data);
				exit(1);
			}
			if (value > 0xff)
				break;
			if (address >= 0x10000)
			{
				printf("[ERROR]: Byte array in %s is too large\n", file);
				free(file_data);
				exit(1);
			}
			state->memory[address++] = (uint8_t)value;
			cursor = end;
		}
	}
	else
	{
		if ((uint32_t)fsize > 0x10000 - offset)
		{
			printf("[ERROR]: %s does not fit in emulated memory\n", file);
			free(file_data);
			exit(1);
		}
		memcpy(&state->memory[offset], file_data, (size_t)fsize);
	}
	free(file_data);
}
