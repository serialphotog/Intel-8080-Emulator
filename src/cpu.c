#include "cpu.h"
#include "decoder.h"

#include <stdio.h>
#include <stdlib.h>

// Initializes the CPU's state
CPUState* InitCPUState() 
{
	CPUState *state = calloc(1, sizeof(CPUState));
	state->memory = malloc(0x10000); // Allocate 16K of memory
	return state;
}

// Run the fetch execute cycle
void runCPU(CPUState *state)
{
	int complete = 0;

	while (complete == 0)
	{
		complete = decode(state);
	}
}

// Read a binary file into memory
void loadFileIntoMemoryAtOffset(CPUState *state, char *file, uint32_t offset)
{
	FILE *f = fopen(file, "rb");
	if (f == NULL) 
	{
		printf("[ERROR]: Couldn't open %s\n", file);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	uint8_t *buffer = &state->memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);
}

// Performs a MVI B, D8 Instruction
// B <- byte 2
void mvi_b(CPUState *state, unsigned char *opcode)
{
	state->b = opcode[1];
	state->pc++;
}

// Performs a MVI instruction
void mvi(uint8_t *reg, uint16_t *pc, unsigned char *opcode)
{
	*reg = opcode[1];
	*pc++;
}

// JMP adr
void jmp(CPUState *state, unsigned char *opcode) 
{
	state->pc = (opcode[2] << 8) | opcode[1];
}

// LXI SP, D16
// SP.hi <- byte 3, SP.lo <- byte 2
void lxi_sp(CPUState *state, unsigned char *opcode)
{
	state->sp = (opcode[2] << 8) | opcode[1];
	state->pc += 2;
}
