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
#include "display.h"

#include "time.h"

#include <stdio.h>
#include <Windows.h>

// Gets the current time in milliseconds
double getTimeMilliseconds()
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return ((double)currentTime.tv_sec * 1E6) + ((double)currentTime.tv_usec);
}

// Runs the CPU run thread
DWORD WINAPI emulatorThreadFunc(void *data)
{
	// Initialize the CPU state
	CPUState *state = (CPUState*)data;

	int complete = 0;
	double timeNow;
	int interrupt = 1;
	double previousInterrupt = 0.0;

	while (complete == 0) 
	{
		complete = runCPUCycle(state);
		timeNow = getTimeMilliseconds();

		// Handle interrupts
		if (timeNow - previousInterrupt > 16667) // if 1/60 of a second has elapsed
		{
			// We only want to perform an interrupt when they are enabled
			if (state->int_enable)
			{
				if (interrupt == 1)
					interrupt = 2;
				if (interrupt == 2)
					interrupt = 1;
				raiseInterrupt(state, 2);
				previousInterrupt = timeNow;
			}
		}
	}

	return 0;
}

// Let's do this!
int main(int argc, char **argv)
{
	// Initialize the CPU state
	CPUState *state = InitCPUState();

	// Load test code into memory
	loadFileIntoMemoryAtOffset(state, "../invaders.h", 0);
	loadFileIntoMemoryAtOffset(state, "../invaders.g", 0x800);
	loadFileIntoMemoryAtOffset(state, "../invaders.f", 0x1000);
	loadFileIntoMemoryAtOffset(state, "../invaders.e", 0x1800);

	// Do the run thread
	HANDLE runThread = CreateThread(NULL, 0, emulatorThreadFunc, state, 0, NULL);

	// Setup the display
	DisplayState *displayState = initDisplay();
	runDisplay(displayState);

	if (runThread) 
	{
		WaitForSingleObject(runThread, INFINITE);
	}

	// Perform cleanup
	destroyDisplay(displayState);

	return 0;
}
