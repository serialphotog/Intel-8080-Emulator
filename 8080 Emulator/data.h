/*******************************************************************************
 * File: data.h
 *
 * Purpose:
 *		Specification for the data control operations performed by the CPU.
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

#ifndef __DATA_H__
#define __DATA_H__

#include "cpu.h"

#include <stdint.h>

 /**
  * Performs a MOV (move) instruction between two registers.
  *
  * RTN:
  *		dest <- src
  */
void mov_r2r(uint8_t *dest, uint8_t *src);

/**
 * Performs a MOV (move) from a register to memory.
 *
 * RTN:
 *		(HL) <- src
 */
void mov_r2m(uint8_t *memory, uint8_t *src, uint8_t *h, uint8_t *l);

/**
 * Performs a MOV (move) from memory to a register
 *
 * RTN:
 *		dest <= (HL)
 */
void mov_m2r(uint8_t *memory, uint8_t *dest, uint8_t *h, uint8_t *l);

/**
 * Performs an MVI (move immediate) into a register
 *
 * RTN:
 *		REGISTER <- byte 2
 */
void mvi(uint8_t *reg, uint16_t *pc, unsigned char *opcode);

/**
 * Performs an MVI (move immediate) into memory
 *
 * RTN:
 *		(HL) <- byte 2
 */
void mvi_m(CPUState *state, unsigned char *opcode);

/**
 * Performs a LXI (load imediate) into a register pair
 *
 * RTN:
 *		hi <- byte 3
 *		lo <- byte 2
 */
void lxi(uint8_t *hi, uint8_t *lo, uint16_t *pc, unsigned char *opcode);

/**
 * Performs a LXI (load immediate) for 16-bit register pairs.
 *
 * RTN:
 *		REGISTER.hi <- byte 3
 *		REGISTER.lo <- byte 2
 */
void lxi_16(uint16_t *reg, uint16_t *pc, unsigned char *opcode);

/**
 * Performs a LDA (load address) instruction.
 *
 * Loads the value from memory into A.
 *
 * RTN:
 *		A <- (addr)
 */
void lda(CPUState *state, unsigned char *opcode);

/**
 * Performs a LDAX (load a indirect) operation.
 *
 * RTN:
 *		A <- (*hi << 8 | lo)
 */
void ldax(uint8_t *a, uint8_t *hi, uint8_t *lo, uint8_t *memory,
	unsigned char *opcode);

/**
 * Performs a STA (store a direct) operation.
 *
 * RTN:
 *		(addr) <- A
 */
void sta(CPUState *state, unsigned char *opcode);

/**
 * Performs a PUSH instruction.
 *
 * Pushes a value onto the stack
 *
 * RTN:
 *		(SP - 2) <- hi
 *		(SP - 1) <- lo
 *		SP <- SP + 2
 */
void push(uint8_t *hi, uint8_t *lo, uint16_t *sp, uint8_t *memory);

/**
 * Performs a PUSH PSW instruction.
 * This is implemented as a seperate instruction, since it functions slightly
 * different from the base PUSH instruction.
 *
 * RTN:
 *		(SP - 2) <- flags
 *		(SP - 1) <- A
 *		SP <- SP - 2
 */
void push_psw(CPUState *state);

/**
 * Performs a POP instruction.
 *
 * Pops a value from the stack and stores the value into a register pair.
 *
 * RTN:
 *		hi <- (SP + 1)
 *		lo <- (SP)
 *		SP <- SP + 2
 */
void pop(uint8_t *hi, uint8_t *lo, uint16_t *sp, uint8_t *memory);

/**
 * Performs a POP PSW instruction.
 * This is implemented as a seperate instruction, since it functions slightly
 * different from the base POP instruction.
 *
 * RTN:
 *		FLAGS <- (SP)
 *		A <- (SP + 1)
 *		SP <- SP + 2
 */
void pop_psw(CPUState *state);

/**
 * Performs an XCHG (exchange) instruction.
 *
 * Exchanges the D & E register and H & L register
 *
 * RTN:
 *		H <-> D
 *		L <-> E
 */
void xchg(CPUState *state);

#endif
