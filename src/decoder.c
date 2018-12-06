#include "cpu.h"

#include "decoder.h"
#include "disasm.h"

#include <stdio.h>
#include <stdlib.h>

// Decodes CPU instructions
int decode(CPUState *state)
{
	unsigned char *opcode = &state->memory[state->pc];
	state->pc += 1;

	switch (*opcode)
	{
		case 0x00:
		case 0x08:
		case 0x10:
		case 0x18:
		case 0x28:
		case 0x38:
		case 0xcb:
		case 0xd9:
		case 0xdd:
		case 0xed:
		case 0xfd:
			// NOP
			break;
		case 0x01:
			// LXI B, D16
			lxi_8(&state->b, &state->pc, opcode);
			break;
		case 0x02:
			// STAX B
			unimplementedInstruction(state);
			break;
		case 0x03:
			// INX B
			unimplementedInstruction(state);
			break;
		case 0x04:
			// INR B
			unimplementedInstruction(state);
			break;
		case 0x05:
			// DCR B
			unimplementedInstruction(state);
			break;
		case 0x06:
			// MVI B, D8
			mvi(&state->b, &state->pc, opcode);
			break;
		case 0x07:
			// RLC
			unimplementedInstruction(state);
			break;
		case 0x09:
			// DAD B
			unimplementedInstruction(state);
			break;
		case 0x0a:
			// LDAX B
			ldax(&state->b, &state->c, state->memory, opcode);
			break;
		case 0x0b:
			// DCX B
			unimplementedInstruction(state);
			break;
		case 0x0c:
			// INR C
			unimplementedInstruction(state);
			break;
		case 0x0d:
			// DCR C
			unimplementedInstruction(state);
			break;
		case 0x0e:
			// MVI C, D8
			unimplementedInstruction(state);
			break;
		case 0x0f:
			// RRC
			unimplementedInstruction(state);
			break;
		case 0x11:
			// LXI D, D16
			lxi_8(&state->d, &state->pc, opcode);
			break;
		case 0x12:
			// STAX D
			unimplementedInstruction(state);
			break;
		case 0x13:
			// INX D
			unimplementedInstruction(state);
			break;
		case 0x14:
			// INR D
			unimplementedInstruction(state);
			break;
		case 0x15:
			// DCR D
			unimplementedInstruction(state);
			break;
		case 0x16:
			// MVI D, D8
			mvi(&state->d, &state->pc, opcode);
			break;
		case 0x17:
			// RAL
			unimplementedInstruction(state);
			break;
		case 0x19:
			// DAD D
			unimplementedInstruction(state);
			break;
		case 0x1a:
			// LDAX D
			ldax(&state->d, &state->e, state->memory, opcode);
			break;
		case 0x1b:
			// DCX D
			unimplementedInstruction(state);
			break;
		case 0x1c:
			// INR E
			unimplementedInstruction(state);
			break;
		case 0x1d:
			// DCR E
			unimplementedInstruction(state);
			break;
		case 0x1e:
			// MVI E, D8
			mvi(&state->e, &state->pc, opcode);
			break;
		case 0x1f:
			// RAR
			unimplementedInstruction(state);
			break;
		case 0x20:
			// RIM
			unimplementedInstruction(state);
			break;
		case 0x21:
			// LXI H, D16
			lxi_8(&state->h, &state->pc, opcode);
			break;
		case 0x22:
			// SHLD adr
			unimplementedInstruction(state);
			break;
		case 0x23:
			// INX H
			unimplementedInstruction(state);
			break;
		case 0x24:
			// INR H
			unimplementedInstruction(state);
			break;
		case 0x25:
			// DCR H
			unimplementedInstruction(state);
			break;
		case 0x26:
			// MVI H, D8
			mvi(&state->h, &state->pc, opcode);
			break;
		case 0x27:
			// DAA
			unimplementedInstruction(state);
			break;
		case 0x29:
			// DAD H
			unimplementedInstruction(state);
			break;
		case 0x2a:
			// LHLD adr
			unimplementedInstruction(state);
			break;
		case 0x2b:
			// DCX H
			unimplementedInstruction(state);
			break;
		case 0x2c:
			// INR L
			unimplementedInstruction(state);
			break;
		case 0x2d:
			// DCR L
			unimplementedInstruction(state);
			break;
		case 0x2e:
			// MVI L, D8
			mvi(&state->l, &state->pc, opcode);
			break;
		case 0x2f:
			// CMA
			unimplementedInstruction(state);
			break;
		case 0x30:
			// SIM
			unimplementedInstruction(state);
			break;
		case 0x31:
			// LXI SP, D16
			lxi_16(&state->sp, &state->pc, opcode);
			break;
		case 0x32:
			// STA adr
			unimplementedInstruction(state);
			break;
		case 0x33:
			// INX SP
			unimplementedInstruction(state);
			break;
		case 0x34:
			// INR M
			unimplementedInstruction(state);
			break;
		case 0x35:
			// DCR M
			unimplementedInstruction(state);
			break;
		case 0x36:
			// MVI M, D8
			mvi_m(state, opcode);
			break;
		case 0x37:
			// STC
			unimplementedInstruction(state);
			break;
		case 0x39:
			// DAD SP
			unimplementedInstruction(state);
			break;
		case 0x3a:
			// LDA adr
			unimplementedInstruction(state);
			break;
		case 0x3b:
			// DCX SP
			unimplementedInstruction(state);
			break;
		case 0x3c:
			// INR A
			unimplementedInstruction(state);
			break;
		case 0x3d:
			// DCR A
			unimplementedInstruction(state);
			break;
		case 0x3e:
			// MVI A, D8
			mvi(&state->a, &state->pc, opcode);
			break;
		case 0x3f:
			// CMC
			unimplementedInstruction(state);
			break;
		case 0x40:
			// MOV B, B
			mov_r2r(&state->b, &state->b, opcode);
			break;
		case 0x41:
			// MOV B, C
			mov_r2r(&state->b, &state->c, opcode);
			break;
		case 0x42:
			// MOV B, D
			mov_r2r(&state->b, &state->d, opcode);
			break;
		case 0x43:
			// MOV B, E
			mov_r2r(&state->b, &state->e, opcode);
			break;
		case 0x44:
			// MOV B, H
			mov_r2r(&state->b, &state->h, opcode);
			break;
		case 0x45:
			// MOV B, L
			mov_r2r(&state->b, &state->l, opcode);
			break;
		case 0x46:
			// MOV B, M
			mov_m2r(state->memory, &state->b, &state->h, &state->l, opcode);
			break;
		case 0x47:
			// MOV B, A
			mov_r2r(&state->b, &state->a, opcode);
			break;
		case 0x48:
			// MOV C, B
			mov_r2r(&state->c, &state->b, opcode);
			break;
		case 0x49:
			// MOV C, C
			mov_r2r(&state->c, &state->c, opcode);
			break;
		case 0x4a:
			// MOV C, D
			mov_r2r(&state->c, &state->d, opcode);
			break;
		case 0x4b:
			// MOV C, E
			mov_r2r(&state->c, &state->e, opcode);
			break;
		case 0x4c:
			// MOV C, H
			mov_r2r(&state->c, &state->h, opcode);
			break;
		case 0x4d:
			// MOV C, L
			mov_r2r(&state->c, &state->l, opcode);
			break;
		case 0x4e:
			// MOV C, M
			mov_m2r(state->memory, &state->c, &state->h, &state->l, opcode);
			break;
		case 0x4f:
			// MOV C, A
			mov_r2r(&state->c, &state->a, opcode);
			break;
		case 0x50:
			// MOV D, B
			mov_r2r(&state->d, &state->b, opcode);
			break;
		case 0x51:
			// MOV D, C
			mov_r2r(&state->d, &state->c, opcode);
			break;
		case 0x52:
			// MOV D, D
			mov_r2r(&state->d, &state->d, opcode);
			break;
		case 0x53:
			// MOV D, E
			mov_r2r(&state->d, &state->e, opcode);
			break;
		case 0x54:
			// MOV D, H
			mov_r2r(&state->d, &state->h, opcode);
			break;
		case 0x55:
			// MOV D, L
			mov_r2r(&state->d, &state->l, opcode);
			break;
		case 0x56:
			// MOV D, M
			mov_m2r(state->memory, &state->d, &state->h, &state->l, opcode);
			break;
		case 0x57:
			// MOV D, A
			mov_r2r(&state->d, &state->a, opcode);
			break;
		case 0x58:
			// MOV E, B
			mov_r2r(&state->e, &state->b, opcode);
			break;
		case 0x59:
			// MOV E, C
			mov_r2r(&state->e, &state->c, opcode);
			break;
		case 0x5a:
			// MOV E, D
			mov_r2r(&state->e, &state->d, opcode);
			break;
		case 0x5b:
			// MOV E, E
			mov_r2r(&state->e, &state->e, opcode);
			break;
		case 0x5c:
			// MOV E, H
			mov_r2r(&state->e, &state->h, opcode);
			break;
		case 0x5d:
			// MOV E, L
			mov_r2r(&state->e, &state->l, opcode);
			break;
		case 0x5e:
			// MOV E, M
			mov_m2r(state->memory, &state->e, &state->h, &state->l, opcode);
			break;
		case 0x5f:
			// MOV E, A
			mov_r2r(&state->e, &state->a, opcode);
			break;
		case 0x60:
			// MOV H, B
			mov_r2r(&state->h, &state->b, opcode);
			break;
		case 0x61:
			// MOV H, C
			mov_r2r(&state->h, &state->c, opcode);
			break;
		case 0x62:
			// MOV H, D
			mov_r2r(&state->h, &state->d, opcode);
			break;
		case 0x63:
			// MOV H, E
			mov_r2r(&state->h, &state->e, opcode);
			break;
		case 0x64:
			// MOV H, H
			mov_r2r(&state->h, &state->h, opcode);
			break;
		case 0x65:
			// MOV H, L
			mov_r2r(&state->h, &state->l, opcode);
			break;
		case 0x66:
			// MOV H, M
			mov_m2r(state->memory, &state->h, &state->h, &state->l, opcode);
			break;
		case 0x67:
			// MOV H, A
			mov_r2r(&state->h, &state->a, opcode);
			break;
		case 0x68:
			// MOV L, B
			mov_r2r(&state->l, &state->b, opcode);
			break;
		case 0x69:
			// MOV L, C
			mov_r2r(&state->l, &state->c, opcode);
			break;
		case 0x6a:
			// MOV L, D
			mov_r2r(&state->l, &state->d, opcode);
			break;
		case 0x6b:
			// MOV L, E
			mov_r2r(&state->l, &state->e, opcode);
			break;
		case 0x6c:
			// MOV L, H
			mov_r2r(&state->l, &state->h, opcode);
			break;
		case 0x6d:
			// MOV L, L
			mov_r2r(&state->l, &state->l, opcode);
			break;
		case 0x6e:
			// MOV L, M
			mov_m2r(state->memory, &state->l, &state->h, &state->l, opcode);
			break;
		case 0x6f:
			// MOV L, A
			mov_r2r(&state->l, &state->a, opcode);
			break;
		case 0x70:
			// MOV M, B
			mov_m2r(state->memory, &state->b, &state->h, &state->l, opcode);
			break;
		case 0x71:
			// MOV M, C
			mov_m2r(state->memory, &state->c, &state->h, &state->l, opcode);
			break;
		case 0x72:
			// MOV M, D
			mov_m2r(state->memory, &state->d, &state->h, &state->l, opcode);
			break;
		case 0x73:
			// MOV M, E
			mov_m2r(state->memory, &state->e, &state->h, &state->l, opcode);
			break;
		case 0x74:
			// MOV M, H
			mov_m2r(state->memory, &state->h, &state->h, &state->l, opcode);
			break;
		case 0x75:
			// MOV M, L
			mov_m2r(state->memory, &state->l, &state->h, &state->l, opcode);
			break;
		case 0x76:
			// HLT
			unimplementedInstruction(state);
			break;
		case 0x77:
			// MOV M, A
			mov_m2r(state->memory, &state->a, &state->h, &state->l, opcode);
			break;
		case 0x78:
			// MOV A, B
			mov_r2r(&state->a, &state->b, opcode);
			break;
		case 0x79:
			// MOV A, C
			mov_r2r(&state->a, &state->c, opcode);
			break;
		case 0x7a:
			// MOV A, D
			mov_r2r(&state->a, &state->d, opcode);
			break;
		case 0x7b:
			// MOV A, E
			mov_r2r(&state->a, &state->e, opcode);
			break;
		case 0x7c:
			// MOV A, H
			mov_r2r(&state->a, &state->h, opcode);
			break;
		case 0x7d:
			// MOV A, L
			mov_r2r(&state->a, &state->l, opcode);
			break;
		case 0x7e:
			// MOV A, M
			mov_m2r(state->memory, &state->a, &state->h, &state->l, opcode);
			break;
		case 0x7f:
			// MOV A, A
			mov_r2r(&state->a, &state->a, opcode);
			break;
		case 0x80:
			// ADD B
			unimplementedInstruction(state);
			break;
		case 0x81:
			// ADD C
			unimplementedInstruction(state);
			break;
		case 0x82:
			// ADD D
			unimplementedInstruction(state);
			break;
		case 0x83:
			// ADD E
			unimplementedInstruction(state);
			break;
		case 0x84:
			// ADD H
			unimplementedInstruction(state);
			break;
		case 0x85:
			// ADD L
			unimplementedInstruction(state);
			break;
		case 0x86:
			// ADD M
			unimplementedInstruction(state);
			break;
		case 0x87:
			// ADD A
			unimplementedInstruction(state);
			break;
		case 0x88:
			// ADC B
			unimplementedInstruction(state);
			break;
		case 0x89:
			// ADC C
			unimplementedInstruction(state);
			break;
		case 0x8a:
			// ADC D
			unimplementedInstruction(state);
			break;
		case 0x8b:
			// ADC E
			unimplementedInstruction(state);
			break;
		case 0x8c:
			// ADC H
			unimplementedInstruction(state);
			break;
		case 0x8d:
			// ADC L
			unimplementedInstruction(state);
			break;
		case 0x8e:
			// ADC M
			unimplementedInstruction(state);
			break;
		case 0x8f:
			// ADC A
			unimplementedInstruction(state);
			break;
		case 0x90:
			// SUB B
			unimplementedInstruction(state);
			break;
		case 0x91:
			// SUB C
			unimplementedInstruction(state);
			break;
		case 0x92:
			// SUB D
			unimplementedInstruction(state);
			break;
		case 0x93:
			// SUB E
			unimplementedInstruction(state);
			break;
		case 0x94:
			// SUB H
			unimplementedInstruction(state);
			break;
		case 0x95:
			// SUB L
			unimplementedInstruction(state);
			break;
		case 0x96:
			// SUB M
			unimplementedInstruction(state);
			break;
		case 0x97:
			// SUB A
			unimplementedInstruction(state);
			break;
		case 0x98:
			// SBB B
			unimplementedInstruction(state);
			break;
		case 0x99:
			// SBB C
			unimplementedInstruction(state);
			break;
		case 0x9a:
			// SBB D
			unimplementedInstruction(state);
			break;
		case 0x9b:
			// SBB E
			unimplementedInstruction(state);
			break;
		case 0x9c:
			// SBB H
			unimplementedInstruction(state);
			break;
		case 0x9d:
			// SBB I
			unimplementedInstruction(state);
			break;
		case 0x9e:
			// SBB M
			unimplementedInstruction(state);
			break;
		case 0x9f:
			// SBB A
			unimplementedInstruction(state);
			break;
		case 0xa0:
			// ANA B
			unimplementedInstruction(state);
			break;
		case 0xa1:
			// ANA C
			unimplementedInstruction(state);
			break;
		case 0xa2:
			// ANA D
			unimplementedInstruction(state);
			break;
		case 0xa3:
			// ANA E
			unimplementedInstruction(state);
			break;
		case 0xa4:
			// ANA H
			unimplementedInstruction(state);
			break;
		case 0xa5:
			// ANA L
			unimplementedInstruction(state);
			break;
		case 0xa6:
			// ANA M
			unimplementedInstruction(state);
			break;
		case 0xa7:
			// ANA A
			unimplementedInstruction(state);
			break;
		case 0xa8:
			// XRA B
			unimplementedInstruction(state);
			break;
		case 0xa9:
			// XRA C
			unimplementedInstruction(state);
			break;
		case 0xaa:
			// XRA D
			unimplementedInstruction(state);
			break;
		case 0xab:
			// XRA E
			unimplementedInstruction(state);
			break;
		case 0xac:
			// XRA H
			unimplementedInstruction(state);
			break;
		case 0xad:
			// XRA L
			unimplementedInstruction(state);
			break;
		case 0xae:
			// XRA M
			unimplementedInstruction(state);
			break;
		case 0xaf:
			// XRA A
			unimplementedInstruction(state);
			break;
		case 0xb0:
			// ORA B
			unimplementedInstruction(state);
			break;
		case 0xb1:
			// ORA C
			unimplementedInstruction(state);
			break;
		case 0xb2:
			// ORA D
			unimplementedInstruction(state);
			break;
		case 0xb3:
			// ORA E
			unimplementedInstruction(state);
			break;
		case 0xb4:
			// ORA H
			unimplementedInstruction(state);
			break;
		case 0xb5:
			// ORA L
			unimplementedInstruction(state);
			break;
		case 0xb6:
			// ORA M
			unimplementedInstruction(state);
			break;
		case 0xb7:
			// ORA A
			unimplementedInstruction(state);
			break;
		case 0xb8:
			// CMP B
			unimplementedInstruction(state);
			break;
		case 0xb9:
			// CMP C
			unimplementedInstruction(state);
			break;
		case 0xba:
			// CMP D
			unimplementedInstruction(state);
			break;
		case 0xbb:
			// CMP E
			unimplementedInstruction(state);
			break;
		case 0xbc:
			// CMP H
			unimplementedInstruction(state);
			break;
		case 0xbd:
			// CMP L
			unimplementedInstruction(state);
			break;
		case 0xbe:
			// CMP M
			unimplementedInstruction(state);
			break;
		case 0xbf:
			// CMP A
			unimplementedInstruction(state);
			break;
		case 0xc0:
			// RNZ
			unimplementedInstruction(state);
			break;
		case 0xc1:
			// POP B
			unimplementedInstruction(state);
			break;
		case 0xc2:
			// JNZ adr
			unimplementedInstruction(state);
			break;
		case 0xc3:
			// JMP adr
			jmp(state, opcode);
			break;
		case 0xc4:
			// CNZ adr
			unimplementedInstruction(state);
			break;
		case 0xc5:
			// PUSH B
			unimplementedInstruction(state);
			break;
		case 0xc6:
			// ADI D8
			unimplementedInstruction(state);
			break;
		case 0xc7:
			// RST 0
			unimplementedInstruction(state);
			break;
		case 0xc8:
			// RZ
			unimplementedInstruction(state);
			break;
		case 0xc9:
			// RET
			unimplementedInstruction(state);
			break;
		case 0xca:
			// JZ adr
			unimplementedInstruction(state);
			break;
		case 0xcc:
			// CZ adr
			unimplementedInstruction(state);
			break;
		case 0xcd:
			// CALL adr
			call(state, opcode);
			break;
		case 0xce:
			// ACI D8
			unimplementedInstruction(state);
			break;
		case 0xcf:
			// RST 1
			unimplementedInstruction(state);
			break;
		case 0xd0:
			// RNC
			unimplementedInstruction(state);
			break;
		case 0xd1:
			// POP D
			unimplementedInstruction(state);
			break;
		case 0xd2:
			// JNC adr
			unimplementedInstruction(state);
			break;
		case 0xd3:
			// OUT D8
			unimplementedInstruction(state);
			break;
		case 0xd4:
			// CNC adr
			unimplementedInstruction(state);
			break;
		case 0xd5:
			// PUSH D
			unimplementedInstruction(state);
			break;
		case 0xd6:
			// SUI D8
			unimplementedInstruction(state);
			break;
		case 0xd7:
			// RST 2
			unimplementedInstruction(state);
			break;
		case 0xd8:
			// RC
			unimplementedInstruction(state);
			break;
		case 0xda:
			// JC adr 
			unimplementedInstruction(state);
			break;
		case 0xdb:
			// IN D8
			unimplementedInstruction(state);
			break;
		case 0xdc:
			// CC adr
			unimplementedInstruction(state);
			break;
		case 0xde:
			// SBI D8
			unimplementedInstruction(state);
			break;
		case 0xdf:
			// RST 3
			unimplementedInstruction(state);
			break;
		case 0xe0:
			// RPO
			unimplementedInstruction(state);
			break;
		case 0xe1:
			// POP H
			unimplementedInstruction(state);
			break;
		case 0xe2:
			// JPO adr
			unimplementedInstruction(state);
			break;
		case 0xe3:
			// XTHL
			unimplementedInstruction(state);
			break;
		case 0xe4:
			// CPO adr
			unimplementedInstruction(state);
			break;
		case 0xe5:
			// PUSH H
			unimplementedInstruction(state);
			break;
		case 0xe6:
			// ANI D8
			unimplementedInstruction(state);
			break;
		case 0xe7:
			// RST 4
			unimplementedInstruction(state);
			break;
		case 0xe8:
			// RPE
			unimplementedInstruction(state);
			break;
		case 0xe9:
			// PCHL
			unimplementedInstruction(state);
			break;
		case 0xea:
			// JPE adr
			unimplementedInstruction(state);
			break;
		case 0xeb:
			// XCHG
			unimplementedInstruction(state);
			break;
		case 0xec:
			// CPE adr
			unimplementedInstruction(state);
			break;
		case 0xee:
			// XRI D8
			unimplementedInstruction(state);
			break;
		case 0xef:
			// RST 5
			unimplementedInstruction(state);
			break;
		case 0xf0:
			// RP
			unimplementedInstruction(state);
			break;
		case 0xf1:
			// POP PSW
			unimplementedInstruction(state);
			break;
		case 0xf2:
			// JP adr
			unimplementedInstruction(state);
			break;
		case 0xf3:
			// DI
			unimplementedInstruction(state);
			break;
		case 0xf4:
			// CP adr 
			unimplementedInstruction(state);
			break;
		case 0xf5:
			// PUSH PSW
			unimplementedInstruction(state);
			break;
		case 0xf6:
			// ORI D8
			unimplementedInstruction(state);
			break;
		case 0xf7:
			// RST 6
			unimplementedInstruction(state);
			break;
		case 0xf8:
			// RM
			unimplementedInstruction(state);
			break;
		case 0xf9:
			// SPHL
			unimplementedInstruction(state);
			break;
		case 0xfa:
			// JM adr
			unimplementedInstruction(state);
			break;
		case 0xfb:
			// EI
			unimplementedInstruction(state);
			break;
		case 0xfc:
			// CM adr
			unimplementedInstruction(state);
			break;
		case 0xfe:
			// CPI D8
			unimplementedInstruction(state);
			break;
		case 0xff:
			// RST 7
			unimplementedInstruction(state);
			break;
		default:
			// We encountered an unknown instruction
			printf("Unknown Instruction: 0x%02x", *opcode);
			break;
	}

	return 0;
}

// Gets called when an unimplemented instruction is encountered
void unimplementedInstruction(CPUState *state)
{
	printf("\n[ERROR]: Unimplemented instruction encountered\n");

	// PC will have incremented. Correct for that
	state->pc--;
	disassembleInstruction(state->memory, state->pc);
	printf("\n");
	exit(1);
}
