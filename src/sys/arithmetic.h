/*******************************************************************************
 * File: arithmetic.h
 *
 * Purpose:
 *		Specification for the arithmetic operations performed by the CPU.
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

#pragma once

#include "sys/cpu.h"

#include <stdint.h>

 /**
  * Performs an ADI (add immediate) instruction.
  *
  * Adds an immediate value to the A register.
  *
  * RTN:
  *		A <- A + D8
  *
  * FLAGS:
  *		Zero (Z), Sign (S), Parity (P), Carry (CY), Auxilary Carry (AC)
  */
void adi(CPUState *state, unsigned char *opcode);

/**
 * Performs a DAD (direct add) instruction.
 *
 * Adds the values of two registers to the H & L registers.
 *
 * RTN:
 *		HL = HL + hilo
 *
 * FLAGS:
 *		Carry (CY)
 */
void dad(uint8_t *h, uint8_t *l, uint8_t *hi, uint8_t *lo, CPUState *state);

/**
 * Performs a DAD (direct add) instruction with the H register.
 * This is implemented as a seperate method, since it's operation is slightly
 * different from the base DAD instruction.
 *
 * RTN:
 *		HL = HL + HI
 *
 * FLAGS:
 *		Carry (CY)
 */
void dad_h(uint8_t *h, uint8_t *l, CPUState *state);

/**
 * Performs an INX instruction.
 *
 * Increments a register pair.
 *
 * RTN:
 *		REG1Reg2 <- Reg1Reg2 + 1
 */
void inx(uint8_t *reg1, uint8_t *reg2);

/**
 * Performs a DCR (decrement register) operation
 *
 * RTN:
 *		REG <- REG - 1
 *
 * FLAGS:
 *		Zero (Z), Sign (S), Parity (P), Auxilary Carry (AC)
 */
void dcr(CPUState *state, uint8_t *reg, unsigned char *opcode);
