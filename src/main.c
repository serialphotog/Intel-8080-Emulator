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
