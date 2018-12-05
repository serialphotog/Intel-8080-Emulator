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

// Reads a binary file into memory
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

// Performs a MVI instruction
//		REGSITER <- byte 2
void mvi(uint8_t *reg, uint16_t *pc, unsigned char *opcode)
{
	*reg = opcode[1];
	*pc++;
}

// Performs an MVI from memory
//		(HL) <- byte 2
void mvi_m(CPUState *state, unsigned char *opcode)
{
	// Calculate the memory offset
	uint16_t offs = (state->h << 8) | state->l;
	state->memory[offs] = opcode[1];
	state->pc++;
}

// Performs a MOV instructions
//		dest <- src
void mov_r2r(uint8_t *dest, uint8_t *src, unsigned char *opcode)
{
	*dest = *src;
}

// Performs a move from register to memory
//		(HL) <- src
void mov_r2m(uint8_t *memory, uint8_t *src, uint8_t *h, uint8_t *l, unsigned char *opcode)
{
	uint16_t offs = (*h << 8) | *l;
	memory[offs] = *src;
}

// performs a move from memory to register
//		dest <- HL
void mov_m2r(uint8_t *memory, uint8_t *dest, uint8_t *h, uint8_t *l, unsigned char *opcode)
{
	uint16_t offs = (*h << 8) | *l;
	*dest = memory[offs];
}

// JMP adr
//		PC <- addr
void jmp(CPUState *state, unsigned char *opcode) 
{
	state->pc = (opcode[2] << 8) | opcode[1];
}

// Performs an LXI instruction on a 16-bit register
//		REGISTER.hi <- byte 3
//		REGISTER.lo <- byte 2
void lxi_16(uint16_t *reg, uint16_t *pc, unsigned char * opcode)
{
	*reg = (opcode[2] << 8) | opcode[1];
	*pc += 2;
}

// Performs an LXI instruction on a 8-bit register
//		REGISTER.hi <- byte 3
//		REGISTER.lo <- byte 2
void lxi_8(uint8_t *reg, uint16_t *pc, unsigned char * opcode)
{
	*reg = (opcode[2] << 8) | opcode[1];
	*pc += 2;
}

// Performs a CALL instruction
// (SP-1) <- PC.hi
// (SP-2) <- PC.lo
// SP <- SP + 2
// PC = adr
void call(CPUState *state, unsigned char *opcode)
{
	uint16_t ret = state->pc + 2;
	state->memory[state->sp - 1] = (ret >> 8) & 0xff;
	state->memory[state->sp - 2] = (ret & 0xff);
	state->sp = state->sp - 2;
	state->pc = (opcode[2] << 8) | opcode[1];
}

// Performs a LDAX instruction
//		REGISTER <- (*reg << 8 | src)
void ldax(uint8_t *reg, uint8_t *src, uint8_t *memory, unsigned char *opcode)
{
	uint16_t offs = (*reg << 8) | *src;
	*reg = memory[offs];
}
