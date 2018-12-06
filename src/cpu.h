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

// Calculates the parity of a number
int calculateParity(int num, int size);

// Performs an MVI instruction
void mvi(uint8_t *reg, uint16_t *pc, unsigned char *opcode);

// Performs an MVI from memory
void mvi_m(CPUState *state, unsigned char *opcode);

// Performs a JMP operation
void jmp(CPUState *state, unsigned char *opcode);

// Performs a JNZ operation
void jnz(CPUState *state, unsigned char *opcode);

// Prfroms an lxi instruction on a 16-bit register
void lxi_16(uint16_t *reg, uint16_t *pc, unsigned char *opcode);

// Performs an lxi instruction on a 8-bit register.
void lxi_8(uint8_t *hi, uint8_t *lo, uint16_t *pc, unsigned char *opcode);

// Performs an LDAX instruction
void ldax(uint8_t *a, uint8_t *hi, uint8_t *lo, uint8_t *memory, unsigned char *opcode);

// Performs a CALL instruction
void call(CPUState *state, unsigned char *opcode);

// Performs a MOV from register to register
void mov_r2r(uint8_t *dest, uint8_t *src, unsigned char *opcode);

// Performs a move from register to memory
void mov_r2m(uint8_t *memory, uint8_t *src, uint8_t *h, uint8_t *l, unsigned char *opcode);

// performs a move from memory to register
void mov_m2r(uint8_t *memory, uint8_t *dest, uint8_t *h, uint8_t *l, unsigned char *opcode);

// Performs a INX instruction on a normal register
void inx(uint8_t *reg1, uint8_t *reg2, unsigned char *opcode);

// Performs a DCR instruction
void dcr(CPUState *state, uint8_t *reg, unsigned char *opcode);

#endif
