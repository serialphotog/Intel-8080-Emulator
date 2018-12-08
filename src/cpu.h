/*******************************************************************************
 * File: cpu.h
 *
 * Purpose:
 *		Specification for the base data structures and operations supported by 
 *		the CPU.
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

#ifndef __CPU_H__
#define __CPU_H__

#include <stdint.h>

// Conditional codes
typedef struct ConditionCodes {
	uint8_t	z:1;
	uint8_t s:1;
	uint8_t p:1;
	uint8_t cy:1;
	uint8_t ac:1;
	uint8_t pad:3;
} ConditionCodes;

// Tracks the current state of the CPU
typedef struct CPUState {
	uint8_t	a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint16_t sp;
	uint16_t pc;
	uint8_t *memory;
	struct ConditionCodes cc;
	uint8_t int_enable;
} CPUState;

// Initializes the state for the CPU
CPUState* InitCPUState();

// Run the fetch execute cycle
void runCPU(CPUState *state);

// Read a binary file into memory
void loadFileIntoMemoryAtOffset(CPUState *state, char *file, uint32_t offset);

// Calculates the parity of a number
int calculateParity(int num, int size);

#endif
