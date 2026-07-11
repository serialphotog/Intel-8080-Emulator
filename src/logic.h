/*******************************************************************************
 * File: logic.h
 *
 * Purpose:
 *		Specification for the various logical operations supported by the CPU.
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

#pragma once

#include "cpu.h"

#include <stdint.h>

 /**
  * Performs an ANA (and a) instruction.
  *
  * Ands a register with the value in A.
  *
  * RTN:
  *		A <- A & REGISTER
  *
  * FLAGS:
  *		Zero (Z), Sign (S), Parity (P), Carry (CY), Auxilary Carry (AC)
  */
void ana(CPUState *state, uint8_t *reg);

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
 * Performs an XRA (xor with a) operationl
 *
 * XORs a register with the A register.
 *
 * RTN:
 *		A <- A ^ REGISTER
 *
 * FLAGS:
 *		Zero (Z), Sign (S), Parity (P), Carry (CY), Auxilary Carry (AC)
 */
void xra(CPUState *state, uint8_t *reg);

/** ORs an 8-bit value into A and updates the logical-operation flags. */
void ora(CPUState *state, uint8_t value);

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

/** Compares A with a value by setting subtraction flags without changing A. */
void cmp(CPUState *state, uint8_t value);

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

/** Rotates A left, copying bit 7 to bit 0 and Carry. */
void rlc(CPUState *state);

/** Rotates A left through Carry. */
void ral(CPUState *state);

/** Rotates A right through Carry. */
void rar(CPUState *state);
