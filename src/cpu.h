#ifndef __CPU_H__
#define __CPU_H__

#include <stdint.h>

// Conditional codes
typedef struct ConditionCodes {
	uint8_t	z:1;
	uint8_t s:1;
	uint8_t p:1;
	uint8_t cy:1;
	uint8_t ac:1;
	uint8_t pad:3;
} ConditionCodes;

// Tracks the current state of the CPU
typedef struct CPUState {
	uint8_t	a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint16_t sp;
	uint16_t pc;
	uint8_t *memory;
	struct ConditionCodes cc;
	uint8_t int_enable;
} CPUState;

// Initializes the state for the CPU
CPUState* InitCPUState();

// Run the fetch execute cycle
void runCPU(CPUState *state);

// Read a binary file into memory
void loadFileIntoMemoryAtOffset(CPUState *state, char *file, uint32_t offset);

// Performs a JMP operation
void jmp(CPUState *state, unsigned char *opcode);

// perfroms an lxi instruction on the SP register
void lxi_sp(CPUState *state, unsigned char *opcode);

#endif
