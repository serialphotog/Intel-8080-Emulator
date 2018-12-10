/*******************************************************************************
 * File: arithmetic.h
 *
 * Purpose:
 *		A software emulator for the Intel 8080 CPU.
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT .IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ******************************************************************************/

#include "cpu.h"

#include <stdio.h>

int main(int argc, char **argv)
{
	// Initialize the CPU state
	CPUState *state = InitCPUState();

	// Load test code into memory
	loadFileIntoMemoryAtOffset(state, "../invaders.h", 0);
	loadFileIntoMemoryAtOffset(state, "../invaders.g", 0x800);
	loadFileIntoMemoryAtOffset(state, "../invaders.f", 0x1000);
	loadFileIntoMemoryAtOffset(state, "../invaders.e", 0x1800);

	runCPU(state);

	return 0;
}
