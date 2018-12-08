/*******************************************************************************
 * File: logic.h
 *
 * Purpose:
 *		Specification for the various logical operations supported by the CPU.
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

#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "cpu.h"

#include <stdint.h>

/*
 * Performs an ANI (and immediate) instruction.
 *
 * ANI performs an immediate and with the A register.
 *
 * RTN:
 *		A <- A & D8
 *
 * FLAGS:
 *		Zero (Z), Sign (S), Parity (P), Carry (CY), Auxillary Carry (AC)
 */
void ani(CPUState *state, unsigned char *opcode);

/**
 * Performs a CPI (compare immediate with A) operation.
 *
 * RTN:
 *		A - data
 *
 * FLAGS:
 *		Zero (Z), Sign (S), Parity (P), Carry (CY), Auxilary Carry (AC)
 */
void cpi(CPUState *state, unsigned char *opcode);

/*
 * Performs an RRC instruction.
 *
 * RRC rotates the A register right.
 *
 * RTN:
 *		A = A >> 1
 *		bit 7 = previous bit
 *		CY = previous bit 0
 *
 * FLAGS:
 *		Carry (CY)
 */
void rrc(CPUState *state);

#endif
