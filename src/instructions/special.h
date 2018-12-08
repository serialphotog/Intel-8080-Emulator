/*******************************************************************************
 * File: special.h
 *
 * Purpose:
 *		Specification for sepcial operations performed by the CPU.
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

#ifndef __SPECIAL_H__
#define __SPECIAL_H__

#include "cpu.h"

/**
 * Performs an OUT operation.
 *
 * NOTE: I'm not currently sure how this should be implemented. This is 
 * currently just a stub implementation to allow the CPU to continue executing
 * when it encounters this instruction.
 */
void out(CPUState *state);

/**
 * Performs an EI (enable interrupts) operation.
 *
 * This mearly sets a flag in the CPU state
 */
void ei(CPUState *state);

#endif
