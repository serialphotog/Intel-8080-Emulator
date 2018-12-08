#include "cpu.h"
#include "decoder.h"
#include "disasm.h"

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
		printf("\t");
		disassembleInstruction(state->memory, state->pc);

		complete = decode(state);

		// Print debug info
		printf("\t");
		printf("%c", state->cc.z ? 'z' : '.');
		printf("%c", state->cc.s ? 's' : '.');
		printf("%c", state->cc.p ? 'p' : '.');
		printf("%c", state->cc.cy ? 'c' : '.');
		printf("%c", state->cc.ac ? 'a' : '.');
		printf(" A $%02x B $%02x C %02x D $%02x E $%02x H $%02x L $%02x SP %04x\n",
			state->a, state->b, state->c, state->d, state->e, state->h, state->l, state->sp);
	}
}

// Calculates the parity of a number
// Parity is one if the number of one bits is even.
int calculateParity(int num, int size)
{
	int i;
	int parity = 0;

	num = (num & ((1 << size) - 1));
	for (i = 0; i < size; i++)
	{
		if (num & 0x1)
			parity++;
		num = num >> 1;
	}

	return ((parity & 0x1) == 0);
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
	(*pc)++;
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

// JNZ addr
//		if Z != 0 then
//			PC <- addr
void jnz(CPUState *state, unsigned char *opcode)
{
	if (state->cc.z == 0) 
	{
		state->pc = (opcode[2] << 8) | opcode[1];
	} 
	else 
	{
		state->pc += 2;
	}
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
//		hi <- byte 3
// 		lo <- byte 2
void lxi_8(uint8_t *hi, uint8_t *lo, uint16_t *pc, unsigned char * opcode)
{
	*hi = opcode[2];
	*lo = opcode[1];
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
//		A <- (*hi << 8 | lo)
void ldax(uint8_t *a, uint8_t *hi, uint8_t *lo, uint8_t *memory, unsigned char *opcode)
{
	uint16_t offs = (*hi << 8) | *lo;
	*a = memory[offs];
}

// Performs a INX instruction on a normal register
//		Reg1Reg2 <- Reg1Reg2 + 1
void inx(uint8_t *reg1, uint8_t *reg2, unsigned char *opcode)
{
	(*reg2)++;
	if ((*reg2) == 0)
		(*reg1)++;
}

// Performs a DCR instruction
//		REG <- REG - 1
//		FLAGS: Z, S, P, AC
void dcr(CPUState *state, uint8_t *reg, unsigned char *opcode)
{
	uint8_t res = (*reg) - 1;
	state->cc.z = (res == 0);
	state->cc.s = ((res & 0x80) == 0x80);
	state->cc.p = calculateParity(res, 8);
	*reg = res;
}

// Performs a RET instruction
//		PC.lo <- (sp)
//		PC.hi <- (sp+1)
//		SP <- SP+2
void ret(CPUState *state)
{
	state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
	state->sp += 2;
}

// Performs a CPI instruction
//		A - data
//		FLAGS: Z, S, P, CY, AC
void cpi(CPUState *state, unsigned char *opcode)
{
	uint8_t res = state->a - opcode[1];
	state->cc.z = (res == 0);
	state->cc.s = ((res & 0x80) == 0x80);
	state->cc.p = calculateParity(res, 8);
	state->cc.cy = (state->a < opcode[1]);
	state->pc++;
}

// Performs a push instruction
//		(SP-2) <- hi
//		(SP-1) <- lo
//		SP <- SP + 2
void push(uint8_t *hi, uint8_t *lo, uint16_t *sp, uint8_t *memory)
{
	memory[(*sp)-2] = *hi;
	memory[(*sp)-1] = *lo;
	(*sp) -= 2;
}

// Performs a push PSW
//		(SP-2) <- flags
//		(SP-1) <- A
//		SP <- SP - 2
void push_psw(CPUState *state)
{
	state->memory[state->sp-1] = state->a;
	uint8_t flags = (
		state->cc.z |
		state->cc.s << 1 |
		state->cc.p << 2 |
		state->cc.cy << 3 | 
		state->cc.ac << 4
	);
	state->memory[state->sp-2] = flags;
	state->sp -= 2;
}

// Performs a POP
//		hi <- (sp+1)
//		lo <- (sp)
//		sp <- sp + 2
void pop(uint8_t *hi, uint8_t *lo, uint16_t *sp, uint8_t *memory)
{
	*lo = memory[*sp];
	*hi = memory[(*sp)+1];
	(*sp) += 2;
}

// Performs a POP PSW
//		FLAGS <- (sp)
//		A <- (sp+1)
//		SP <- SP + 2
void pop_psw(CPUState *state)
{
	state->a = state->memory[state->sp+1];
	uint8_t flags = state->memory[state->sp];
	state->cc.z = ((flags & 0x01) == 0x01);
	state->cc.s = ((flags & 0x02) == 0x02);
	state->cc.p = ((flags & 0x04) == 0x04);
	state->cc.cy = ((flags & 0x08) == 0x08);
	state->cc.ac = ((flags & 0x10) == 0x10);
	state->sp += 2;
}

// Performs a DAD H instruction
//		HL = HL + HI
void dad_h(uint8_t *h, uint8_t *l, CPUState *state)
{
	uint32_t hl = ((*h) << 8) | *l;
	uint32_t res = hl + hl;
	*h = (res & 0xff00) >> 8;
	*l = res & 0xff;
	state->cc.cy = ((res & 0xffff0000) != 0);
}

// Performs a DAD instruction
//		HL = HL + hilo 
void dad(uint8_t *h, uint8_t *l, uint8_t *hi, uint8_t *lo, CPUState *state)
{
	uint32_t hl = ((*h) << 8) | *l;
	uint32_t hilo = ((*hi) << 8) | *lo;
	uint32_t res = hl + hilo;
	*h = (res & 0xff00) >> 8;
	*l = res & 0xff;
	state->cc.cy = ((res & 0xffff0000) != 0);
}

// Performs an XCHG
//		H <-> D
//		L <-> E
void xchg(CPUState *state)
{
	uint8_t tmp = state->h;
	state->h = state->d;
	state->d = tmp;
	tmp = state->l;
	state->l = state->e;
	state->e = tmp;
}

// Performs an OUT instruction
// TOOD: I have no idea how to implement this currently...
void out(CPUState *state)
{
	state->pc++;
}

// Performs an RRC instruction
//		A = A >> 1
//		bit 7 = previous bit
//		CY = previous bit 0
//		FLAGS: CY
void rrc(CPUState *state)
{
	uint8_t prevA = state->a;
	state->a = ((prevA & 0x01) << 7) | (prevA >> 1);
	state->cc.cy = ((prevA & 0x01) == 0x01);
}
