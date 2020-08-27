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

#ifndef __EMU_CPU_H__
#define __EMU_CPU_H__

#include <stdint.h>

 // The CPU Flags
typedef struct ConditionCodes {
	uint8_t	z : 1; // Zero 
	uint8_t s : 1; // Sign
	uint8_t p : 1; // Parity
	uint8_t cy : 1; // Carry
	uint8_t ac : 1; // Auxilary Carry
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
	struct ConditionCodes cc; // CPU Flags
	uint8_t int_enable; // Tracks if interrupts are enables or disabled
} CPUState;

/**
 * Initializes the CPU state.
 */
CPUState* InitCPUState();

/**
 * Runs the CPU's fetch-execute cycle.
 */
int runCPUCycle(CPUState *state);

/**
 * Raises an interrupt within the CPU
*/
void raiseInterrupt(CPUState *state, int interruptCode);

/**
 * Prints the debug info to the termianl.
 */
void printDebug(CPUState *state);

/**
 * Reads a binary file into memory at a given offset.
 */
void loadFileIntoMemoryAtOffset(CPUState *state, char *file, uint32_t offset);

/**
 * Sets the flags based on the value of A
 */
void setFlagsFromA(CPUState *state);

/**
 * Swaps the values in two registers.
 */
void swapRegisters(uint8_t *reg1, uint8_t *reg2);

/**
 * Builds a 2-byte value
 */
uint16_t build2ByteValue(uint8_t hi, uint8_t lo);

/**
 * Builds a memory offset
 */
uint16_t buildMemoryOffset(uint8_t hi, uint8_t lo);

/**
 * Retrieves a value from memory.
 */
uint8_t fetchFromMemory(uint8_t *memory, uint16_t offs);

/**
 * Sets a value in memory at offset.
 */
void setMemoryOffset(uint8_t *memory, uint16_t offs, uint8_t value);

/**
 * Encodes the CPU flags into a bitstream.
 */
uint8_t encodeFlags(CPUState *state);

/**
 * Decodes the CPU flags from a bitstream.
 */
void decodeFlags(CPUState *state, uint8_t flags);

/**
 * Calculates the parity bit of a number.
 */
int calculateParity(int num, int size);

#endif