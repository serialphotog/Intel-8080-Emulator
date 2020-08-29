/*******************************************************************************
 * File: branch.h
 *
 * Purpose:
 *		Specification of the various branching statements supported by the CPU.
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

#ifndef __EMU_BRANCH_H__
#define __EMU_BRANCH_H__

#include "sys/cpu.h"

#include <stdint.h>

 /**
  * Performs a JMP (unconditional jump) operation.
  *
  * RTN:
  *		PC <- addr
  */
void jmp(CPUState *state, unsigned char *opcode);

/**
 * Performs a JNZ (jump on no zero) operation.
 *
 * RTN:
 *		if Z != 0 then
 *			PC <- addr
 */
void jnz(CPUState *state, unsigned char *opcode);

/**
 * Performs a CALL (unconditional call) operation.
 *
 * RTN:
 *		(SP - 1) <- PC.hi
 *		(SP - 2) <- PC.lo
 *		SP <- SP + 2
 *		PC <- addr
 */
void call(CPUState *state, unsigned char *opcode);

/**
 * Performs a RET (return) instruction.
 *
 * RTN:
 *		PC.lo <- (SP)
 *		PC.hi <- (SP + 1)
 *		SP <- SP + 2
 */
void ret(CPUState *state);

/**
 * Performs a RZ (return if zero) instruction
 *
 * RTN:
 *		If Z, RET
 */
void rz(CPUState *state);

#endif